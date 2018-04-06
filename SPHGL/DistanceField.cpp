#include "DistanceField.h"

DistanceField::DistanceField()
{
}

DistanceField::DistanceField(double len, uint hashSize, double xSize, double ySize, double zSize, double xOrigin, double yOrigin, double zOrigin)
{
	if (xSize == 0)
	{
		xSize = 1;
	}
	if (ySize == 0)
	{
		ySize = 1;
	}
	if (zSize == 0)
	{
		zSize = 1;
	}

	this->lenght = len;
	this->xLen = xSize;
	this->yLen = ySize;
	this->zLen = zSize;
	this->ox = xOrigin;
	this->oy = yOrigin;
	this->oz = zOrigin;
	hash.resize(hashSize, NULL);

	//���ֿռ�����
	Partition(len, xSize, ySize, zSize, xOrigin, yOrigin, zOrigin);
}

DistanceField::~DistanceField()
{
	DeleteHash();
}

bool DistanceField::Partition(double &len, double xSize, double ySize, double zSize, double ox, double oy, double oz)
{
	//��������Ԫ������
	int xSum = xSize / len + 1;
	int ySum = ySize / len + 1;
	int zSum = zSize / len + 1;

	numX = xSum;
	numY = ySum;
	numZ = zSum;

	//��ǰ�����嵥Ԫ����㣬(OpenGL�����º�)
	double curx = ox;
	double cury = oy;
	double curz = oz;

	GridIndex idxGrid;
	DistanceNode* gridNode;
	//��ʼ���ֵ�Ԫ�񣬲���ӵ���ϣ��
	for (int i = 0; i < xSum; i++)
	{
		for (int j = 0; j < ySum; j++)
		{
			for (int k = 0; k < zSum; k++)
			{
				//��ȡ��ǰ��������
				idxGrid = this->GetGridIndex(curx, cury, curz);

				//�������Ԫ����ϣ��
				gridNode = new DistanceNode(curx, cury, curz, idxGrid);
				this->AddGridNode(idxGrid, gridNode);
				curz += len;//��ǰz����
			}
			curz = oz;
			cury += len;//��ǰy����
		}
		cury = oy;
		curx += len;//��ǰx����
	}

	return true;
}

bool DistanceField::DeleteHash()
{
	DistanceNode* gridNode = NULL;
	DistanceNode* next = NULL;
	for (int i = 0; i < hash.size(); i++)
	{
		gridNode = hash[i];
		if (gridNode != NULL)
		{
			next = gridNode->next;
			delete gridNode;
			while (next != NULL)
			{
				gridNode = next;
				next = gridNode->next;
				if (gridNode != NULL)
					delete gridNode;
			}
		}
	}
	return true;
}

bool DistanceField::ClearGrid()
{
	//��������Ԫ������
	int xSum = xLen / lenght + 1;
	int ySum = yLen / lenght + 1;
	int zSum = zLen / lenght + 1;

	//��ǰ�����嵥Ԫ����㣬(OpenGL�����º�)
	double curx = ox;
	double cury = oy;
	double curz = oz;

	GridIndex idxGrid;
	DistanceNode* gridNode;
	//����ÿ���ڵ�����꣬����ÿ���ڵ�
	for (int i = 0; i < xSum; i++)
	{
		for (int j = 0; j < ySum; j++)
		{
			for (int k = 0; k < zSum; k++)
			{
				//��ȡ����Ԫ
				gridNode = GetGridNode(curx, cury, curz);

				gridNode->data = MAX_DIS;

				curz += lenght;//��ǰz����
			}
			curz = oz;
			cury += lenght;//��ǰy����
		}
		cury = oy;
		curx += lenght;//��ǰx����
	}


	return true;
}

/********************����*********************/
int DistanceField::GetHshSize()
{
	return hash.size();
}

GridIndex DistanceField::GetGridIndex(double x, double y, double z)
{
	//������������������ȡ��(x/len)..
	return GridIndex(floor(float(x / lenght)), floor(float(y / lenght)), floor(float(z / lenght)));
}

//��ȡ��ϣֵ����������λ��
int DistanceField::GetHashIndex(double x, double y, double z)
{
	if (hash.empty())
	{
		return -1;
	}	//��������
	int ix = floor(float(x / lenght));
	int iy = floor(float(y / lenght));
	int iz = floor(float(z / lenght));

	//��ϣ����
	return (ix*P1 ^ iy*P2 ^ iz*P3) % hash.size();
}

//��ȡ��ϣֵ��ͨ����������
int DistanceField::GetHashIndex(GridIndex &idxGrid)
{
	if (hash.empty())
	{
		return -1;
	}
	//��������
	int ix = idxGrid.x;
	int iy = idxGrid.y;
	int iz = idxGrid.z;

	return (ix*P1 ^ iy*P2 ^ iz*P3) % hash.size();
}

