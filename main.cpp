#include "IntensityProfile.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QApplication a(argc, argv);
	IntensityProfile w;
	w.show();
	return a.exec();
}
