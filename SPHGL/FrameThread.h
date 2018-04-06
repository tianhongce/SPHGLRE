#pragma once
#include <qthread.h>
#include <qtimer.h>
#include <QTime>
#include "SPH.h"

using namespace std;

class FrameThread : public QThread
{
	Q_OBJECT
private:
	FrameThread();
	static FrameThread* frameThread;

public:
	~FrameThread();
	static FrameThread* getInstance(SPH* s = NULL, int* fs = NULL);

public:
	QTimer timer;//������
	QTime time;	//ʱ��
	int frame;	//֡��
	int curFrame = 0;
	float timeStep;	  //ÿ֡ʱ�䲽��
	static int *frameSec;

private:
	// SPH
	static SPH* sph;

private:
	void run();

public:
	void start();
	void stop();

	public slots:
	//����֡
	void timeFrame();

};

