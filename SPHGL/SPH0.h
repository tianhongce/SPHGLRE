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
	double GRAVITY;//重力系数
	double SMOOTH_RANGE;//光滑核核半径
	double SMOOTH_RANGE2;//光滑核半径的平方
	double DENSITY;//平静时的密度
	double PRESSURE;//压力系数
	double VISCOSITY;//粘稠度系数

	vector<Particle> particles;//粒子数组
	//int numParticles;//粒子个数
	HashGrid* hashGrid;
	MCDistField* mcDistField = NULL;


	int group;//粒子发射组数
	int curGroup;
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

