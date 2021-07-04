#include "QtWidgetsMessageBox2.h"
#pragma execution_character_set("utf-8")

QtWidgetsMessageBox2::QtWidgetsMessageBox2(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	connect(ui.pushButton_OK, SIGNAL(clicked()), this, SLOT(OK()));
	connect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(Cancel()));
}

QtWidgetsMessageBox2::~QtWidgetsMessageBox2()
{
}

void QtWidgetsMessageBox2::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton &&
		ui.TitleLabelRect->frameRect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		leftBtnClk = true;
	}
	event->ignore();
}

void QtWidgetsMessageBox2::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		leftBtnClk = false;
	}
	event->ignore();
}
void QtWidgetsMessageBox2::mouseMoveEvent(QMouseEvent *event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}

void QtWidgetsMessageBox2::setText(QString messgase)
{
	ui.label_Message->setText(messgase);
}

void QtWidgetsMessageBox2::OK()
{
	emit this->isOK(true);
	emit this->SendCloseSelfSignalToMainWidgets(true);
	this->close();
}

void QtWidgetsMessageBox2::Cancel()
{
	emit this->isOK(false);
	emit this->SendCloseSelfSignalToMainWidgets(false);
	this->close();
}
