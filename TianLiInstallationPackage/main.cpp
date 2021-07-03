#include "TianLiInstallationPackage.h"
#include <QtWidgets/QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	int iWidth = 710;
	int iHeight = 400;

	QDesktopWidget *desktop = QApplication::desktop();
	QRect desktopRect = desktop->availableGeometry();

    TianLiInstallationPackage w;
	w.setGeometry((desktopRect.width() - iWidth) / 2,
		(desktopRect.height() - iHeight) / 2,
		iWidth, iHeight);


    w.show();
    return a.exec();
}
