#include "ModelWidget.h"
#include <freeglut.h>

ModelWidget::ModelWidget(Ui::SPHGLClass *ui, int id, QWidget* parent)
	:QOpenGLWidget(parent)
{
	this->id = id;
	this->ui = ui;
	initializeGL();
	initWidget();
}

ModelWidget::~ModelWidget()
{
}

//**********************************初始化*********************************************
void ModelWidget::initWidget()
{
	isMouseDown = false;			//鼠标是否按下
	frame = 0;
	sumFrame = 0;
	frameSec = 0;
	rotX = 0;//初始化旋转角度
	rotY = 10;
	zoom = -1.2;
	posX = 0;
	posY = 0;
	isLock = false;
	isBackground = false;

	isShowParticle = Param::ctrlParam[IS_PARTICLE];
	isShowDisField = Param::ctrlParam[IS_DIS_FIELD];
	isShowSurface = Param::ctrlParam[IS_SURFACE];
	isShowHashGrid = Param::ctrlParam[IS_HASH_GRID];
	isOutputFrame = Param::ctrlParam[IS_OUTPUT_FRAME];

	//设置参数
	//ratio = 3.6 / pcisph.wBottle;//缩放比例
	ratio = 1;
	ballRadius = 0.028;	//粒子半径
	timeStep = 10;		//帧间隔,ms

	lightPosition[0] = 0.0f;	//光源位置,xyz,dis
	lightPosition[1] = 1.5f;
	lightPosition[2] = 0.0f;
	lightPosition[3] = 0.0f;
	lightDirection[0] = 0.0f;	//光照方向
	lightDirection[0] = -1.0f;
	lightDirection[0] = 0.0f;

	//设置并获取背景
	double bgy = pcisph.yBottle * ratio - 0.1;//背景y坐标
	setBackground(30, 30, bgy, 1.0);
	setFocusPolicy(Qt::StrongFocus);

	/*****开始模拟*****/
	//timer.start(timeStep);
	timer.stop();

	//开始时钟，以及播放一帧
	time.start();
	//timeFrame();

	QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timeFrame()));

}

void ModelWidget::initializeGL()
{
	glClearColor(0, 0, 0, 1);//背景色

	glEnable(GL_DEPTH_TEST);//深度测试，深度值有效
	glEnable(GL_BLEND);		//颜色混合
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);//启用颜色追踪
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);		//设置深度范围为最大

	//设置光照参数
	setLight();
}

void ModelWidget::setLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//GLfloat lightPosition[] = { 0.0, 1.2, -1.0, 1.0 };		//光源位置,xyz,dis
	GLfloat lightAmbient[] = { 0.6, 0.6, 0.6, 1.0 };		//全局光属性,光强度
	GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat lightDirection[] = { 0.0f, -1.0f, 0.0f };	//光照方向
	GLfloat lightExponent[] = { 1.0f };					//聚光程度
	GLfloat lightCutoff[] = { 60.0f };					//光源夹角

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//指定需要颜色追踪的材料属性
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//指定第0号光源的位置 
	//反射
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse); //漫反射后
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);//镜面反射后
	//聚光
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
	//glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, lightExponent);
	//glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, lightCutoff);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);
}

