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

#define T_CHAR(str)  QString::fromLocal8Bit(str)	//转换字符串

class ModelWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	ModelWidget(Ui::SPHGLClass *ui = NULL, int id = 0, QWidget* parent = 0);
	~ModelWidget();

private:
	GLint iWinWidth;			//窗口尺寸
	GLint iWinHeight;
	Ui::SPHGLClass *ui;
	void initWidget();

protected:
	/* 响应函数，openg */
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);

	private slots:
	//动画帧
	void timeFrame();
	void updateFrame();
private:
	/* SPH	*/
	//SPH sph;
	SPH pcisph;
	QTimer updateTimer;//计数器
	QTimer timer;//计数器
	QTime time;	//时钟
	int frame;	//当前帧数,用于记录每秒的帧率
	int sumFrame;//总帧
	int frameSec;
	double ratio;//缩放比例
	/* 环境变量 */
	int mouseX, mouseY;			//当前鼠标位置
	bool isMouseDown;			//鼠标是否按下
	float rotX = 0, rotY = 0;	//旋转角度
	double zoom;
	double posX, posY;			//视点位置
	int id;
	GLfloat lightDirection[3];		//光照方向
	GLfloat lightPosition[4];		//光源位置,xyz,dis
	/* 背景 */
	vector<float> bottomBackgroud;
	vector<float> colorBackgroud;

private:
	/* OpenGL */
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void setLight();

private:
	/* 绘制模型 */
	//绘制背景，地板
	void drawBoundary(GLenum mode = GL_RENDER);
	//绘制网格
	void drawDistFieldGrid();
	//绘制粒子
	void drawParticle();
	//绘制背景
	void drawBackground();
	//底边背景
	void setBackground(float xr = 5, float zr = 5, float yp = -0.5, float step = 0.2);
	//绘制流体表面
	void drawFluid();
	void drawHashGrid();

public:
	bool isLock;//锁定视图
	bool isBackground;//显示背景
	float ballRadius; //粒子半径
	float timeStep;	  //每帧时间步长
	bool isShowParticle;
	bool isShowDisField;
	bool isShowSurface;
	bool isShowHashGrid;
	bool isOutputFrame;

public:
	//切换2D/3D
	void change2D(bool c);
	//设置模式
	void setModel(MODEL m);
	//开始
	void start();
	//停止
	void stop();
	//设置粒子数
	void setNumParticle(int num);
	void showSurface(bool isShow);
	void showParticle(bool isShow);
	void showDisField(bool isShow);
	void showHashGrid(bool isShow);
	//设置是否输出帧
	void setOutputFrame(bool isOutput);
	//根据当前帧状态，输出当前帧的mesh
	void outputFrame();
	//将点集输出到filePath
	void outputVertexTxt(char *filePath);
	//将mesh以ply格式输出到filePath
	void outputMeshPly(char *filePath);
};



#endif 
