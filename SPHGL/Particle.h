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
	//λ��
	//double x;
	//double y;
	//double z;
	Point3D pos;
	//�ٶ�
	double vx;
	double vy;
	double vz;
	Point3D velocity;
	//����
	double fx;
	double fy;
	double fz;
	Point3D force;
	//�ܶ�
	double density;
	//�ڲ�ѹǿ
	double pressure;
	//�ھ�
	vector<Particle*> neighbors;

public:
	//���������ó��ٶȺ�λ��
	void move(double xBottle, double yBottle, double zBottle, double wBottle, double hBottle, double lBottle);
	void collisionHandling(double xBottle, double yBottle, double zBottle, double wBottle, double hBottle, double lBottle, double dt);
};

