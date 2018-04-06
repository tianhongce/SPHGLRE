#include "SPH.h"
#include <QtDebug>

#define T_CHAR(str)  QString::fromLocal8Bit(str)	//ת���ַ���
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
	GRAVITY = -0.5;//����ϵ��
	SMOOTH_RANGE = 12;//�⻬�˺˰뾶
	SMOOTH_RANGE2 = SMOOTH_RANGE * SMOOTH_RANGE;//�⻬�˰뾶��ƽ��
	DENSITY = 1.0;//ƽ��ʱ���ܶ�
	PRESSURE = 2.0;//ѹ��ϵ��
	VISCOSITY = 0.075;//ճ���ϵ��

	//����
	group = 20000;//���ӷ�������
	curGroup = 0;

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

	xBottle = -150;
	yBottle = -100;
	zBottle = -100;
	wBottle = 300;
	hBottle = 200;
	lBottle = 200;

}

void SPH::initialize()
{
	//����SPH����
	initParameter();
	bottle_x = xBottle;
	bottle_y = yBottle;
	bottle_z = zBottle;

	//ʵ����ϣ����
	hashGrid = new HashGrid(SMOOTH_RANGE, 2000000, wBottle + 60, hBottle + 60, lBottle + 60, xBottle - 30, yBottle - 30, zBottle - 30);
	//ʵ�����볡
	mcDistField = new MCDistField(2, 5000000, wBottle + 20, 160, lBottle + 20, xBottle - 10, yBottle - 10, zBottle - 10);

}

/*һ��֡����ִ��һ��*/
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
			pour();//��������
			curGroup++;
			break;
		default:
			break;
		}
	}

	bottleMove();

	//���������˶�
	move();

	//���ɱ���
	GenerateSurface();

}

//���ɱ���
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