DistanceNode* DistanceField::GetGridNode(double x, double y, double z)
{
	//��������������ȡ��ϣ����
	int hashIndex = GetHashIndex(x, y, z);
	DistanceNode* gridNode = hash[hashIndex];

	DistanceNode goalNode(x, y, z);
	//ֱ���ҵ���Ӧ������Ԫ
	while (gridNode != NULL && *gridNode != goalNode)
	{
		gridNode = gridNode->next;
	}

	if (gridNode == NULL)
		return gridNode;

	return gridNode;
}

double DistanceField::GetNodeData(double x, double y, double z)
{
	return GetGridNode(x, y, z)->data;
}

/********************���*********************/
bool DistanceField::AddGridNode(GridIndex idxGrid, DistanceNode* gridNode)
{
	if (gridNode == NULL)
		return false;
	int idxHash = GetHashIndex(idxGrid);
	if (idxHash > hash.size())
	{
		return false;
	}

	/*�ڵ�ǰ��ϣ�ڵ�������У��ҵ���β�����������Ԫ*/
	DistanceNode* curNode = NULL;
	curNode = hash[idxHash];
	if (curNode == NULL)
	{
		hash[idxHash] = gridNode;

		numNode++;
		return true;
	}
	//�����ڵ������ҵ���β
	while (curNode->next != NULL)
	{
		curNode = curNode->next;
	}
	curNode->next = gridNode;

	numNode++;

	return true;
}

bool DistanceField::AddData(double x, double y, double z, double data)
{
	DistanceNode* gridNode = this->GetGridNode(x, y, z);

	if (gridNode == NULL)
		return false;

	gridNode->data = data;

	return true;
}

bool DistanceField::ComputDistanceField(vector<Particle> &particles, HashGrid &hashGrid, double radius)
{
	//��������ڵ������
	int xSum = xLen / lenght + 1;
	int ySum = yLen / lenght + 1;
	int zSum = zLen / lenght + 1;

	//��ǰ�ڵ����꣬(OpenGL�����º�)
	double curx = ox;
	double cury = oy;
	double curz = oz;

	GridIndex idxGrid;
	DistanceNode* gridNode;
	//����ÿ���ڵ�����꣬����ÿ���ڵ㣻����ڵ����ֵ��
	for (int i = 0; i < xSum; i++)
	{
		for (int j = 0; j < ySum; j++)
		{
			for (int k = 0; k < zSum; k++)
			{
				//��ȡ����ڵ�
				gridNode = GetGridNode(curx, cury, curz);

				//������ʽ���棬�������
				gridNode->data = GetDisValue(curx, cury, curz, particles, hashGrid, radius);

				curz += lenght;//��ǰz����
			}
			curz = oz;
			cury += lenght;//��ǰy����
		}
		cury = oy;
		curx += lenght;//��ǰx����
	}

	return true;
}

#define max(a,b) (b<a?a:b)

double DistanceField::GetDisValue(double x, double y, double z, const vector<Particle> &particles, HashGrid &hashGrid, double radius)
{
	double ri = radius;//ÿ�����ӵİ뾶
	const double SR = 4 * ri;//������뾶

	vector<int> adjParticles;//��ǰ���ӵ��ڽ�����
	hashGrid.SearchAdjParticles(x, y, z, SR, particles, adjParticles);

	if (adjParticles.empty())
	{
		return MAX_DIS;
	}

	double _x = 0.0;
	double _y = 0.0;
	double _z = 0.0;
	double _r = 0.0;
	double R = SR;//��������뾶
	double wi = 0.0;//Ȩֵ
	double s = 0.0;
	double distValue = MAX_DIS;

	Particle pi;//����i
	Particle pj;//����j
	//���������ڵ�ÿ�����ӣ������Ȩƽ��ֵ
	for (int i = 0; i < adjParticles.size(); i++)
	{
		pi = particles[adjParticles[i]];

		double uws = abs(sqrt(pow((pi.pos.x - x), 2) + pow((pi.pos.y - y), 2) + pow((pi.pos.z - z), 2))) / R;
		double uw = max(0, pow((1 - uws*uws), 3));

		double dw = 0.0;
		for (int j = 0; j < adjParticles.size(); j++)
		{
			pj = particles[adjParticles[j]];

			double dws = abs(sqrt(pow((pj.pos.x - x), 2) + pow((pj.pos.y - y), 2) + pow((pj.pos.z - z), 2))) / R;
			dw += max(0, pow((1 - dws*dws), 3));
		}

		wi = ((uw == 0 && dw == 0) ? 1 : (uw / dw));

		_x += wi*pi.pos.x;
		_y += wi*pi.pos.y;
		_z += wi*pi.pos.z;

		_r += wi * ri;
	}

	distValue = abs(sqrt(pow((x - _x), 2) + pow((y - _y), 2) + pow((z - _z), 2))) - _r;

	return distValue;
}