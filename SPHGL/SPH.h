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
	Point3D GRAVITY;//�������ٶ�
	double SMOOTH_RANGE;//�⻬�˺˰뾶
	double SMOOTH_RANGE2;//�⻬�˰뾶��ƽ��
	double REST_DENSITY;//ƽ��ʱ���ܶ�
	double VISCOSITY;//ճ���ϵ��
	double MASS;//��������
	double RADIUS;//���Ӱ뾶
	double TIME_STEP;//ʱ�䲽��

public:
	vector<Particle> particles;//��������
	HashGrid* hashGrid;
	MCDistField* mcDistField = NULL;

public:
	int numParticle;//��������
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
	bool isSurface;
	bool isDisField;

private:
	//double poly6Kern;//�ܶ�
	//double spikyKern;//ѹǿ
	//double lapKern;//ճ��

private:
	double maxPredictedDensity;
	double avgPredictedDensity;
	double searchDist = 0.0;
	double kernelself;
	double denErrFactor;
	Point3D*  sumGradW;
	double* sumGradWDot;

private:
	void initParameter();
	void initialize();
	void damBreak();
	void generateSurface();

	double poly6Kern(double dist);//�ܶ�
	double spikyKern(double dist);//ѹǿ
	double lapKern(double dist);//ճ��
	double densityErrorFactor(Particle &pi);


	void setupKernels();

private:
	void run();
	void searchNeighborParticle();
	void computePressure();
	void computeForce();
	void advanceCollision();//��ײ���
};
