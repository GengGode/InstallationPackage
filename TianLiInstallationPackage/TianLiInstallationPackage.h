#pragma once
#pragma execution_character_set("utf-8")
#include <dwmapi.h>
#include <atlconv.h>
#include <Windows.h>
#include <QDir>
#include <QLabel>
#include <QThread>
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
#include "Process7zWorker.h"

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
	QString SourceName = "/Source.Zip";
	QString LinkerName = "空荧酒馆原神地图";
	QString ExportName = "/map.exe";
#ifdef _DEBUG
	QString InstallPath = "C:/Program Files";// "E:/tmp/launcher";//"C:/Program Files";
#else
	QString InstallPath = "C:/Program Files";
#endif
	QString InstallDirName = "/空荧酒馆原神地图";
	QString Url_Agreement = "https://yuanshen.weixitianlizhi.ren/";

	Process7zWorker *unZip_7z = nullptr;
	QThread *unzipProcess = nullptr;
private:
	int Size = 254;
	QString TextStr0 = QString::number(Size) + "MB";
	QString ShowTextStr0 = "是否退出安装";
	QString ShowTextStr1 = "需要同意许可协议";
	QString ShowTextStr2 = "空间不足";
	QString ShowTextStr3 = "无效路径";
	QString ShowTextStr4 = "未能找到解压模块";
	QString ShowTextStr5 = "资源缺失";
	QString ShowTextStr6 = "无法开始解压";
	QString ShowTextStr7 = "解压失败";

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

	void unZip_Error(int errorCode);
	void unZip_Process(int value);
	void unZip_finished();

	void ReceiveCloseSelfSignalFromMainWidgets();
	void ReceiveCloseSelfSignalFromMainWidgets(bool isOK);


signals:
	void unZip(); 
};
