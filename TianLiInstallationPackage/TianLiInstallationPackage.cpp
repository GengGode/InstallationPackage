#include "TianLiInstallationPackage.h"
#pragma execution_character_set("utf-8")

TianLiInstallationPackage::TianLiInstallationPackage(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	BOOL bEnable = false;
	DwmIsCompositionEnabled(&bEnable);
	if (bEnable) {
		DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
		DwmSetWindowAttribute((HWND)winId(), DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
		MARGINS margins = { -1 };
		DwmExtendFrameIntoClientArea((HWND)winId(), &margins);
	}

	ui.lineEdit_Path->setText(InstallPath+ InstallDirName);

	ui.ins_label->hide();
	ui.ins_progressBar->hide();
	ui.end_pushButton_Start->hide();
	ui.end_radioButton->hide();

	ui.label_Tag->hide();
	ui.lineEdit_Path->hide();
	ui.pushButton_ChangePath->hide();
	ui.label_Tag_Message->hide();

	connect(ui.pushButton_Title_Mini, SIGNAL(clicked()), this, SLOT(Mini()));
	connect(ui.pushButton_Title_Exit, SIGNAL(clicked()), this, SLOT(Exit()));

	connect(ui.pushButton_Install, SIGNAL(clicked()), this, SLOT(Install()));

	connect(ui.pushButton_Custom, SIGNAL(clicked()), this, SLOT(CustomSetChange()));

	connect(ui.pushButton_ChangePath, SIGNAL(clicked()), this, SLOT(ChangePathBox()));
	connect(ui.pushButton_ShowLisence, SIGNAL(clicked()), this, SLOT(ShowLisence()));
	connect(ui.lineEdit_Path, SIGNAL(textChanged(QString)), this, SLOT(PathChanged(QString)));

	connect(ui.end_pushButton_Start, SIGNAL(clicked()), this, SLOT(Start()));
}

void TianLiInstallationPackage::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton &&
		ui.TitleLabelRect->frameRect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		leftBtnClk = true;
	}
	event->ignore();
}

void TianLiInstallationPackage::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		leftBtnClk = false;
	}
	event->ignore();
}
void TianLiInstallationPackage::mouseMoveEvent(QMouseEvent *event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}

bool TianLiInstallationPackage::isCoundUn7z()
{
	QString exe = QApplication::applicationDirPath() + "/7z.exe";
	if (QDir().exists(exe) == false)
	{
		ShowMessageBox(ShowTextStr4);//未能找到解压模块
		return false;
	}

	SourcePath = QApplication::applicationDirPath() + SourceName;
	if (QDir().exists(SourcePath) == false)
	{
		ShowMessageBox(ShowTextStr5);//资源缺失
		return false;
	}
	return true;
}

void TianLiInstallationPackage::ShowMask()
{
	if (MainMaskLabel == nullptr)
	{
		MainMaskLabel = new QLabel(this);
		MainMaskLabel->setText("");
		MainMaskLabel->setGeometry(QRect(0, 0, this->width(), this->height()));
		MainMaskLabel->setStyleSheet("background-color:rgba(0, 0, 0, 120);");
		MainMaskLabel->show();
	}
	else
	{
		MainMaskLabel->setGeometry(QRect(0, 0, this->width(), this->height()));
		MainMaskLabel->show();
	}
}

void TianLiInstallationPackage::HideMask()
{
	if (MainMaskLabel != nullptr)
	{
		MainMaskLabel->hide();
	}
}

bool TianLiInstallationPackage::CheckInstallPath(QString path)
{
	QString driver = path.section(":/", 0, 0) + ":/";
	LPCWSTR lpcwstrDriver = (LPCWSTR)driver.utf16();
	ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
	if (GetDiskFreeSpaceEx(lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes))
	{
		quint64 size = (quint64)liTotalFreeBytes.QuadPart / 1024 / 1024;
		if (size < 300)
		{
			//kongjianbuzu
			isValidPath = false;
			ShowMessageLabel(ShowTextStr2);
			ui.label_Tag_Message->setText("所需空间：" + TextStr0 + "       可用空间：" + QString::number(size) + "MB");
		}
		else if (size < 1024)
		{
			isValidPath = true;
			HideMessageLabel();
			ui.label_Tag_Message->setText("所需空间：" + TextStr0 + "       可用空间：" + QString::number(size) + "MB");
		}
		else
		{
			isValidPath = true;
			size = size / 1024;
			HideMessageLabel();
			ui.label_Tag_Message->setText("所需空间：" + TextStr0 + "       可用空间：" + QString::number(size) + "GB");
		}
	}
	else
	{
		// 无效路径
		isValidPath = false;
		ShowMessageLabel(ShowTextStr3);
	}
	return isValidPath;
}

