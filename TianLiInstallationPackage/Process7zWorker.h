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
	void setZipFilePath(QString zipFile);
	void setUnZipFilePath(QString unZipFile);

public slots:
	void unzip();

signals:
	void start();
signals:
	void unZipError(int errorCode);
	void unZipProcess(int value);
	void unZipFinished();
};
