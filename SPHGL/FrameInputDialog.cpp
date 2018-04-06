#include "FrameInputDialog.h"


FrameInputDialog::FrameInputDialog(int curpos, QWidget* parent) : InputDialog(parent)
{
	curValue = curpos;
	this->setGeometry(500, 500, 200,30);
	initSlider();
	this->exec();
}

void FrameInputDialog::initSlider()
{
	if (slider == NULL)
		slider = new QSlider(Qt::Horizontal);

	slider->setMaximum(1000);
	slider->setTickInterval(10);
	slider->setSliderPosition(curValue);

	QVBoxLayout vlayout;
	vlayout.addWidget(slider);
	this->setLayout(&vlayout);

	QObject::connect(slider, SIGNAL(sliderMoved()), this, SLOT(onChangeFrame()));
}

FrameInputDialog::~FrameInputDialog()
{
	if (slider != NULL)
		delete slider;
}

void FrameInputDialog::onChangeFrame(int& step)
{
	step = slider->value();
}