#pragma once
#include "HGrid.h"


/***************
*��ϣ��������Ԫ��.
*ÿ���������һ������Ԫ��
*/
class GridCell
{
public:
	GridCell(double ox, double oy, double oz, GridIndex &gi)
	{
		this->ox = ox;
		this->oy = oy;
		this->oz = oz;

		gridIndex = gi;
	}

	GridCell(GridCell *gc)
	{
		if (gc == NULL)
			return;

		this->ox = gc->ox;
		this->oy = gc->oy;
		this->oz = gc->oz;
		this->idxParticle = gc->idxParticle;
		this->gridIndex = gc->gridIndex;
		this->next = gc->next;
	}

	bool operator=(GridCell *gc)
	{
		if (gc == NULL)
			return false;

		this->ox = gc->ox;
		this->oy = gc->oy;
		this->oz = gc->oz;
		this->idxParticle = gc->idxParticle;
		this->gridIndex = gc->gridIndex;
		this->next = gc->next;

		return true;
	}
public:
	vector<int> idxParticle;	// ����Ԫ������������n��
	double ox, oy, oz;				// ����Ԫ��㣬Ψһ
	GridIndex gridIndex;		// ����Ԫ������������Ψһ
	GridCell *next = NULL;
};

/***************
*��ϣ������
*/
class HashGrid : public HGrid
{
public:
	HashGrid();
	HashGrid(double len, uint hashSize, double xSize, double ySize, double zSize, double xOrigin = 0.0, double yOrigin = 0.0, double zOrigin = 0.0);
	~HashGrid();

	bool operator=(HashGrid &hashGrid);

public:
	int numParticle = 0;//����ռ�����������
	int numCell = 0;	//����Ԫ����

public:
	vector<GridCell*> hash;//�����ϣ��

public:
	//��������Ԫ���ȣ��Ϳռ�xyz���򳤶ȡ���㣬���ֿռ�����
	bool Partition(double &len, double xSize, double ySize, double zSize, double ox, double oy, double oz);
	//ɾ����ϣ����ÿ������Ԫ
	bool DeleteHash();
	//�������ռ������
	bool ClearGrid();

public:
	//��ȡ�����������꣬��������ֵ
	GridIndex GetGridIndex(double x, double y, double z);
	//��ȡ��ϣֵ����������λ��
	int GetHashIndex(double x, double y, double z);
	//��ȡ��ϣֵ��ͨ����������
	int GetHashIndex(GridIndex &idxGrid);
	//��ȡ��ϣ��
	int GetHshSize();
	//ͨ����ϣ���ȡ����Ԫ
	GridCell* GetGridCell(GridIndex &idxGrid);

public:
	//������Ԫ��ӵ���ϣ��
	//bool AddHashCell(double x, double y, double z, GridCell* gridCell);
	//bool AddHashCell(int hashIndex, GridCell* gridCell);
	bool AddHashCell(GridIndex idxGrid, GridCell* gridCell);
	//������������ӵ�����Ԫ
	bool AddParticle(double x, double y, double z, int idx);
	//������(x,y,z)��Χ�뾶radii��Χ�ڵ��ڵ�
	bool SearchAdjParticles(double x, double y, double z, double radii, const vector<Particle> &particles, vector<int> &adjParticles);
};

