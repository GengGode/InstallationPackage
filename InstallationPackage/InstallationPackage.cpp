#include "InstallationPackage.h"

InstallationPackage::InstallationPackage(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	//this->setFixedSize(710, 400);
	this->setFixedHeight(400);
	ui.label_Tag->hide();
	ui.lineEdit_Path->hide();
	ui.pushButton_ChangePath->hide();
	ui.label_Tag_Message->hide();

	connect(ui.pushButton_Title_Mini, SIGNAL(clicked()), this, SLOT(Mini()));
	connect(ui.pushButton_Title_Exit, SIGNAL(clicked()), this, SLOT(Exit()));

	//
	connect(ui.pushButton_Install, SIGNAL(clicked()), this, SLOT(Install()));

	connect(ui.pushButton_Custom, SIGNAL(clicked()), this, SLOT(CustomSetChange()));
}

void InstallationPackage::Mini()
{
	this->showMinimized();
}

bool InstallationPackage::un7z()
{
	QProcess    m_7z;
	QString exe = QApplication::applicationDirPath() + "/7z.exe";
	if (QDir().exists(exe) == false)
	{
		//未能找到解压模块
		return false;
	}

	SourcePath= QApplication::applicationDirPath() + "/source.7z";
	if (QDir().exists(SourcePath) == false)
	{
		//资源缺失
		return false;
	}

	QStringList args;

	args << "x" << SourcePath << "-o" + InstallPath << "-aoa";
	//x:eXtract with full paths用文件的完整路径解压至当前目录或指定目录
	//-o (Set Output Directory)
	//-aoa 直接覆盖现有文件，而没有任何提示
	m_7z.start(exe, args);
	m_7z.waitForStarted();
	m_7z.waitForFinished();
	
	if (m_7z.state() != QProcess::NotRunning) {
		return false;
	}

	if (m_7z.exitStatus() == QProcess::NormalExit) {
		return true;
	}
}

void InstallationPackage::Exit()
{
	this->close();
}

void InstallationPackage::Install()
{
	if (ui.radioButton_IsAgree->isChecked() == true)
	{
		//进度条
		un7z();
		//确定退出
	}
	else
	{
		//需要同意许可协议
	}

}

void InstallationPackage::CustomSetChange()
{
	if (isDownBar)
	{
		ui.label_Tag->hide();
		ui.lineEdit_Path->hide();
		ui.pushButton_ChangePath->hide();
		ui.label_Tag_Message->hide();

		this->setFixedHeight(400);

		isDownBar = false;
	}
	else
	{
		ui.label_Tag->show();
		ui.lineEdit_Path->show();
		ui.pushButton_ChangePath->show();
		ui.label_Tag_Message->show();

		this->setFixedHeight(530);

		isDownBar = true;
	}

	//QPropertyAnimation * pWidgetProcessUp = new QPropertyAnimation(this, "size");
	//// QRect（x，y，width，hight）
	//pWidgetProcessUp->setStartValue(size());  //初始状态
	//pWidgetProcessUp->setEndValue(QSize( width(), 530));  //结束状态
	//pWidgetProcessUp->setDuration(300);                              //设置动画执行时间，单位毫秒
	//pWidgetProcessUp->start();
}
