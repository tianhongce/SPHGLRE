#pragma once
#include <qslider.h>
#include <qlayout.h>
#include "InputDialog.h"

class FrameInputDialog :
	public InputDialog
{
public:
	FrameInputDialog(int curpos, QWidget* parent = 0);
	~FrameInputDialog();

private:
	QSlider *slider = NULL;
	int curValue = 0;
	void initSlider();


	private slots:
	void onChangeFrame(int& step);
};

