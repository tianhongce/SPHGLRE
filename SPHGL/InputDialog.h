#pragma once
#include "qdialog.h"

class InputDialog :
	public QDialog
{
	Q_OBJECT
public:
	InputDialog(QWidget* parent = 0);
	~InputDialog();
};

