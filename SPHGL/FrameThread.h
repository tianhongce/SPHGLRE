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
	QTimer timer;//计数器
	QTime time;	//时钟
	int frame;	//帧数
	int curFrame = 0;
	float timeStep;	  //每帧时间步长
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
	//动画帧
	void timeFrame();

};

