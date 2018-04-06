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

//**********************************��ʼ��*********************************************
void ModelWidget::initWidget()
{
	isMouseDown = false;			//����Ƿ���
	frame = 0;
	sumFrame = 0;
	frameSec = 0;
	rotX = 0;//��ʼ����ת�Ƕ�
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

	//���ò���
	//ratio = 3.6 / pcisph.wBottle;//���ű���
	ratio = 1;
	ballRadius = 0.028;	//���Ӱ뾶
	timeStep = 10;		//֡���,ms

	lightPosition[0] = 0.0f;	//��Դλ��,xyz,dis
	lightPosition[1] = 1.5f;
	lightPosition[2] = 0.0f;
	lightPosition[3] = 0.0f;
	lightDirection[0] = 0.0f;	//���շ���
	lightDirection[0] = -1.0f;
	lightDirection[0] = 0.0f;

	//���ò���ȡ����
	double bgy = pcisph.yBottle * ratio - 0.1;//����y����
	setBackground(30, 30, bgy, 1.0);
	setFocusPolicy(Qt::StrongFocus);

	/*****��ʼģ��*****/
	//timer.start(timeStep);
	timer.stop();

	//��ʼʱ�ӣ��Լ�����һ֡
	time.start();
	//timeFrame();

	QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timeFrame()));

}

void ModelWidget::initializeGL()
{
	glClearColor(0, 0, 0, 1);//����ɫ

	glEnable(GL_DEPTH_TEST);//��Ȳ��ԣ����ֵ��Ч
	glEnable(GL_BLEND);		//��ɫ���
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);//������ɫ׷��
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);		//������ȷ�ΧΪ���

	//���ù��ղ���
	setLight();
}

void ModelWidget::setLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//GLfloat lightPosition[] = { 0.0, 1.2, -1.0, 1.0 };		//��Դλ��,xyz,dis
	GLfloat lightAmbient[] = { 0.6, 0.6, 0.6, 1.0 };		//ȫ�ֹ�����,��ǿ��
	GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat lightDirection[] = { 0.0f, -1.0f, 0.0f };	//���շ���
	GLfloat lightExponent[] = { 1.0f };					//�۹�̶�
	GLfloat lightCutoff[] = { 60.0f };					//��Դ�н�

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//ָ����Ҫ��ɫ׷�ٵĲ�������
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//ָ����0�Ź�Դ��λ�� 
	//����
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse); //�������
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);//���淴���
	//�۹�
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

	//ͶӰ��ʽ��͸��ͶӰ
	gluPerspective(60, (GLfloat)iWinWidth / (GLfloat)iWinHeight, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

//**********************************�¼�*********************************************
void ModelWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (isMouseDown)
	{
		int deltX, deltY;
		// ��������ƶ����룬��תͼ��
		deltX = e->x() - mouseX;
		deltY = e->y() - mouseY;

		if (false == isLock)
		{
			// ��ת��  
			rotX += deltX / 2;
			rotY += deltY / 2;
			// ��ת�ǲ�����360�� 
			rotX = fmodf(rotX, 360.0);
			rotY = fmodf(rotY, 360.0);
		}
		else
		{
			pcisph.xBottle += double(deltX) / this->width() / ratio * 4;
			pcisph.yBottle -= double(deltY) / this->height() / ratio * 4;
		}

		//���µ�ǰ���λ�ã�ʹͼ��ʵʱ��ת
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
	int numDegrees = e->delta() / 8;//�����ĽǶȣ�*8�����������ľ���
	double numSteps = numDegrees / 120.0;//�����Ĳ�����*15�����������ĽǶ�
	zoom += numSteps / 2;
	e->accept();      //���ո��¼�
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

	pcisph.frame();//һ֡
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
	//qDebug() << T_CHAR("֡�ڼ����") << e - s;
}

void ModelWidget::updateFrame()
{
	static int us = 0, ue = 0;
	us = clock();

	this->update();

	ui->statusBar->showMessage(QString("FPS : %1 / s\t  Frame: %2 ms\t  sum: %3").arg(1)
		.arg(frameSec).arg(pcisph.particles.size()));

	ue = clock();
	//qDebug() << T_CHAR("�ػ�����") << ue - us;
}

//**********************************����*********************************************
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

	//�ƶ���Դͬ���ƶ�
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//ָ����0�Ź�Դ��λ�� 
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

	//����ԭ��
	GLUquadricObj *quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_SMOOTH);
	glColor4f(0.5, 0.5, 0.5, 0.5);
	gluSphere(quadricObj, 0.008, 5, 5);

	if (true == isBackground){
		drawBackground();//���Ʊ���
	}

	if (isShowDisField == true){
		drawDistFieldGrid();//���볡
	}

	if (isShowParticle){
		drawParticle();//��������
	}

	if (isShowSurface){
		drawFluid();//Һ��
	}

	if (isShowHashGrid){
		drawHashGrid();
	}

	drawBoundary();//���Ʊ߽�

	glPopMatrix();

	ed = clock();
	//qDebug() << T_CHAR("�����ڼ����") << ed - sd;
}

