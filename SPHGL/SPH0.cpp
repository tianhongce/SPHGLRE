#include "SPH.h"
#include <QtDebug>

#define T_CHAR(str)  QString::fromLocal8Bit(str)	//转换字符串
#define DEBUG_OUTPUT(str) 	{e = clock();qDebug() << T_CHAR((str)) << e - s;s = clock(); }

SPH::SPH()
{
	initialize();
}

SPH::~SPH()
{
	delete hashGrid;
	delete mcDistField;
}

void SPH::initParameter()
{
	GRAVITY = -0.5;//重力系数
	SMOOTH_RANGE = 12;//光滑核核半径
	SMOOTH_RANGE2 = SMOOTH_RANGE * SMOOTH_RANGE;//光滑核半径的平方
	DENSITY = 1.0;//平静时的密度
	PRESSURE = 2.0;//压力系数
	VISCOSITY = 0.075;//粘稠度系数

	//粒子
	group = 20000;//粒子发射组数
	curGroup = 0;

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

	xBottle = -150;
	yBottle = -100;
	zBottle = -100;
	wBottle = 300;
	hBottle = 200;
	lBottle = 200;

}

void SPH::initialize()
{
	//设置SPH参数
	initParameter();
	bottle_x = xBottle;
	bottle_y = yBottle;
	bottle_z = zBottle;

	//实例哈希网格
	hashGrid = new HashGrid(SMOOTH_RANGE, 2000000, wBottle + 60, hBottle + 60, lBottle + 60, xBottle - 30, yBottle - 30, zBottle - 30);
	//实例距离场
	mcDistField = new MCDistField(2, 5000000, wBottle + 20, 160, lBottle + 20, xBottle - 10, yBottle - 10, zBottle - 10);

}

/*一个帧周期执行一次*/
void SPH::frame()
{
	if (curGroup < group)
	{
		switch (model)
		{
		case BAM_BREAK:
			damBreak();
			curGroup = group;
			break;
		case WATER_PIPE:
			pour();//发射粒子
			curGroup++;
			break;
		default:
			break;
		}
	}

	bottleMove();

	//计算粒子运动
	move();

	//生成表面
	GenerateSurface();

}

//生成表面
void SPH::GenerateSurface()
{
	if (isDisField == true || isSurface == true){
		mcDistField->ComputDistanceField(particles, *hashGrid, 3);
	}
	else{
		return;
	}

	if (isSurface == true){
		mcDistField->GenerateMesh();
	}
}

//溃坝
void SPH::damBreak()
{
	int numParticle = 12000;//粒子数
	int wcount = 30;
	int lcount = 20;
	int hcount = numParticle / (wcount*lcount);//group * 5 / (wcount*lcount);
	//粒子直径
	double dia = 8;

	for (int yi = 1; yi <= hcount; yi++)
	{
		for (int xi = 0; xi < wcount; xi++)
		{
			for (int zi = 0; zi < lcount; zi++)
			{
				particles.push_back(Particle(20 + xBottle + xi * dia, yBottle + 0.2 + yi * dia, 20 + zBottle + zi * dia));

				//粒子添加到网格
				hashGrid->AddParticle(particles.back().x, particles.back().y, particles.back().z, particles.size() - 1);
			}
		}
	}
}
//void SPH::damBreak()
//{
//	int wcount = 10;
//	int lcount = 10;
//	int hcount = group * 5 / (wcount*lcount);
//	//粒子直径
//	int dia = 23;
//
//	for (int yi = 1; yi <= hcount; yi++)
//	{
//		for (int xi = 0; xi < wcount; xi++)
//		{
//			for (int zi = 0; zi < lcount; zi++)
//			{
//				particles.push_back(Particle(50 + xBottle + xi * dia, yBottle + 20 + yi * dia, 20 + zBottle + zi * dia));
//
//				//粒子添加到网格
//				hashGrid.AddParticle(particles.back().x, particles.back().y, particles.back().z, particles.size() - 1);
//			}
//		}
//	}
//}


/*发射一组粒子*/
void SPH::pour()
{
	for (int i = -2; i <= 2; i++)
	{
		particles.push_back(Particle(xBottle + wBottle*0.7 - double(i * 36), yBottle + hBottle - 0.002, 0));
		particles.back().vy = -10;
		particles.back().vx = -10;
		particles.back().vz = -1;

		hashGrid->AddParticle(particles.back().x, particles.back().y, particles.back().z, particles.size() - 1);
	}

	int i = 0;
}

