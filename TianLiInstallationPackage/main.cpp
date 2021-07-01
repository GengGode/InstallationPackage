#include "TianLiInstallationPackage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TianLiInstallationPackage w;
    w.show();
    return a.exec();
}
