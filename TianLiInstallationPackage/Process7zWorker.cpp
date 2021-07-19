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
		L"%s%s%s%s%s%s",
		reinterpret_cast<const wchar_t *>(exe.utf16()),
		L" x ",
		reinterpret_cast<const wchar_t *>(zipFilePath.utf16()),
		L" -o",
		reinterpret_cast<const wchar_t *>(unZipFilePath.utf16()),
		L" -bt -aoa -bsp1");

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

	while (true)
	{
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL)
			break;
		QString res(buffer);

		res = res.section("%", -2, -2);
		int unzipBarValue = res.toInt();

		emit unZipProcess(unzipBarValue);
	}
	CloseHandle(hRead);

	emit unZipFinished();
}
