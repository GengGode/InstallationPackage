#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_InstallationPackage.h"

class InstallationPackage : public QMainWindow
{
    Q_OBJECT

public:
    InstallationPackage(QWidget *parent = Q_NULLPTR);

private:
    Ui::InstallationPackageClass ui;
};
