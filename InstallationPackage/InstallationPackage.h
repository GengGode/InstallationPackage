#pragma once

#include <QDir>
#include <QProcess>
#include <QPropertyAnimation>
#include <QtWidgets/QMainWindow>
#include "ui_InstallationPackage.h"

class InstallationPackage : public QMainWindow
{
    Q_OBJECT

public:
    InstallationPackage(QWidget *parent = Q_NULLPTR);

private:
    Ui::InstallationPackageClass ui;


private:
	bool isDownBar = false;

private:
	QString SourcePath;
	QString InstallPath="E:/tmp";


private:
	bool un7z();
private slots:
	void Mini();
	void Exit();


	void Install();
	void CustomSetChange();

};
