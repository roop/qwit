#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MainWindow::getInstance()->show();
//	MainWindow *mainWindow = new MainWindow();
//	mainWindow->show();
	return app.exec();
}
