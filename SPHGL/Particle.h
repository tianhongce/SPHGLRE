#pragma once
#include <vector>
#include <Point3D.h>

using namespace std;


class Particle
{
public:
	Particle();
	Particle(double x, double y, double z);
	~Particle();

public:
	//位置
	//double x;
	//double y;
	//double z;
	Point3D pos;
	//速度
	double vx;
	double vy;
	double vz;
	Point3D velocity;
	//受力
	double fx;
	double fy;
	double fz;
	Point3D force;
	//密度
	double density;
	//内部压强
	double pressure;
	//邻居
	vector<Particle*> neighbors;

public:
	//将受力作用成速度和位移
	void move(double xBottle, double yBottle, double zBottle, double wBottle, double hBottle, double lBottle);
	void collisionHandling(double xBottle, double yBottle, double zBottle, double wBottle, double hBottle, double lBottle, double dt);
};

