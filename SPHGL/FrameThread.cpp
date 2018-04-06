#include "FrameThread.h"

FrameThread* FrameThread::frameThread = NULL;
SPH* FrameThread::sph = NULL;
int* FrameThread::frameSec = NULL;

FrameThread::FrameThread()
{
	frame = 0;
	frameSec = 0;
	timeStep = 10;		//֡���,ms
}

FrameThread::~FrameThread()
{
	if (frameThread != NULL)
		delete frameThread;
}

FrameThread* FrameThread::getInstance(SPH* s, int* fs)
{
	if (frameThread == NULL)
	{
		frameThread = new FrameThread;
	}
	if (sph == NULL)
	{
		sph = s;
	}
	if (frameSec == NULL)
	{
		frameSec = fs;
	}

	return frameThread;
}

void FrameThread::run()
{
	/*****��ʼģ��*****/
	timer.start(timeStep);

	//��ʼʱ�ӣ��Լ�����һ֡
	time.start();
	timeFrame();

	QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timeFrame()));
}

void FrameThread::timeFrame()
{
	static int s = 0, e = 0;
	s = clock();

	sph->frame();//һ֡

	curFrame++;
	*frameSec = clock() - s;
	if (1000 <= time.elapsed())
	{
		time.restart();
		frame = curFrame;
		curFrame = 0;
	}

	e = clock();
	//qDebug() << "frame��" << e - s;
}

void  FrameThread::start()
{
	if (sph == NULL)
	{
		return;
	}

	this->run();
}

void  FrameThread::stop()
{
	if (!this->isRunning())
	{
		return;
	}
	timer.stop();
	this->wait();
}