void ModelWidget::resizeGL(int width, int height)
{
	iWinWidth = (GLint)width;
	iWinHeight = (GLint)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//投影方式：透视投影
	gluPerspective(60, (GLfloat)iWinWidth / (GLfloat)iWinHeight, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

//**********************************事件*********************************************
void ModelWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (isMouseDown)
	{
		int deltX, deltY;
		// 计算鼠标移动距离，旋转图像
		deltX = e->x() - mouseX;
		deltY = e->y() - mouseY;

		if (false == isLock)
		{
			// 旋转角  
			rotX += deltX / 2;
			rotY += deltY / 2;
			// 旋转角不超过360度 
			rotX = fmodf(rotX, 360.0);
			rotY = fmodf(rotY, 360.0);
		}
		else
		{
			pcisph.xBottle += double(deltX) / this->width() / ratio * 4;
			pcisph.yBottle -= double(deltY) / this->height() / ratio * 4;
		}

		//更新当前鼠标位置，使图像实时旋转
		mouseX = e->x();
		mouseY = e->y();
		update();
	}
}
void ModelWidget::mousePressEvent(QMouseEvent *e)
{
	mouseX = e->x();
	mouseY = e->y();

	isMouseDown = true;


}
void ModelWidget::mouseReleaseEvent(QMouseEvent *e)
{
	isMouseDown = false;
}
void ModelWidget::mouseDoubleClickEvent(QMouseEvent *e)
{

}
void ModelWidget::wheelEvent(QWheelEvent *e)
{
	int numDegrees = e->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
	double numSteps = numDegrees / 120.0;//滚动的步数，*15就是鼠标滚动的角度
	zoom += numSteps / 2;
	e->accept();      //接收该事件
	update();
}
void ModelWidget::keyPressEvent(QKeyEvent *e)
{
	double dstep = 0.1;
	switch (e->key())
	{
	case Qt::Key_W:
		posY += dstep;
		//zoom += (GLfloat)0.05;
		break;
	case Qt::Key_S:
		posY -= dstep;
		//zoom -= (GLfloat)0.05;
		break;
	case Qt::Key_A:
		posX -= dstep;
		//rotX -= (GLfloat)5;
		break;
	case Qt::Key_D:
		posX += dstep;
		//rotX += (GLfloat)5;
		break;
	}
	update();
}
void ModelWidget::timeFrame()
{
	static int s = 0, e = 0;
	s = clock();

	pcisph.frame();//一帧
	this->update();

	frame++;
	sumFrame++;
	frameSec = clock() - s;
	if (1000 <= time.elapsed())
	{
		ui->statusBar->showMessage(QString("FPS : %1 / s\t  Frame: %2 ms\t  sum: %3\t  frames:%4").arg(frame).arg(frameSec)
			.arg(pcisph.particles.size()).arg(sumFrame));
		time.restart();
		frame = 0;
	}


	if (isOutputFrame)
	{
		outputFrame();
	}

	e = clock();
	//qDebug() << T_CHAR("帧内间隔：") << e - s;
}

void ModelWidget::updateFrame()
{
	static int us = 0, ue = 0;
	us = clock();

	this->update();

	ui->statusBar->showMessage(QString("FPS : %1 / s\t  Frame: %2 ms\t  sum: %3").arg(1)
		.arg(frameSec).arg(pcisph.particles.size()));

	ue = clock();
	//qDebug() << T_CHAR("重绘间隔：") << ue - us;
}

//**********************************绘制*********************************************
void ModelWidget::paintGL()
{
	static int sd = 0, ed = 0;
	sd = clock();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glTranslatef(posX, posY, zoom);
	glRotatef(rotX, 0.0, 1.0, 0.0);
	glRotatef(rotY, 1.0, 0.0, 0.0);
	//glRotatef(90, 1.0, 0.0, 0.0);

	//移动光源同步移动
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//指定第0号光源的位置 
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

	//绘制原点
	GLUquadricObj *quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_SMOOTH);
	glColor4f(0.5, 0.5, 0.5, 0.5);
	gluSphere(quadricObj, 0.008, 5, 5);

	if (true == isBackground){
		drawBackground();//绘制背景
	}

	if (isShowDisField == true){
		drawDistFieldGrid();//距离场
	}

	if (isShowParticle){
		drawParticle();//绘制粒子
	}

	if (isShowSurface){
		drawFluid();//液面
	}

	if (isShowHashGrid){
		drawHashGrid();
	}

	drawBoundary();//绘制边界

	glPopMatrix();

	ed = clock();
	//qDebug() << T_CHAR("绘制内间隔：") << ed - sd;
}

void ModelWidget::drawHashGrid()
{
	//颜色
	glColor4f(0.1, 0.5, 0.5, 1.0);
	glPointSize(2);

	glPushMatrix();
	glScaled(ratio, ratio, ratio);

	glBegin(GL_POINTS);

	GridCell* gridCell = NULL;
	for (int i = 0; i < pcisph.hashGrid->hash.size(); i++)
	{
		gridCell = pcisph.hashGrid->hash[i];

		while (gridCell != NULL)
		{
			glPushMatrix();
			glVertex3d(gridCell->ox, gridCell->oy, gridCell->oz);
			glPopMatrix();

			gridCell = gridCell->next;
		}
	}

	glEnd();
	glPopMatrix();
}

