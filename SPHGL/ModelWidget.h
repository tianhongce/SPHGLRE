#ifndef MODEL_WIDGET
#define MODEL_WIDGET

#include <QtOpenGL/QtOpenGL>
#include <QWheelEvent>
#include <qtimer.h>
#include <fstream>
#include <direct.h> 
#include <cstdlib>
#include "SPH.h"
#include "ui_sphgl.h"
#include "FrameInputDialog.h"
#include "FrameThread.h"
#include "tool\Quaternion.h"

using namespace std;

#define T_CHAR(str)  QString::fromLocal8Bit(str)	//ת���ַ���

class ModelWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	ModelWidget(Ui::SPHGLClass *ui = NULL, int id = 0, QWidget* parent = 0);
	~ModelWidget();

private:
	GLint iWinWidth;			//���ڳߴ�
	GLint iWinHeight;
	Ui::SPHGLClass *ui;
	void initWidget();

protected:
	/* ��Ӧ������openg */
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);

	private slots:
	//����֡
	void timeFrame();
	void updateFrame();
private:
	/* SPH	*/
	//SPH sph;
	SPH pcisph;
	QTimer updateTimer;//������
	QTimer timer;//������
	QTime time;	//ʱ��
	int frame;	//��ǰ֡��,���ڼ�¼ÿ���֡��
	int sumFrame;//��֡
	int frameSec;
	double ratio;//���ű���
	/* �������� */
	int mouseX, mouseY;			//��ǰ���λ��
	bool isMouseDown;			//����Ƿ���
	float rotX = 0, rotY = 0;	//��ת�Ƕ�
	double zoom;
	double posX, posY;			//�ӵ�λ��
	int id;
	GLfloat lightDirection[3];		//���շ���
	GLfloat lightPosition[4];		//��Դλ��,xyz,dis
	/* ���� */
	vector<float> bottomBackgroud;
	vector<float> colorBackgroud;

private:
	/* OpenGL */
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void setLight();

private:
	/* ����ģ�� */
	//���Ʊ������ذ�
	void drawBoundary(GLenum mode = GL_RENDER);
	//��������
	void drawDistFieldGrid();
	//��������
	void drawParticle();
	//���Ʊ���
	void drawBackground();
	//�ױ߱���
	void setBackground(float xr = 5, float zr = 5, float yp = -0.5, float step = 0.2);
	//�����������
	void drawFluid();
	void drawHashGrid();

public:
	bool isLock;//������ͼ
	bool isBackground;//��ʾ����
	float ballRadius; //���Ӱ뾶
	float timeStep;	  //ÿ֡ʱ�䲽��
	bool isShowParticle;
	bool isShowDisField;
	bool isShowSurface;
	bool isShowHashGrid;
	bool isOutputFrame;

public:
	//�л�2D/3D
	void change2D(bool c);
	//����ģʽ
	void setModel(MODEL m);
	//��ʼ
	void start();
	//ֹͣ
	void stop();
	//����������
	void setNumParticle(int num);
	void showSurface(bool isShow);
	void showParticle(bool isShow);
	void showDisField(bool isShow);
	void showHashGrid(bool isShow);
	//�����Ƿ����֡
	void setOutputFrame(bool isOutput);
	//���ݵ�ǰ֡״̬�������ǰ֡��mesh
	void outputFrame();
	//���㼯�����filePath
	void outputVertexTxt(char *filePath);
	//��mesh��ply��ʽ�����filePath
	void outputMeshPly(char *filePath);
};



#endif 
