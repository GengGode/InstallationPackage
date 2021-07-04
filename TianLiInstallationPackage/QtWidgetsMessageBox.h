#pragma once
#pragma execution_character_set("utf-8")
#include <QMouseEvent>
#include <QWidget>
#include "ui_QtWidgetsMessageBox.h"

class QtWidgetsMessageBox : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsMessageBox(QWidget *parent = Q_NULLPTR);
	~QtWidgetsMessageBox();

private:
	Ui::QtWidgetsMessageBox ui;

private:
	QPoint m_Press;
	QPoint m_Move;
	bool leftBtnClk = false;

private:
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

public:
	void setText(QString messgase);

private slots:
	void OK();

signals:
	void SendCloseSelfSignalToMainWidgets();
};
