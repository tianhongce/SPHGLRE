#include <SPH.h>
#include <QtDebug>

SPH::SPH()
{
	Param::initParam();
	initialize();
}

SPH::~SPH()
{
	delete hashGrid;
	delete mcDistField;
}

void SPH::initParameter()
{
	GRAVITY.set(0, -9.8, 0);//重力系数
	SMOOTH_RANGE = 0.112;//光滑核核半径
	SMOOTH_RANGE2 = SMOOTH_RANGE * SMOOTH_RANGE;//光滑核半径的平方
	REST_DENSITY = 1000.0;//平静时的密度
	VISCOSITY = 0.35;//粘稠度系数
	MASS = 0.1756;
	RADIUS = 0.028;
	TIME_STEP = 0.00025;

	isSurface = Param::ctrlParam[IS_SURFACE];
	isDisField = Param::ctrlParam[IS_DIS_FIELD];

	//粒子
	numParticle = 800;//粒子数

	/*瓶子的最近一次历史坐标*/
	bottle_x = 0;
	bottle_y = 0;
	bottle_z = 0;
	/*瓶子的最近一次历史速度和目前的速度*/
	bottle_vx1 = 0;
	bottle_vx2 = 0;
	bottle_vy1 = 0;
	bottle_vy2 = 0;
	bottle_vz1 = 0;
	bottle_vz2 = 0;
	/*瓶子的加速度（受力）*/
	bottle_fx = 0;
	bottle_fy = 0;
	bottle_fz = 0;

	xBottle = -0.4;
	yBottle = -0.4;
	zBottle = -0.3;
	wBottle = 0.8;
	hBottle = 0.8;
	lBottle = 0.6;

}

void SPH::initialize()
{
	//设置SPH参数
	initParameter();
	bottle_x = xBottle;
	bottle_y = yBottle;
	bottle_z = zBottle;

	//实例哈希网格
	hashGrid = new HashGrid(SMOOTH_RANGE, 2000000, wBottle - 0.01, hBottle - 0.01, lBottle - 0.01, xBottle - 0.02, yBottle - 0.02, zBottle - 0.02);
	//实例距离场
	mcDistField = new MCDistField(0.1, 5000000, wBottle * 1.5, hBottle*1.2, lBottle*1.5, xBottle - 0.1, yBottle - 0.1, zBottle - 0.1);

	damBreak();

}

/*一个帧周期执行一次*/
void SPH::frame()
{
	bottleMove();

	//一帧迭代次数
	int del = int((1.0/25.0) / TIME_STEP);

	//计算粒子运动
	while (del-- > 0)
	{
		run();
	}

	//生成表面
	generateSurface();
}


//生成表面
void SPH::generateSurface()
{
	if (isDisField == true || isSurface == true){
		mcDistField->ComputDistanceField(particles, *hashGrid, RADIUS);
	}
	else
	{
		return;
	}

	if (isSurface == true)
	{
		mcDistField->GenerateMesh();
	}
}

//溃坝
void SPH::damBreak()
{
	int wcount = 10;
	int lcount = 6;
	int hcount = numParticle / (wcount*lcount);

	//流体坐标
	const double ox = 0.05 + xBottle;	//x轴
	const double oy = 0.00 + yBottle;	//y轴
	const double oz = 0.2 + zBottle;	//深度
	//const double ox = 0.00 + xBottle;	//x轴
	//const double oy = 0.00 + yBottle;	//y轴
	//const double oz = 0.0 + zBottle;	//深度

	//粒子间距
	double dia = pow(MASS / REST_DENSITY, 1.0 / 3.0)*1.2;

	for (int yi = 0; yi < hcount; yi++)
	{
		for (int xi = 0; xi < wcount; xi++)
		{
			for (int zi = 0; zi < lcount; zi++)
			{
				particles.push_back(Particle(ox + xi * dia, oy +
					yi * dia, oz + zi * dia));

				//粒子添加到网格
				hashGrid->AddParticle(particles.back().pos.x, particles.back().pos.y, particles.back().pos.z, particles.size() - 1);
			}
		}
	}

	setupKernels();
}

//重新开始
void SPH::restart()
{
	particles.clear();
	damBreak();
	hashGrid->ClearGrid();
}

/*检测瓶子的运动状态和外力受力状态并记录数据*/
void SPH::bottleMove()
{
	bottle_vx2 = xBottle - bottle_x;//记录速度
	bottle_x = xBottle;//将位置保存到历史
	bottle_fx = (bottle_vx2 - bottle_vx1) / 5;//记录受力
	bottle_vx1 = bottle_vx2;//保存历史速度

	bottle_vy2 = yBottle - bottle_y;
	bottle_y = yBottle;
	bottle_fy = (bottle_vy2 - bottle_vy1) / 5;
	bottle_vy1 = bottle_vy2;

	bottle_vz2 = zBottle - bottle_z;
	bottle_z = zBottle;
	bottle_fz = (bottle_vz2 - bottle_vz1) / 5;
	bottle_vz1 = bottle_vz2;
}

//执行一次粒子的计算
void SPH::run()
{
	searchNeighborParticle();

	computePressure();

	computeForce();

	advanceCollision();
}

