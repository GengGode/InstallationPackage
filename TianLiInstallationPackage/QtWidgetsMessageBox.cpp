#include "QtWidgetsMessageBox.h"
#pragma execution_character_set("utf-8")

QtWidgetsMessageBox::QtWidgetsMessageBox(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	connect(ui.pushButton_OK, SIGNAL(clicked()), this, SLOT(OK()));

}

QtWidgetsMessageBox::~QtWidgetsMessageBox()
{
}

void QtWidgetsMessageBox::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton &&
		ui.TitleLabelRect->frameRect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
		m_Press = event->globalPos();
		leftBtnClk = true;
	}
	event->ignore();
}

void QtWidgetsMessageBox::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		leftBtnClk = false;
	}
	event->ignore();
}
void QtWidgetsMessageBox::mouseMoveEvent(QMouseEvent *event)
{
	if (leftBtnClk) {
		m_Move = event->globalPos();
		this->move(this->pos() + m_Move - m_Press);
		m_Press = m_Move;
	}
	event->ignore();
}

void QtWidgetsMessageBox::setText(QString messgase)
{
	ui.label_Message->setText(messgase);
}

void QtWidgetsMessageBox::OK()
{
	emit this->SendCloseSelfSignalToMainWidgets();
	this->close();
}

