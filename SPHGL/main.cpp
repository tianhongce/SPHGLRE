#include "sphgl.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SPHGL w;
	w.show();
	return a.exec();
}
