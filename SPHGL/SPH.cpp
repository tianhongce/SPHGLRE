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
	GRAVITY.set(0, -9.8, 0);//����ϵ��
	SMOOTH_RANGE = 0.112;//�⻬�˺˰뾶
	SMOOTH_RANGE2 = SMOOTH_RANGE * SMOOTH_RANGE;//�⻬�˰뾶��ƽ��
	REST_DENSITY = 1000.0;//ƽ��ʱ���ܶ�
	VISCOSITY = 0.35;//ճ���ϵ��
	MASS = 0.1756;
	RADIUS = 0.028;
	TIME_STEP = 0.00025;

	isSurface = Param::ctrlParam[IS_SURFACE];
	isDisField = Param::ctrlParam[IS_DIS_FIELD];

	//����
	numParticle = 800;//������

	/*ƿ�ӵ����һ����ʷ����*/
	bottle_x = 0;
	bottle_y = 0;
	bottle_z = 0;
	/*ƿ�ӵ����һ����ʷ�ٶȺ�Ŀǰ���ٶ�*/
	bottle_vx1 = 0;
	bottle_vx2 = 0;
	bottle_vy1 = 0;
	bottle_vy2 = 0;
	bottle_vz1 = 0;
	bottle_vz2 = 0;
	/*ƿ�ӵļ��ٶȣ�������*/
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
	//����SPH����
	initParameter();
	bottle_x = xBottle;
	bottle_y = yBottle;
	bottle_z = zBottle;

	//ʵ����ϣ����
	hashGrid = new HashGrid(SMOOTH_RANGE, 2000000, wBottle - 0.01, hBottle - 0.01, lBottle - 0.01, xBottle - 0.02, yBottle - 0.02, zBottle - 0.02);
	//ʵ�����볡
	mcDistField = new MCDistField(0.1, 5000000, wBottle * 1.5, hBottle*1.2, lBottle*1.5, xBottle - 0.1, yBottle - 0.1, zBottle - 0.1);

	damBreak();

}

/*һ��֡����ִ��һ��*/
void SPH::frame()
{
	bottleMove();

	//һ֡��������
	int del = int((1.0/25.0) / TIME_STEP);

	//���������˶�
	while (del-- > 0)
	{
		run();
	}

	//���ɱ���
	generateSurface();
}


//���ɱ���
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

//����
void SPH::damBreak()
{
	int wcount = 10;
	int lcount = 6;
	int hcount = numParticle / (wcount*lcount);

	//��������
	const double ox = 0.05 + xBottle;	//x��
	const double oy = 0.00 + yBottle;	//y��
	const double oz = 0.2 + zBottle;	//���
	//const double ox = 0.00 + xBottle;	//x��
	//const double oy = 0.00 + yBottle;	//y��
	//const double oz = 0.0 + zBottle;	//���

	//���Ӽ��
	double dia = pow(MASS / REST_DENSITY, 1.0 / 3.0)*1.2;

	for (int yi = 0; yi < hcount; yi++)
	{
		for (int xi = 0; xi < wcount; xi++)
		{
			for (int zi = 0; zi < lcount; zi++)
			{
				particles.push_back(Particle(ox + xi * dia, oy +
					yi * dia, oz + zi * dia));

				//������ӵ�����
				hashGrid->AddParticle(particles.back().pos.x, particles.back().pos.y, particles.back().pos.z, particles.size() - 1);
			}
		}
	}

	setupKernels();
}

//���¿�ʼ
void SPH::restart()
{
	particles.clear();
	damBreak();
	hashGrid->ClearGrid();
}

/*���ƿ�ӵ��˶�״̬����������״̬����¼����*/
void SPH::bottleMove()
{
	bottle_vx2 = xBottle - bottle_x;//��¼�ٶ�
	bottle_x = xBottle;//��λ�ñ��浽��ʷ
	bottle_fx = (bottle_vx2 - bottle_vx1) / 5;//��¼����
	bottle_vx1 = bottle_vx2;//������ʷ�ٶ�

	bottle_vy2 = yBottle - bottle_y;
	bottle_y = yBottle;
	bottle_fy = (bottle_vy2 - bottle_vy1) / 5;
	bottle_vy1 = bottle_vy2;

	bottle_vz2 = zBottle - bottle_z;
	bottle_z = zBottle;
	bottle_fz = (bottle_vz2 - bottle_vz1) / 5;
	bottle_vz1 = bottle_vz2;
}

