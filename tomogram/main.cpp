#include "tomogram.h"
#include <QtWidgets/QApplication>
#include "view.h"
int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	view w(NULL);
	
	w.loaddata("C:\\kg2\\v.bin");
	w.show();
	return a.exec();
}