void TianLiInstallationPackage::CreateLinke()
{
	QFile::link(InstallPath + InstallDirName + ExportName, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/").append(LinkerName+".lnk"));
}

void TianLiInstallationPackage::Mini()
{
	this->showMinimized();
}

void TianLiInstallationPackage::Exit()
{
	if (isInstallFinish)
	{
		this->close();
	}
	else
	{
		this->ShowMessageBox2(ShowTextStr0);
		connect(WidgetBox2, SIGNAL(isOK(bool)), this, SLOT(isClose(bool)));
	}

}

void TianLiInstallationPackage::isClose(bool isClose)
{
	if (isClose)
	{
		this->close();
	}
}

void TianLiInstallationPackage::ShowMessageLabel(QString text)
{
	if (ErrorMessageLabel == nullptr)
	{
		ErrorMessageLabel = new QLabel(this);
		ErrorMessageLabel->setText(text);
		ErrorMessageLabel->setGeometry(603, 424, 79, 24);
		ErrorMessageLabel->setFont(QFont("微软雅黑"));
		ErrorMessageLabel->setAlignment(Qt::AlignCenter);
		ErrorMessageLabel->setStyleSheet("QLabel{background:rgb(255,0,0);font:\"微软雅黑\";font-size:14px;color:rbg(255,255,255);}");
		ErrorMessageLabel->show();
	}
	else
	{
		ErrorMessageLabel->setText(text);
		ErrorMessageLabel->show();
	}
}

void TianLiInstallationPackage::HideMessageLabel()
{
	if (ErrorMessageLabel != nullptr)
	{
		ErrorMessageLabel->hide();
	}
}

void TianLiInstallationPackage::ShowMessageBox(QString text)
{
	this->ShowMask();
	if (WidgetBox == nullptr)
	{
		WidgetBox = new QtWidgetsMessageBox();
		connect(WidgetBox, SIGNAL(SendCloseSelfSignalToMainWidgets()), this, SLOT(ReceiveCloseSelfSignalFromMainWidgets()));
		WidgetBox->setText(text);

		WidgetBox->setWindowModality(Qt::ApplicationModal);
		WidgetBox->move(this->x() + (int)((this->width() - WidgetBox->width()) / 2), this->y() + (int)((this->height() - WidgetBox->height()) / 2));
		WidgetBox->show();
	}
	else
	{
		WidgetBox->setText(text);
		WidgetBox->move(this->x() + (int)((this->width() - WidgetBox->width()) / 2), this->y() + (int)((this->height() - WidgetBox->height()) / 2));
		WidgetBox->show();
	}
}

void TianLiInstallationPackage::ShowMessageBox2(QString text)
{
	this->ShowMask();
	if (WidgetBox2 == nullptr)
	{
		WidgetBox2 = new QtWidgetsMessageBox2();
		connect(WidgetBox2, SIGNAL(SendCloseSelfSignalToMainWidgets(bool)), this, SLOT(ReceiveCloseSelfSignalFromMainWidgets(bool)));
		WidgetBox2->setText(text);

		WidgetBox2->setWindowModality(Qt::ApplicationModal);
		WidgetBox2->move(this->x() + (int)((this->width() - WidgetBox2->width()) / 2), this->y() + (int)((this->height() - WidgetBox2->height()) / 2));
		WidgetBox2->show();
	}
	else
	{
		WidgetBox2->setText(text);
		WidgetBox2->move(this->x() + (int)((this->width() - WidgetBox2->width()) / 2), this->y() + (int)((this->height() - WidgetBox2->height()) / 2));
		WidgetBox2->show();
	}
}

void TianLiInstallationPackage::Install()
{
	if (ui.radioButton_IsAgree->isChecked() == true)
	{
		CheckInstallPath(ui.lineEdit_Path->text());
		if (!isValidPath)
		{
			return;
		}
		if (isCoundUn7z())
		{
			//切换UI
			ui.label_Tag->hide();
			ui.lineEdit_Path->hide();
			ui.pushButton_ChangePath->hide();
			ui.label_Tag_Message->hide();

			this->setFixedHeight(400);

			isDownBar = false;

			ui.pushButton_Install->hide();
			ui.radioButton_IsAgree->hide();
			ui.pushButton_ShowLisence->hide();
			ui.pushButton_Custom->hide();

			ui.label_Bg_TopRect->setStyleSheet("QLabel{background-image:url(:/TianLiInstallationPackage/Resource/bg2.png)}");
			//显示安装中UI
			ui.ins_label->show();
			ui.ins_progressBar->show();
			ui.ins_progressBar->setValue(0);

			//进度条

			unZip_7z = new Process7zWorker(NULL);
			unZip_7z->setZipFilePath(SourcePath);
			unZip_7z->setUnZipFilePath(InstallPath + InstallDirName);

			connect(unZip_7z, &Process7zWorker::unZipError, this, &TianLiInstallationPackage::unZip_Error);
			connect(unZip_7z, &Process7zWorker::unZipProcess, this, &TianLiInstallationPackage::unZip_Process);
			connect(unZip_7z, &Process7zWorker::unZipFinished, this, &TianLiInstallationPackage::unZip_finished);

			connect(this, &TianLiInstallationPackage::unZip,unZip_7z, &Process7zWorker::unzip);
			
			emit unZip();
		}
	}
	else
	{
		ShowMessageBox(ShowTextStr1);//需要同意许可协议
	}
}

void TianLiInstallationPackage::ShowLisence()
{
	QDesktopServices::openUrl(QUrl(QLatin1String("https://yuanshen.weixitianlizhi.ren/docs/agreement.html")));
}

void TianLiInstallationPackage::CustomSetChange()
{
	if (isDownBar)
	{
		ui.label_Tag->hide();
		ui.lineEdit_Path->hide();
		ui.pushButton_ChangePath->hide();
		ui.label_Tag_Message->hide();

		QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
		animation->setDuration(100);
		animation->setStartValue(this->geometry());
		animation->setEndValue(QRect(this->x(), this->y(), 710, 400));
		animation->setEasingCurve(QEasingCurve::InOutQuad);
		animation->start(QAbstractAnimation::DeleteWhenStopped);

		isDownBar = false;
	}
	else
	{
		ui.label_Tag->show();
		ui.lineEdit_Path->show();
		ui.pushButton_ChangePath->show();
		ui.label_Tag_Message->show();

		QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
		animation->setDuration(100);
		animation->setStartValue(this->geometry());
		animation->setEndValue(QRect(this->x(), this->y(), 710, 530));
		animation->setEasingCurve(QEasingCurve::InOutQuad);
		animation->start(QAbstractAnimation::DeleteWhenStopped);

		isDownBar = true;
	}
}

void TianLiInstallationPackage::ChangePathBox()
{
	ShowMask();
	QString Path = QFileDialog::getExistingDirectory(this, "选择安装目录", InstallPath, QFileDialog::ShowDirsOnly);
	if (!Path.isEmpty())
	{
		InstallPath = Path;
		ui.lineEdit_Path->setText(Path+ InstallDirName);
	}
	HideMask();
}

void TianLiInstallationPackage::PathChanged(QString path)
{
	path = path.section("/", 0, -1);
	CheckInstallPath(path);
}

void TianLiInstallationPackage::Start()
{
	QString command = InstallPath + InstallDirName + ExportName;
	TCHAR szCmdLine[1024] = {};

	command.toWCharArray(szCmdLine);
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi;

	bool res = CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	if (res != true)
	{
		int k = GetLastError();
	}
	this->close();
}

void TianLiInstallationPackage::unZip_Error(int errorCode)
{
	switch (errorCode)
	{
	case 0:
	{
		break;
	}
	case 1:
	{
		break;
	}
	case 2:
	{
		break;
	}
	}
}

void TianLiInstallationPackage::unZip_Process(int value)
{
	QPropertyAnimation *animation = new QPropertyAnimation(ui.ins_progressBar, "value");
	animation->setDuration(300);
	animation->setStartValue(ui.ins_progressBar->value());
	animation->setEndValue(value);
	animation->setEasingCurve(QEasingCurve::InOutQuad);
	//animation->setEasingCurve(QEasingCurve::OutBack);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void TianLiInstallationPackage::unZip_finished()
{
	CreateLinke();

	isInstallFinish = true;
	//隐藏安装中UI
	ui.ins_label->hide();
	ui.ins_progressBar->hide();

	//显示安装结束UI
	ui.end_pushButton_Start->show();
	ui.end_radioButton->show();
	ui.label_Bg_TopRect->setStyleSheet("QLabel{background-image:url(:/TianLiInstallationPackage/Resource/bg.png)}");

}

void TianLiInstallationPackage::ReceiveCloseSelfSignalFromMainWidgets()
{
	this->HideMask();
}

void TianLiInstallationPackage::ReceiveCloseSelfSignalFromMainWidgets(bool isOK)
{

	this->HideMask();
}
