#include "ServerTcp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ServerTcp w;
	w.show();
	return a.exec();
}
