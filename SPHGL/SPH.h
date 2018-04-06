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
	Point3D GRAVITY;//重力加速度
	double SMOOTH_RANGE;//光滑核核半径
	double SMOOTH_RANGE2;//光滑核半径的平方
	double REST_DENSITY;//平静时的密度
	double VISCOSITY;//粘稠度系数
	double MASS;//粒子质量
	double RADIUS;//粒子半径
	double TIME_STEP;//时间步长

public:
	vector<Particle> particles;//粒子数组
	HashGrid* hashGrid;
	MCDistField* mcDistField = NULL;

public:
	int numParticle;//粒子总数
	/*瓶子的最近一次历史坐标*/
	double bottle_x;
	double bottle_y;
	double bottle_z;
	/*瓶子的最近一次历史速度和目前的速度*/
	double bottle_vx1;
	double bottle_vx2;
	double bottle_vy1;
	double bottle_vy2;
	double bottle_vz1;
	double bottle_vz2;
	/*瓶子的加速度（受力）*/
	double bottle_fx;
	double bottle_fy;
	double bottle_fz;
	/*瓶子尺寸和位置(后左下角)*/
	double xBottle;
	double yBottle;
	double zBottle;
	double wBottle;
	double hBottle;
	double lBottle;

public:
	/*一个帧周期执行一次*/
	void frame();
	/*检测瓶子的运动状态和外力受力状态并记录数据*/
	void bottleMove();
	void restart();

public:
	bool is2d = false;
	bool isSurface;
	bool isDisField;

private:
	//double poly6Kern;//密度
	//double spikyKern;//压强
	//double lapKern;//粘性

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

	double poly6Kern(double dist);//密度
	double spikyKern(double dist);//压强
	double lapKern(double dist);//粘性
	double densityErrorFactor(Particle &pi);


	void setupKernels();

private:
	void run();
	void searchNeighborParticle();
	void computePressure();
	void computeForce();
	void advanceCollision();//碰撞检测
};