void ModelWidget::drawHashGrid()
{
	//��ɫ
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
	//��ɫ
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
				//Һ����
				glColor4f(0.9, 0.1, 0.1, 1.0);
			}
			else
			{
				//Һ����
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

	//Һ����ɫ
	glColor4f(0.4, 0.5, 0.7, 1);
	//glColor4f(0, 0, 0.8, 1);

	glScaled(ratio, ratio, ratio);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	//���嶥������  
	glVertexPointer(3, GL_DOUBLE, 0, pcisph.mcDistField->mesh.data());
	glNormalPointer(GL_DOUBLE, 0, pcisph.mcDistField->meshVector.data());
	//glColorPointer(4, GL_FLOAT, 0, colorBackgroud.data());

	//����
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
	//�߽�λ�úʹ�С
	double xb = pcisph.xBottle - ballRadius;
	double yb = pcisph.yBottle - ballRadius;
	double zb = pcisph.zBottle - ballRadius;
	double wb = pcisph.wBottle + ballRadius;
	double hb = pcisph.hBottle + ballRadius;
	double lb = pcisph.lBottle + ballRadius;

	//���Ʊ߽�
	glLineWidth(2);
	glColor4f(0.7, 0.7, 0.9, 0.5);//�߽���ɫ
	//����
	glBegin(GL_LINES);
	//����
	glVertex3d(xb, yb + hb, zb);	//����
	glVertex3d(xb, yb, zb);
	glVertex3d(xb, yb, zb);			//����
	glVertex3d(xb + wb, yb, zb);
	glVertex3d(xb + wb, yb, zb);	//����
	glVertex3d(xb + wb, yb + hb, zb);
	glVertex3d(xb + wb, yb + hb, zb);//����
	glVertex3d(xb, yb + hb, zb);
	//ǰ��
	glVertex3d(xb, yb + hb, zb + lb);
	glVertex3d(xb, yb, zb + lb);
	glVertex3d(xb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb + lb);
	glVertex3d(xb + wb, yb + hb, zb + lb);
	glVertex3d(xb + wb, yb + hb, zb + lb);
	glVertex3d(xb, yb + hb, zb + lb);
	//���
	glVertex3d(xb, yb + hb, zb);	//����
	glVertex3d(xb, yb + hb, zb + lb);
	glVertex3d(xb, yb, zb);			//����
	glVertex3d(xb, yb, zb + lb);
	//�Ҳ�
	glVertex3d(xb + wb, yb + hb, zb);//����
	glVertex3d(xb + wb, yb + hb, zb + lb);
	glVertex3d(xb + wb, yb, zb);	//����
	glVertex3d(xb + wb, yb, zb + lb);
	glEnd();

	//������
	glColor4f(0.9, 0.9, 1, 0.10);//�߽���ɫ
	glBegin(GL_QUADS);
	//����
	glVertex3d(xb, yb + hb, zb);
	glVertex3d(xb, yb, zb);
	glVertex3d(xb + wb, yb, zb);
	glVertex3d(xb + wb, yb + hb, zb);
	//�²���
	glVertex3d(xb, yb, zb);			//������
	glVertex3d(xb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb);	//������
	//����
	glVertex3d(xb, yb + hb, zb);	//������
	glVertex3d(xb, yb + hb, zb + lb);//ǰ����
	glVertex3d(xb, yb, zb + lb);
	glVertex3d(xb, yb, zb);			//������
	//�Ҳ���
	glVertex3d(xb + wb, yb + hb, zb);//������
	glVertex3d(xb + wb, yb + hb, zb + lb);
	glVertex3d(xb + wb, yb, zb + lb);
	glVertex3d(xb + wb, yb, zb);	//������
	glEnd();

	glPopMatrix();
}

