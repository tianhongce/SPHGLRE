#include "Particle.h"

Particle::Particle()
{
}

Particle::Particle(double x, double y, double z)
{
	vx = vy = vz = fx = fy = fz = 0;


	pos.set(x, y, z);
	velocity.set(0, 0, 0);
	force.set(0, 0, 0);
	neighbors.clear();

}

Particle::~Particle()
{
}

void Particle::collisionHandling(double xBottle, double yBottle, double zBottle, double wBottle, double hBottle, double lBottle, double dt)
{
	//���ײǽ���ٶ�Ϊ0���������ǽ�����ܵ��ͳ���������ͬ�����ķ���
	double d = 0.0001;
	if (pos.x < d + xBottle)
	{
		velocity.x += (d - pos.x + xBottle) / dt;
	}
	if (pos.y < d + yBottle)
	{
		velocity.y += (d - pos.y + yBottle) / dt;
	}
	if (pos.z < d + zBottle)
	{
		velocity.z += (d - pos.z + zBottle) / dt;
	}
	if (pos.x > wBottle - d + xBottle)
	{
		velocity.x += (wBottle - d - pos.x + xBottle) / dt;
	}

	////����
	//if (pos.y > hBottle*2 - d + yBottle)
	//{
	//	velocity.y += (hBottle*2 - d - pos.y + yBottle) / dt;
	//}

	if (pos.z > lBottle - d + zBottle)
	{
		velocity.z += (lBottle - d - pos.z + zBottle) / dt;
	}
}
