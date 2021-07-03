#pragma once
#pragma execution_character_set("utf-8")
#include <dwmapi.h>
#include <atlconv.h>
#include <Windows.h>
#include <QDir>
#include <QLabel>
#include <QProcess>
#include <QFileDialog>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QtWidgets/QMainWindow>
#include "ui_TianLiInstallationPackage.h"
#include "QtWidgetsMessageBox.h"
#include "QtWidgetsMessageBox2.h"

class TianLiInstallationPackage : public QMainWindow
{
	Q_OBJECT

public:
	TianLiInstallationPackage(QWidget *parent = Q_NULLPTR);

private:
	Ui::TianLiInstallationPackageClass ui;

private:
	bool isValidPath = false;
	bool isInstallFinish = false;
	bool isDownBar = false;
	QLabel *MainMaskLabel = nullptr;

	QLabel *ErrorMessageLabel = nullptr;

	QtWidgetsMessageBox *WidgetBox = nullptr;
	QtWidgetsMessageBox2 *WidgetBox2 = nullptr;
private:
	QString SourcePath;
	QString SourceName= "/source.7z";
	QString LinkerName = "����";
	QString ExportName = "/����ϵͳ.exe";
	QString InstallPath = "C:/Program Files";
	QString InstallDirName = "/����";
	QProcess *unZip_7z = nullptr;

private:
	QPoint m_Press;
	QPoint m_Move;
	bool leftBtnClk = false;

private:
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
	bool isCoundUn7z();
	bool un7z();
	void ShowMask();
	void HideMask();

	bool CheckInstallPath(QString path);

	void CreateLinke();
private slots:
	void Mini();
	void Exit();

	void isClose(bool isClose);

	void ShowMessageLabel(QString text);
	void HideMessageLabel();
	void ShowMessageBox(QString text);
	void ShowMessageBox2(QString text);

	void Install();
	void ShowLisence();
	void CustomSetChange();
	void ChangePathBox();
	void PathChanged(QString path);
	void Start();

	void unZip_ReadStandardOutput();
	void unZip_finished(int exitCode);

	void ReceiveCloseSelfSignalFromMainWidgets();
	void ReceiveCloseSelfSignalFromMainWidgets(bool isOK);
};