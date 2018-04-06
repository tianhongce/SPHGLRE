#ifndef SPHGL_H
#define SPHGL_H

#include <QtWidgets/QMainWindow>
#include "ui_sphgl.h"
#include "ModelWidget.h"

class SPHGL : public QMainWindow
{
	Q_OBJECT

public:
	SPHGL(QWidget *parent = 0);
	~SPHGL();

private:
	Ui::SPHGLClass ui;

private:
	void init();

private slots:
	void onLockView();
	void onSetBackground();
	void onChange2D();
	void onBamBreak();
	void onWaterPipe();
	void onStart();
	void onStop();
	void onSetNumParticles();
	void onShowSurface();
	void onShowParticle();
	void onShowDisField();
	void onShowHashGrid();
	void onOutputFrame();

private:
	ModelWidget* viewWidget;
};

#endif // SPHGL_H