//����
void SPH::damBreak()
{
	int numParticle = 12000;//������
	int wcount = 30;
	int lcount = 20;
	int hcount = numParticle / (wcount*lcount);//group * 5 / (wcount*lcount);
	//����ֱ��
	double dia = 8;

	for (int yi = 1; yi <= hcount; yi++)
	{
		for (int xi = 0; xi < wcount; xi++)
		{
			for (int zi = 0; zi < lcount; zi++)
			{
				particles.push_back(Particle(20 + xBottle + xi * dia, yBottle + 0.2 + yi * dia, 20 + zBottle + zi * dia));

				//������ӵ�����
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
//	//����ֱ��
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
//				//������ӵ�����
//				hashGrid.AddParticle(particles.back().x, particles.back().y, particles.back().z, particles.size() - 1);
//			}
//		}
//	}
//}


/*����һ������*/
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

//���¿�ʼ
void SPH::restart()
{
	curGroup = 0;
	particles.clear();
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

/*ִ��һ�����ӵļ���*/
void SPH::move()
{
	int i;//��ѭ���м����
	int j;//��ѭ���м����
	double dist;//����
	Particle pi;//��ѭ��������
	Particle pj;//��ѭ��������
	double dx;//������x����
	double dy;//������y����
	double dz;//������z����
	double weight;//�⻬�˺���ֵ�����ֵ��
	double pressure;//ѹ����С
	double viscosity;//ճ��������С

	static int s = 0, e = 0;
	s = clock();

	/*��ѭ�������ӷ������ڹ⻬�˰뾶��Χ�ڵ����ӻ�Ϊ�ھ�*/
	for (i = 0; i < particles.size(); i++)
	{
		Particle &pi = particles[i];//ȡ��ѭ��Ҫ����������
		particles[i].neighbors.clear();
		pi.density = 0;//�������ܶ����Գ�ʼ��

		//�����������꣬������Χ����Ԫ,3*3*3���ٶ�(1w,debug)��900ms/f2R��400ms/fR.(10w,relerse):9s/fR
		GridIndex idxGrid = hashGrid->GetGridIndex(pi.x, pi.y, pi.z);//ԭ��������
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
							dist = pow(pi.x - pj.x, 2) + pow(pi.y - pj.y, 2) + pow(pi.z - pj.z, 2);//�����������ӵľ���ƽ��
							/*����������Ӿ����ڷ�Χ�� �����ж�*/
							if (dist < SMOOTH_RANGE2&&dist != 0.0)
							{
								dist = sqrt(dist);//���Ӽ��
								weight = pow(1 - dist / SMOOTH_RANGE, 2);//����⻬�˺�����ֵ
								//��ֵ�ۼӵ����ӵ��ܶ���
								pi.density += weight;
								//����ھ�
								pi.neighbors.push_back(&pj);

							}
						}
					}
				}
			}
		}

		//��ѭ���������ֹ������ѭ���������ٶ�(1w,debug)��1500ms/f
		//for (j = 0; j < i; j++)
		//{
		//	Particle &pj = particles[j];//ȡ��ѭ������
		//	dist = pow(pi.x - pj.x, 2) + pow(pi.y - pj.y, 2) + pow(pi.z - pj.z, 2);//�����������ӵľ���ƽ��
		//	/*����������Ӿ����ڷ�Χ�� �����ж�*/
		//	if (dist < SMOOTH_RANGE2)
		//	{
		//		dist = sqrt(dist);//���Ӽ��
		//		weight = pow(1 - dist / SMOOTH_RANGE, 2);//����⻬�˺�����ֵ
		//		//��ֵ�ۼӵ��������ӵ��ܶ���
		//		pi.density += weight;
		//		pj.density += weight;
		//		//����Ϊ�ھ�
		//		pi.neighbors.push_back(&pj);
		//		pj.neighbors.push_back(&pi);
		//	}
		//}
	}

	DEBUG_OUTPUT("�����ܶ����ھӣ�")

		/*������ӵ�ѹǿ*/
		for (i = 0; i < particles.size(); i++)
		{
			Particle &pi = particles[i];
			if (pi.density < DENSITY) pi.density = DENSITY;
			pi.pressure = (pi.density - DENSITY)*1;//����ѹ������K(�����ܶ�-��̬�ܶ�)��KĬ��Ϊ1
		}

	DEBUG_OUTPUT("����ѹǿ��")

		/*�����������ܱ�����ӵ�ѹ��������*/
		for (i = 0; i < particles.size(); i++)
		{
			Particle &pi = particles[i];
			//��ʼ�����ӵ�����
			pi.fx = 0;
			pi.fy = 0;
			pi.fz = 0;
			//ѭ���ڼ��������Ӹ�������ӵ�ѹ��������
			for (j = 0; j < pi.neighbors.size(); j++)
			{
				pj = *pi.neighbors[j];
				//�����������ӵ������
				dx = pi.x - pj.x;
				dy = pi.y - pj.y;
				dz = pi.z - pj.z;
				dist = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));//�����������ӵľ���
				weight = 1 - dist / SMOOTH_RANGE;//�⻬�˺�����һ��΢�ֺ�
				pressure = weight * (pi.pressure + pj.pressure) / (2 * pj.density) * PRESSURE;//�������Ӷ����ӵ�ѹ������
				/*��ѹ���ֳ�����������ֵ�ۼƵ�������*/
				dist = 1 / dist;
				dx *= dist;
				dy *= dist;
				dz *= dist;
				pi.fx += dx * pressure;
				pi.fy += dy * pressure;
				pi.fz += dz * pressure;
				viscosity = weight / pj.density * VISCOSITY;//������ٶ��������ϵ��
				//������������ٶȵ�����
				dx = pi.vx - pj.vx;
				dy = pi.vy - pj.vy;
				dz = pi.vz - pj.vz;
				//�����ٶ����� �ó����Ӷ����ӵ�ѹ���ۼ�������
				pi.fx -= dx * viscosity;
				pi.fy -= dy * viscosity;
				pi.fz -= dz * viscosity;
			}
		}

	DEBUG_OUTPUT("����ѹ����ճ������")

		hashGrid->ClearGrid();
	for (i = 0; i < particles.size(); i++)
	{
		Particle &pi = particles[i];
		//�����ⲿƿ�����������䷴���ۼ�
		pi.fx -= bottle_fx;
		pi.fy -= bottle_fy;
		pi.fz -= bottle_fz;
		pi.fy += GRAVITY;
		//���������ó��ٶȺ�λ��
		if (false == is2d)
		{
			pi.move(xBottle, yBottle, zBottle, wBottle, hBottle, lBottle);
		}
		else
		{
			pi.move(xBottle, yBottle, wBottle, hBottle);
		}

		//��������ӵ�����
		hashGrid->AddParticle(pi.x, pi.y, pi.z, i);
	}
	DEBUG_OUTPUT("���������ͳ��λ�ƣ�")

}