void ModelWidget::drawDistFieldGrid()
{
	//颜色
	glColor4f(0.8, 0.1, 0.1, 1.0);
	glPointSize(2);

	glPushMatrix();
	glScaled(ratio, ratio, ratio);
	glBegin(GL_POINTS);

	DistanceNode* gridNode = NULL;
	for (int i = 0; i < pcisph.mcDistField->hash.size(); i++)
	{
		gridNode = pcisph.mcDistField->hash[i];

		while (gridNode != NULL)
		{
			if (gridNode->data < 0)
			{
				//液面内
				glColor4f(0.9, 0.1, 0.1, 1.0);
			}
			else
			{
				//液面外
				glColor4f(0.1, 0.1, 0.5, 0.8);
			}

			glPushMatrix();
			glVertex3d(gridNode->x, gridNode->y, gridNode->z);
			glPopMatrix();

			gridNode = gridNode->next;
		}

	}

	glEnd();
	glPopMatrix();
}

void ModelWidget::drawFluid()
{
	glPushMatrix();

	//液面颜色
	glColor4f(0.4, 0.5, 0.7, 1);
	//glColor4f(0, 0, 0.8, 1);

	glScaled(ratio, ratio, ratio);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	//定义顶点数组  
	glVertexPointer(3, GL_DOUBLE, 0, pcisph.mcDistField->mesh.data());
	glNormalPointer(GL_DOUBLE, 0, pcisph.mcDistField->meshVector.data());
	//glColorPointer(4, GL_FLOAT, 0, colorBackgroud.data());

	//绘制
	glDrawArrays(GL_TRIANGLES, 0, pcisph.mcDistField->mesh.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}

void ModelWidget::drawBoundary(GLenum mode)
{
	glPushMatrix();
	glScaled(ratio, ratio, ratio);
	//边界位置和大小
	double xb = pcisph.xBottle - ballRadius;
	double yb = pcisph.yBottle - ballRadius;
	double zb = pcisph.zBottle - ballRadius;
	double wb = pcisph.wBottle + ballRadius;
	double hb = pcisph.hBottle + ballRadius;
	double lb = pcisph.lBottle + ballRadius;

	//绘制边界
	glLineWidth(2);
	glColor4f(0.7, 0.7, 0.9, 0.5);//边界颜色
	//边线
	glBegin(GL_LINES);
	//后面
	glVertex3d(xb, yb + hb, zb);	//左上
	glVertex3d(xb, yb, zb);
	glVertex3d(xb, yb, zb);			//左下
	glVertex3d(xb + wb, yb, zb);
	glVertex3d(xb + wb, yb, zb);	//右下
	glVertex3d(xb + wb, yb + hb, zb);
	glVertex3d(xb + wb, yb + hb, zb);//右上
	glVertex3d(xb, yb + hb, zb);
	//前面
	glVertex3d(xb, yb + hb, zb + lb);
	glVertex3d(xb, yb, zb + lb);
	glVertex3d(xb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb + lb);
	glVertex3d(xb + wb, yb + hb, zb + lb);
	glVertex3d(xb + wb, yb + hb, zb + lb);
	glVertex3d(xb, yb + hb, zb + lb);
	//左侧
	glVertex3d(xb, yb + hb, zb);	//左上
	glVertex3d(xb, yb + hb, zb + lb);
	glVertex3d(xb, yb, zb);			//左下
	glVertex3d(xb, yb, zb + lb);
	//右侧
	glVertex3d(xb + wb, yb + hb, zb);//右上
	glVertex3d(xb + wb, yb + hb, zb + lb);
	glVertex3d(xb + wb, yb, zb);	//右下
	glVertex3d(xb + wb, yb, zb + lb);
	glEnd();

	//玻璃面
	glColor4f(0.9, 0.9, 1, 0.10);//边界颜色
	glBegin(GL_QUADS);
	//后玻璃
	glVertex3d(xb, yb + hb, zb);
	glVertex3d(xb, yb, zb);
	glVertex3d(xb + wb, yb, zb);
	glVertex3d(xb + wb, yb + hb, zb);
	//下玻璃
	glVertex3d(xb, yb, zb);			//后左下
	glVertex3d(xb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb);	//后右下
	//左玻璃
	glVertex3d(xb, yb + hb, zb);	//后左上
	glVertex3d(xb, yb + hb, zb + lb);//前左上
	glVertex3d(xb, yb, zb + lb);
	glVertex3d(xb, yb, zb);			//后左下
	//右玻璃
	glVertex3d(xb + wb, yb + hb, zb);//后右上
	glVertex3d(xb + wb, yb + hb, zb + lb);
	glVertex3d(xb + wb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb);	//后右下
	glEnd();

	glPopMatrix();
}

//绘制粒子
void ModelWidget::drawParticle()
{
	GLUquadricObj *quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_SMOOTH);

	//粒子颜色
	glColor4f(0.5, 0.55, 0.7, 0.9);

	glPushMatrix();

	//绘制每个粒子
	for (int i = 0; i < pcisph.particles.size(); i++)
	{
		Particle &p = pcisph.particles[i];

		//qDebug() << T_CHAR("密度：") << p.density;
		//if (p.density < 1.5)
		//{
		//	//密度小，红
		//	glColor4f(1.0, 0.55, 0.7, 0.95);//
		//}
		//else
		//{
		//	glColor4f(0.5, 0.55, 0.7, 0.95);
		//}

		glPushMatrix();
		glTranslatef(p.pos.x * ratio, p.pos.y * ratio, p.pos.z * ratio);
		gluSphere(quadricObj, ballRadius, 16, 16);
		glPopMatrix();
	}

	glPopMatrix();
}

