#include "Process7zWorker.h"

Process7zWorker::Process7zWorker(QObject *parent)
	: QObject(parent)
{
	unzipProcess = new QThread;
	moveToThread(unzipProcess);
	unzipProcess->start();

	connect(this, &Process7zWorker::start, this, &Process7zWorker::unzip);
}

Process7zWorker::~Process7zWorker()
{

}

void Process7zWorker::setZipFilePath(QString zipFile)
{
	zipFilePath = zipFile;
}

void Process7zWorker::setUnZipFilePath(QString unZipFile)
{
	unZipFilePath = unZipFile;
}

void Process7zWorker::unzip()
{
	QString exe = QApplication::applicationDirPath() + "/7z.exe";

	QDir dir;
	if (!dir.exists(unZipFilePath)) {
		dir.mkpath(unZipFilePath);
	}

	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		emit unZipError(1);
		emit unZipFinished();
		return;
	}

	wchar_t command[1024];
	swprintf_s(command,
		1024,
		L"\"%s%s%s%s%s%s",
		reinterpret_cast<const wchar_t *>(exe.utf16()),
		L"\" x \"",
		reinterpret_cast<const wchar_t *>(zipFilePath.utf16()),
		L"\" -o\"",
		reinterpret_cast<const wchar_t *>(unZipFilePath.utf16()),
		L"\" -bt -aoa -bsp1");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);

	GetStartupInfo(&si);

	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!CreateProcess(NULL, command, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		CloseHandle(hWrite);
		CloseHandle(hRead);
		emit unZipError(2);
		emit unZipFinished();
		return;
	}
	CloseHandle(hWrite);
	char buffer[4095] = { 0 };       //用4K的空间来存储输出的内容，只要不是显示文件内容，一般情况下是够用了。
	DWORD bytesRead;

	QString res;
	while (true)
	{
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL)
			break;
		QString resStr(buffer);
		//正则表达式1(1[0-9]{2}%)|([1-9][O-9]%)|([O-9]%)/
				//res = resStr.section(QRegularExpression("^\\d{1,3}%$", QRegularExpression::MultilineOption), -1);
				//res = resStr.section("\%",-2);

				//QRegularExpression re= QRegularExpression("^(\\d{1,3})%$", QRegularExpression::MultilineOption);
				//QRegularExpressionMatch matchRes=re.match(resStr);
				//res = matchRes.captured(0);

		QRegExp rx(R"(\r\n(\d+)%)");
		int pos = 0;
		while ((pos = rx.indexIn(resStr, pos)) != -1) {
			//qDebug() << rx.capturedTexts().at(0);
			//
			res = rx.capturedTexts().at(1);
			int unzipBarValue = res.toInt();

			emit unZipProcess(unzipBarValue);

			if (unzipBarValue == 100)
			{
				break;
			}
			pos += rx.matchedLength();
			Sleep(20);
		}


	}
	CloseHandle(hRead);

	emit unZipFinished();
}
