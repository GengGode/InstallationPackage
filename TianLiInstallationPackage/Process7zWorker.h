#pragma once

#include <QDir>
#include <QObject>
#include <QThread>
#include <QApplication>
#include <Windows.h>

class Process7zWorker : public QObject
{
	Q_OBJECT

public:
	Process7zWorker(QObject *parent);
	~Process7zWorker();
private:
	QThread *unzipProcess = nullptr;

	QString zipFilePath;
	QString unZipFilePath;

public:
	void setZipFilePath(QString zipFilePath);
	void setUnZipFilePath(QString unZipFilePath);


private:
	void unzip();

signals:
	void start();
signals:
	void unZipError();
	void valueChanged(int value);
	void unZipFinished();
};