//ִ��һ�����ӵļ���
void SPH::run()
{
	searchNeighborParticle();

	computePressure();

	computeForce();

	advanceCollision();
}

//---------�����������ӣ�����
void SPH::searchNeighborParticle()
{
	/*��ѭ�������ӷ������ڹ⻬�˰뾶��Χ�ڵ����ӻ�Ϊ�ھ�*/
	for (int i = 0; i < particles.size(); i++)
	{
		Particle &pi = particles[i];//ȡ��ѭ��Ҫ����������
		particles[i].neighbors.clear();
		pi.density = 0;//�������ܶ����Գ�ʼ��

		//�����������꣬������Χ����Ԫ,3*3*3;
		GridIndex idxGrid = hashGrid->GetGridIndex(pi.pos.x, pi.pos.y, pi.pos.z);//ԭ��������
		for (int ixc = -1; ixc <= 1; ixc++)
		{
			for (int iyc = -1; iyc <= 1; iyc++)
			{
				for (int izc = -1; izc <= 1; izc++)
				{
					GridCell* gridCell = hashGrid->GetGridCell(GridIndex(idxGrid.x + ixc, idxGrid.y + iyc, idxGrid.z + izc));//��ǰ����
					if (gridCell != NULL)
					{
						for (int icell = 0; icell < gridCell->idxParticle.size(); icell++)
						{
							Particle &pj = particles[gridCell->idxParticle[icell]];//ȡ��ѭ������
							double dist = pow(pi.pos.x - pj.pos.x, 2) + pow(pi.pos.y - pj.pos.y, 2) + pow(pi.pos.z - pj.pos.z, 2);//�����������ӵľ���ƽ��
							/*����������Ӿ����ڷ�Χ�� �����ж�*/
							if (dist < SMOOTH_RANGE2&&dist != 0.0)
							{
								dist = sqrt(dist);//���Ӽ��
								double weight = poly6Kern(dist);//����⻬�˺�����ֵ
								//��ֵ�ۼӵ����ӵ��ܶ���
								pi.density += weight*MASS;
								
								//����ھ�
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

//--------------�������
void SPH::computeForce()
{
	const double	mass = MASS;
	const double smooth_radius = SMOOTH_RANGE;//�˰뾶
	const double smooth_radius_square = smooth_radius * smooth_radius;
	const double visc = VISCOSITY;//ճ��ϵ��
	const Point3D gravity = GRAVITY;//����

	const double	mass_square = mass * mass;
	const double restVolume = mass / REST_DENSITY;//��ֹʱ���������

	for (int i = 0; i < numParticle; i++)
	{
		Particle &pi = particles[i];//ȡ��ѭ��Ҫ����������
		pi.force.set(0, 0, 0);
		//if (pi.neighbors.size() > 30)
		//{
		//	cout << pi.neighbors.size() << endl;
		//}
		
		for (int j = 0; j < pi.neighbors.size(); j++)
		{
			
			Particle pj = *pi.neighbors[j];

			//���Ӽ�����
			Point3D vector_iJ = pi.pos - pj.pos;
			const double dx = vector_iJ.x;
			const double dy = vector_iJ.y;
			const double dz = vector_iJ.z;

			const double dist_square = dx*dx + dy*dy + dz*dz;
			if (dist_square <= smooth_radius_square)
			{
				const double dist = sqrt(dist_square);//���Ӽ����

				double kernelGradientValue = spikyKern(dist);//ѹ���ĺ˺���ֵ
				Point3D kernelGradient = vector_iJ * kernelGradientValue;
				pi.force -= kernelGradient * MASS*MASS * (pi.pressure / pi.density + pj.pressure / pj.density);

				Point3D vel_eval_j_i = pj.velocity - pi.velocity;
				vel_eval_j_i /= dist;
				pi.force += vel_eval_j_i * visc * restVolume * restVolume * (smooth_radius - dist) * (45.0 / (PI * pow(SMOOTH_RANGE, 6)));//�ۻ�����
			}
		}

		pi.force += (gravity * mass);//�ۻ�����

		//cout << pi.force.x<<"," << pi.force.y << "," << pi.force.z <<  endl;
	}
}

//-----------�ƶ�����ײ���
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

		//��������ӵ�����
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