//---------搜索相邻粒子，保存
void SPH::searchNeighborParticle()
{
	/*该循环将粒子分区，在光滑核半径范围内的粒子互为邻居*/
	for (int i = 0; i < particles.size(); i++)
	{
		Particle &pi = particles[i];//取外循环要操作的粒子
		particles[i].neighbors.clear();
		pi.density = 0;//将粒子密度属性初始化

		//根据粒子坐标，遍历周围网格单元,3*3*3;
		GridIndex idxGrid = hashGrid->GetGridIndex(pi.pos.x, pi.pos.y, pi.pos.z);//原网格索引
		for (int ixc = -1; ixc <= 1; ixc++)
		{
			for (int iyc = -1; iyc <= 1; iyc++)
			{
				for (int izc = -1; izc <= 1; izc++)
				{
					GridCell* gridCell = hashGrid->GetGridCell(GridIndex(idxGrid.x + ixc, idxGrid.y + iyc, idxGrid.z + izc));//当前网格
					if (gridCell != NULL)
					{
						for (int icell = 0; icell < gridCell->idxParticle.size(); icell++)
						{
							Particle &pj = particles[gridCell->idxParticle[icell]];//取内循环粒子
							double dist = pow(pi.pos.x - pj.pos.x, 2) + pow(pi.pos.y - pj.pos.y, 2) + pow(pi.pos.z - pj.pos.z, 2);//计算两个粒子的距离平方
							/*如果两个粒子距离在范围内 进入判断*/
							if (dist < SMOOTH_RANGE2&&dist != 0.0)
							{
								dist = sqrt(dist);//粒子间距
								double weight = poly6Kern(dist);//算出光滑核函数的值
								//将值累加到粒子的密度里
								pi.density += weight*MASS;
								
								//添加邻居
								pi.neighbors.push_back(&pj);

							}

						}
					}
				}
			}
		}

		//cout << "density" << particles[i].density << endl;
	}
}

void SPH::computePressure()
{
	double k = 10;
	for (int i = 0; i < numParticle; i++)
	{
		
		Particle &pi = particles[i];

		if (pi.density < 1000.0)
		{
			pi.density = 1000.0;
		}

		pi.pressure = (k*REST_DENSITY / 7) * (pow(pi.density / REST_DENSITY, 7) - 1);

		//cout << "pressure" << particles[i].pressure << endl;
	}
}

//--------------计算合力
void SPH::computeForce()
{
	const double	mass = MASS;
	const double smooth_radius = SMOOTH_RANGE;//核半径
	const double smooth_radius_square = smooth_radius * smooth_radius;
	const double visc = VISCOSITY;//粘性系数
	const Point3D gravity = GRAVITY;//重力

	const double	mass_square = mass * mass;
	const double restVolume = mass / REST_DENSITY;//静止时，粒子体积

	for (int i = 0; i < numParticle; i++)
	{
		Particle &pi = particles[i];//取外循环要操作的粒子
		pi.force.set(0, 0, 0);
		//if (pi.neighbors.size() > 30)
		//{
		//	cout << pi.neighbors.size() << endl;
		//}
		
		for (int j = 0; j < pi.neighbors.size(); j++)
		{
			
			Particle pj = *pi.neighbors[j];

			//粒子间向量
			Point3D vector_iJ = pi.pos - pj.pos;
			const double dx = vector_iJ.x;
			const double dy = vector_iJ.y;
			const double dz = vector_iJ.z;

			const double dist_square = dx*dx + dy*dy + dz*dz;
			if (dist_square <= smooth_radius_square)
			{
				const double dist = sqrt(dist_square);//粒子间距离

				double kernelGradientValue = spikyKern(dist);//压力的核函数值
				Point3D kernelGradient = vector_iJ * kernelGradientValue;
				pi.force -= kernelGradient * MASS*MASS * (pi.pressure / pi.density + pj.pressure / pj.density);

				Point3D vel_eval_j_i = pj.velocity - pi.velocity;
				vel_eval_j_i /= dist;
				pi.force += vel_eval_j_i * visc * restVolume * restVolume * (smooth_radius - dist) * (45.0 / (PI * pow(SMOOTH_RANGE, 6)));//累积合力
			}
		}

		pi.force += (gravity * mass);//累积重力

		//cout << pi.force.x<<"," << pi.force.y << "," << pi.force.z <<  endl;
	}
}

//-----------移动和碰撞检测
void SPH::advanceCollision()
{
	Point3D norm;
	Point3D clr;
	double adj;
	double speed;
	double diff;

	hashGrid->ClearGrid();
	for (int i = 0; i < numParticle; i++)
	{
		Particle &pi = particles[i];
		Point3D acceleration = pi.force;
		acceleration /= MASS;

		pi.velocity += acceleration * TIME_STEP;
		pi.pos += pi.velocity * TIME_STEP;

		pi.collisionHandling(xBottle, yBottle, zBottle, wBottle, hBottle, lBottle, TIME_STEP);

		//将粒子添加到网格
		hashGrid->AddParticle(pi.pos.x, pi.pos.y, pi.pos.z, i);
	}
}

double SPH::poly6Kern(double dist)
{
	if (SMOOTH_RANGE < dist)
		return 0;

	return 315.0 / (64.0 * PI * pow(SMOOTH_RANGE, 9)) * pow((SMOOTH_RANGE*SMOOTH_RANGE - dist*dist), 3);
}

double SPH::spikyKern(double dist)
{
	if (SMOOTH_RANGE < dist)
		return 0;

	return -45.0 / (PI * pow(SMOOTH_RANGE, 6)) * pow((SMOOTH_RANGE - dist), 2) * dist;
}
double SPH::lapKern(double dist)
{
	if (SMOOTH_RANGE < dist)
		return 0;

	return 45.0 / (PI * pow(SMOOTH_RANGE, 6)) * (SMOOTH_RANGE - dist);
}


void SPH::setupKernels()
{

}
