#include "InstallationPackage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InstallationPackage w;
    w.show();
    return a.exec();
}