void ModelWidget::drawBackground()
{
	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//定义顶点数组  
	glVertexPointer(3, GL_FLOAT, 0, bottomBackgroud.data());
	glColorPointer(4, GL_FLOAT, 0, colorBackgroud.data());

	//绘制
	glDrawArrays(GL_QUADS, 0, bottomBackgroud.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}

void ModelWidget::setBackground(float xr, float zr, float yp, float step)
{
	bottomBackgroud.clear();
	colorBackgroud.clear();

	//总长度,-xlen:xlen
	float xlen = xr;
	float zlen = zr;
	float ypos = yp;//y坐标

	//单格长度
	float steplen = step;

	//方格数量
	int xsum = xlen * 2 / steplen;
	int zsum = zlen * 2 / steplen;

	//背景不透明度
	float alpha = 0.5;

	//方格1的颜色
	float r1 = 0.7;
	float g1 = 0.7;
	float b1 = 0.7;
	float a1 = alpha;

	//方格2的颜色
	float r2 = 1;
	float g2 = 1;
	float b2 = 1;
	float a2 = alpha;

	//当前采样位置,方格的一个端点；
	float curx = -xlen;
	float curz = -zlen;
	float cury = ypos;
	//计算方格位置
	for (int i = 0; i < xsum; i++)
	{
		for (int j = 0; j < zsum; j++)
		{
			bottomBackgroud.push_back(curx);
			bottomBackgroud.push_back(cury);
			bottomBackgroud.push_back(curz);

			bottomBackgroud.push_back(curx + steplen);
			bottomBackgroud.push_back(cury);
			bottomBackgroud.push_back(curz);

			bottomBackgroud.push_back(curx + steplen);
			bottomBackgroud.push_back(cury);
			bottomBackgroud.push_back(curz + steplen);

			bottomBackgroud.push_back(curx);
			bottomBackgroud.push_back(cury);
			bottomBackgroud.push_back(curz + steplen);

			float r = r1;
			float g = g1;
			float b = b1;
			float a = a1;
			if (0 == (i + j) % 2)
			{
				r = r1;
				g = g1;
				b = b1;
				a = a1;
			}
			else
			{
				r = r2;
				g = g2;
				b = b2;
				a = a2;
			}
			for (int k = 0; k < 4; k++)
			{
				colorBackgroud.push_back(r);
				colorBackgroud.push_back(g);
				colorBackgroud.push_back(b);
				colorBackgroud.push_back(a);
			}

			curx += steplen;
		}
		curx = -xlen;
		curz += steplen;
	}
}

//**********************************交互*********************************************
void ModelWidget::change2D(bool c)
{
	pcisph.is2d = c;
	pcisph.restart();
}

void ModelWidget::setModel(MODEL m)
{
	pcisph.restart();
}

void ModelWidget::start()
{
	//启动动画帧
	timer.start(timeStep);
	//FrameThread::getInstance(&pcisph, &frameSec)->start();
}

void ModelWidget::stop()
{
	//停止动画帧
	timer.stop();
	//FrameThread::getInstance()->stop();
}

void ModelWidget::setNumParticle(int num)
{
	pcisph.numParticle = num;
	pcisph.restart();
}

void ModelWidget::showParticle(bool isShow)
{
	isShowParticle = isShow;
	update();
}

void ModelWidget::showSurface(bool isShow)
{
	pcisph.isDisField = isShow;

	pcisph.isSurface = isShow;
	isShowSurface = isShow;

	if (isShow)
	{
		timeFrame();
	}
	update();
}


void ModelWidget::showDisField(bool isShow)
{
	pcisph.isDisField = isShow;
	isShowDisField = isShow;

	if (isShow)
	{
		timeFrame();
	}
	update();
}

void ModelWidget::showHashGrid(bool isShow)
{
	isShowHashGrid = isShow;

	update();
}

void ModelWidget::setOutputFrame(bool isOutput)
{
	isOutputFrame = isOutput;
}

//**********************************输出*********************************************
void ModelWidget::outputFrame()
{
	char idxFrame[8];//当前帧序号的string
	char filePath[32] = "f:\\animation\\frame";//当前帧ply的路径
	itoa(sumFrame, idxFrame, 10);
	strcat(filePath, idxFrame);
	strcat(filePath, ".ply");

	//输出mesh的ply
	outputMeshPly(filePath);
}

void ModelWidget::outputMeshPly(char *filePath)
{
	if (pcisph.mcDistField->mesh.empty()){
		ui->statusBar->showMessage(T_CHAR("未生成流体表面！"));
		return;
	}

	ofstream fout;
	fout.open(filePath, ios::out);//只读的方式打开文件。
	if (!fout)
	{
		mkdir("f:\\animation");
		fout.open(filePath, ios::out);
	}

	//获取当前帧的mesh
	vector<double> &mesh = pcisph.mcDistField->mesh;
	//---------------针对mesh的数据，输出为ply------------
	//--------输出文件头-----------
	fout << "ply" << endl;
	fout << "format ascii 1.0" << endl;
	fout << "comment SPH Surface QQQ" << endl;
	fout << "element vertex " << mesh.size() / 3 << endl;
	fout << "property double x" << endl;
	fout << "property double y" << endl;
	fout << "property double z" << endl;
	fout << "element face " << mesh.size() / 9 << endl;
	fout << "property list uchar uint vertex_indices" << endl;
	fout << "end_header" << endl;

	//-------顶点数据--------
	double r = 1;//缩放
	Quaternion qua;//四元数，用于旋转mesh
	for (int i = 0; i < mesh.size(); i += 3)
	{
		qua.setVertex(mesh[i + 0] * r, mesh[i + 1] * r, mesh[i + 2] * r);
		qua = qua.rotational(3.1415926 / 2, 1.0, 0, 0);//沿x轴旋转90度，即旋转至z轴向上

		fout << qua.x << ' ' << qua.y << ' ' << qua.z << endl;
	}

	//-------面片数据--------
	for (int i = 0; i < mesh.size() / 3; i += 3)
	{
		fout << 3 << ' ' << i + 0 << ' ' << i + 1 << ' ' << i + 2 << endl;
	}

	fout.close();
}

void ModelWidget::outputVertexTxt(char *filePath)
{
	ofstream fout;
	fout.open(filePath, ios::out);//只读的方式打开文件。
	if (!fout)
	{
		mkdir("f:\\particles");
		fout.open(filePath, ios::out);
	}

	vector<Particle> &particles = pcisph.particles;
	for (int i = 0; i < particles.size(); i++)
	{
		fout << particles[i].pos.x << ' ' << particles[i].pos.y << ' ' << particles[i].pos.z << endl;
	}

	fout.close();
}