//��������
void ModelWidget::drawParticle()
{
	GLUquadricObj *quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_SMOOTH);

	//������ɫ
	glColor4f(0.5, 0.55, 0.7, 0.9);

	glPushMatrix();

	//����ÿ������
	for (int i = 0; i < pcisph.particles.size(); i++)
	{
		Particle &p = pcisph.particles[i];

		//qDebug() << T_CHAR("�ܶȣ�") << p.density;
		//if (p.density < 1.5)
		//{
		//	//�ܶ�С����
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

	//���嶥������  
	glVertexPointer(3, GL_FLOAT, 0, bottomBackgroud.data());
	glColorPointer(4, GL_FLOAT, 0, colorBackgroud.data());

	//����
	glDrawArrays(GL_QUADS, 0, bottomBackgroud.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}

void ModelWidget::setBackground(float xr, float zr, float yp, float step)
{
	bottomBackgroud.clear();
	colorBackgroud.clear();

	//�ܳ���,-xlen:xlen
	float xlen = xr;
	float zlen = zr;
	float ypos = yp;//y����

	//���񳤶�
	float steplen = step;

	//��������
	int xsum = xlen * 2 / steplen;
	int zsum = zlen * 2 / steplen;

	//������͸����
	float alpha = 0.5;

	//����1����ɫ
	float r1 = 0.7;
	float g1 = 0.7;
	float b1 = 0.7;
	float a1 = alpha;

	//����2����ɫ
	float r2 = 1;
	float g2 = 1;
	float b2 = 1;
	float a2 = alpha;

	//��ǰ����λ��,�����һ���˵㣻
	float curx = -xlen;
	float curz = -zlen;
	float cury = ypos;
	//���㷽��λ��
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

//**********************************����*********************************************
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
	//��������֡
	timer.start(timeStep);
	//FrameThread::getInstance(&pcisph, &frameSec)->start();
}

void ModelWidget::stop()
{
	//ֹͣ����֡
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

//**********************************���*********************************************
void ModelWidget::outputFrame()
{
	char idxFrame[8];//��ǰ֡��ŵ�string
	char filePath[32] = "f:\\animation\\frame";//��ǰ֡ply��·��
	itoa(sumFrame, idxFrame, 10);
	strcat(filePath, idxFrame);
	strcat(filePath, ".ply");

	//���mesh��ply
	outputMeshPly(filePath);
}

void ModelWidget::outputMeshPly(char *filePath)
{
	if (pcisph.mcDistField->mesh.empty()){
		ui->statusBar->showMessage(T_CHAR("δ����������棡"));
		return;
	}

	ofstream fout;
	fout.open(filePath, ios::out);//ֻ���ķ�ʽ���ļ���
	if (!fout)
	{
		mkdir("f:\\animation");
		fout.open(filePath, ios::out);
	}

	//��ȡ��ǰ֡��mesh
	vector<double> &mesh = pcisph.mcDistField->mesh;
	//---------------���mesh�����ݣ����Ϊply------------
	//--------����ļ�ͷ-----------
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

	//-------��������--------
	double r = 1;//����
	Quaternion qua;//��Ԫ����������תmesh
	for (int i = 0; i < mesh.size(); i += 3)
	{
		qua.setVertex(mesh[i + 0] * r, mesh[i + 1] * r, mesh[i + 2] * r);
		qua = qua.rotational(3.1415926 / 2, 1.0, 0, 0);//��x����ת90�ȣ�����ת��z������

		fout << qua.x << ' ' << qua.y << ' ' << qua.z << endl;
	}

	//-------��Ƭ����--------
	for (int i = 0; i < mesh.size() / 3; i += 3)
	{
		fout << 3 << ' ' << i + 0 << ' ' << i + 1 << ' ' << i + 2 << endl;
	}

	fout.close();
}

void ModelWidget::outputVertexTxt(char *filePath)
{
	ofstream fout;
	fout.open(filePath, ios::out);//ֻ���ķ�ʽ���ļ���
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