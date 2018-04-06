#pragma once
#include <time.h>
#include "Particle.h"
#include "MCDistField.h"
#include "HashGrid.h"

enum MODEL{
	BAM_BREAK,
	WATER_PIPE,
};

class SPH
{
public:
	SPH();
	~SPH();

public:
	double GRAVITY;//����ϵ��
	double SMOOTH_RANGE;//�⻬�˺˰뾶
	double SMOOTH_RANGE2;//�⻬�˰뾶��ƽ��
	double DENSITY;//ƽ��ʱ���ܶ�
	double PRESSURE;//ѹ��ϵ��
	double VISCOSITY;//ճ���ϵ��

	vector<Particle> particles;//��������
	//int numParticles;//���Ӹ���
	HashGrid* hashGrid;
	MCDistField* mcDistField = NULL;


	int group;//���ӷ�������
	int curGroup;
	/*ƿ�ӵ����һ����ʷ����*/
	double bottle_x;
	double bottle_y;
	double bottle_z;
	/*ƿ�ӵ����һ����ʷ�ٶȺ�Ŀǰ���ٶ�*/
	double bottle_vx1;
	double bottle_vx2;
	double bottle_vy1;
	double bottle_vy2;
	double bottle_vz1;
	double bottle_vz2;
	/*ƿ�ӵļ��ٶȣ�������*/
	double bottle_fx;
	double bottle_fy;
	double bottle_fz;
	/*ƿ�ӳߴ��λ��(�����½�)*/
	double xBottle;
	double yBottle;
	double zBottle;
	double wBottle;
	double hBottle;
	double lBottle;

public:
	/*һ��֡����ִ��һ��*/
	void frame();
	/*���ƿ�ӵ��˶�״̬����������״̬����¼����*/
	void bottleMove();
	void restart();

public:
	bool is2d = false;
	MODEL model = BAM_BREAK;
	bool isSurface = false;
	bool isDisField = false;

private:
	void initParameter();
	void initialize();
	void pour();
	void damBreak();
	void move();
	void GenerateSurface();
};