//重新开始
void SPH::restart()
{
	curGroup = 0;
	particles.clear();
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

/*执行一次粒子的计算*/
void SPH::move()
{
	int i;//外循环中间变量
	int j;//内循环中间变量
	double dist;//距离
	Particle pi;//外循环的粒子
	Particle pj;//内循环的粒子
	double dx;//向量的x分量
	double dy;//向量的y分量
	double dz;//向量的z分量
	double weight;//光滑核函数值（或变值）
	double pressure;//压力大小
	double viscosity;//粘稠阻力大小

	static int s = 0, e = 0;
	s = clock();

	/*该循环将粒子分区，在光滑核半径范围内的粒子互为邻居*/
	for (i = 0; i < particles.size(); i++)
	{
		Particle &pi = particles[i];//取外循环要操作的粒子
		particles[i].neighbors.clear();
		pi.density = 0;//将粒子密度属性初始化

		//根据粒子坐标，遍历周围网格单元,3*3*3，速度(1w,debug)：900ms/f2R，400ms/fR.(10w,relerse):9s/fR
		GridIndex idxGrid = hashGrid->GetGridIndex(pi.x, pi.y, pi.z);//原网格索引
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
							dist = pow(pi.x - pj.x, 2) + pow(pi.y - pj.y, 2) + pow(pi.z - pj.z, 2);//计算两个粒子的距离平方
							/*如果两个粒子距离在范围内 进入判断*/
							if (dist < SMOOTH_RANGE2&&dist != 0.0)
							{
								dist = sqrt(dist);//粒子间距
								weight = pow(1 - dist / SMOOTH_RANGE, 2);//算出光滑核函数的值
								//将值累加到粒子的密度里
								pi.density += weight;
								//添加邻居
								pi.neighbors.push_back(&pj);

							}
						}
					}
				}
			}
		}

		//内循环，用握手规则减少循环次数，速度(1w,debug)：1500ms/f
		//for (j = 0; j < i; j++)
		//{
		//	Particle &pj = particles[j];//取内循环粒子
		//	dist = pow(pi.x - pj.x, 2) + pow(pi.y - pj.y, 2) + pow(pi.z - pj.z, 2);//计算两个粒子的距离平方
		//	/*如果两个粒子距离在范围内 进入判断*/
		//	if (dist < SMOOTH_RANGE2)
		//	{
		//		dist = sqrt(dist);//粒子间距
		//		weight = pow(1 - dist / SMOOTH_RANGE, 2);//算出光滑核函数的值
		//		//将值累加到两个粒子的密度里
		//		pi.density += weight;
		//		pj.density += weight;
		//		//互加为邻居
		//		pi.neighbors.push_back(&pj);
		//		pj.neighbors.push_back(&pi);
		//	}
		//}
	}

	DEBUG_OUTPUT("计算密度与邻居：")

		/*算出粒子的压强*/
		for (i = 0; i < particles.size(); i++)
		{
			Particle &pi = particles[i];
			if (pi.density < DENSITY) pi.density = DENSITY;
			pi.pressure = (pi.density - DENSITY)*1;//粒子压力等于K(粒子密度-静态密度)，K默认为1
		}

	DEBUG_OUTPUT("计算压强：")

		/*计算粒子所受别的粒子的压力和阻力*/
		for (i = 0; i < particles.size(); i++)
		{
			Particle &pi = particles[i];
			//初始化粒子的受力
			pi.fx = 0;
			pi.fy = 0;
			pi.fz = 0;
			//循环内计算别的粒子给这个粒子的压力和阻力
			for (j = 0; j < pi.neighbors.size(); j++)
			{
				pj = *pi.neighbors[j];
				//计算两个粒子的坐标差
				dx = pi.x - pj.x;
				dy = pi.y - pj.y;
				dz = pi.z - pj.z;
				dist = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));//计算两个粒子的距离
				weight = 1 - dist / SMOOTH_RANGE;//光滑核函数的一阶微分和
				pressure = weight * (pi.pressure + pj.pressure) / (2 * pj.density) * PRESSURE;//计算粒子对粒子的压力标量
				/*将压力分成向量的两个值累计到受力中*/
				dist = 1 / dist;
				dx *= dist;
				dy *= dist;
				dz *= dist;
				pi.fx += dx * pressure;
				pi.fy += dy * pressure;
				pi.fz += dz * pressure;
				viscosity = weight / pj.density * VISCOSITY;//计算除速度外的阻力系数
				//计算粒子相对速度的向量
				dx = pi.vx - pj.vx;
				dy = pi.vy - pj.vy;
				dz = pi.vz - pj.vz;
				//乘入速度因子 得出粒子对粒子的压力累加入受力
				pi.fx -= dx * viscosity;
				pi.fy -= dy * viscosity;
				pi.fz -= dz * viscosity;
			}
		}

	DEBUG_OUTPUT("计算压力和粘性力：")

		hashGrid->ClearGrid();
	for (i = 0; i < particles.size(); i++)
	{
		Particle &pi = particles[i];
		//考虑外部瓶子受力，将其反向累计
		pi.fx -= bottle_fx;
		pi.fy -= bottle_fy;
		pi.fz -= bottle_fz;
		pi.fy += GRAVITY;
		//将受力作用成速度和位移
		if (false == is2d)
		{
			pi.move(xBottle, yBottle, zBottle, wBottle, hBottle, lBottle);
		}
		else
		{
			pi.move(xBottle, yBottle, wBottle, hBottle);
		}

		//将粒子添加到网格
		hashGrid->AddParticle(pi.x, pi.y, pi.z, i);
	}
	DEBUG_OUTPUT("计算合力，统计位移：")

}
