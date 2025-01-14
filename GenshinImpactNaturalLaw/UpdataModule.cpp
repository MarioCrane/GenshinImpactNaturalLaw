#include "UpdataModule.h"

UpdataModule::UpdataModule()
{
	downloadFile = NULL;
	downloadReply = NULL;
	bisFinished = false;
}

UpdataModule::~UpdataModule()
{
	cancel();
}

void UpdataModule::setData(QUrl url, QString pathtoSave)
{
	QString sd = QSslSocket::sslLibraryBuildVersionString();
	bool is= QSslSocket::supportsSsl();
	downloadUrl = url;

	QDir dir;
	QString filePath = QApplication::applicationDirPath() + "/download/";
	if (!dir.exists(filePath)) {
		dir.mkpath(filePath);
	}

	savePath = filePath + downloadUrl.fileName();
}

void UpdataModule::getFile()
{
	if (savePath.isEmpty() || downloadUrl.isEmpty())
	{
		return;
	}
	if (downloadFile)
	{
		return;
	}
	bisFinished = false;
	downloadFile = new QFile(savePath);
	FileBuff = "";
	if (!downloadFile->open(QIODevice::WriteOnly))
	{
		delete downloadFile;
		downloadFile = 0;
		return;
	}
	allBits = downloadFile->size();
	httpRequestAborted = false;
	//尝试获取文件
	startRequest(downloadUrl);
	//事件循环，防止在下载没完成前结束对象
}

void UpdataModule::startRequest(QUrl url)
{
	QNetworkRequest req(url);
	if (allBits)
	{
		QByteArray rangeHeaderValue = "bytes=" + QByteArray::number(allBits) + "-";// + QByteArray::number(already + cUpdateChunk() - 1); 
		req.setRawHeader("Range", rangeHeaderValue);
		req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
		req.setRawHeader("RANGE", tr("bytes=%1-").arg(allBits).toUtf8());//
	}
	downloadReply = downloadmanager.get(req);
	//连接信号与槽
	if (downloadReply)
	{
		connect(downloadReply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
		connect(downloadReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
		connect(downloadReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
	}
}

QString UpdataModule::getDownloadVersionCode()
{
	return versionCode;
}

void UpdataModule::cancel()
{
	if (downloadFile)
	{
		downloadFile->close();
		delete downloadFile;
		downloadFile = NULL;
	}
	if (downloadReply)
	{
		downloadReply->deleteLater();
		downloadReply = NULL;
	}
}
void UpdataModule::downloadReadyRead()
{
	//如果文件可以访问，下载
	if (downloadFile)
		downloadFile->write(downloadReply->readAll());
	FileBuff = FileBuff + QString(downloadReply->readAll()); 
}
void UpdataModule::downloadFinished()
{
	if (httpRequestAborted)
	{
		cancel();
		return;
	}
	downloadFile->flush();
	downloadFile->close();

	

	QVariant redirectionTarget = downloadReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (downloadReply->error())
	{//299提示RANGE错误
		//DEBUG_LOG(qsl("downloadReply error code:") + QString::number(downloadReply->error()));
		//downloadFile->remove();
		//emit downloadResult(-1);
	}
	else if (!redirectionTarget.isNull())
	{
		QUrl newUrl = downloadUrl.resolved(redirectionTarget.toUrl());
		{
			downloadUrl = newUrl;
			downloadReply->deleteLater();
			downloadFile->open(QIODevice::WriteOnly);
			downloadFile->resize(0); allBits = 0;
			startRequest(downloadUrl);
			return;
		}
	}
	else
	{

		QString fileName = QFileInfo(downloadUrl.path()).fileName();
		delete downloadFile;
		downloadFile = 0;
		bisFinished = true;
		emit downloadResult(0);
	}
	downloadReply->deleteLater();
	downloadReply = 0;
	if (downloadFile)
	{
		delete downloadFile;
		downloadFile = 0;
	}
}

void UpdataModule::downloadError(QNetworkReply::NetworkError errorCode)
{
	emit downloadResult(-1);
}

void UpdataModule::downloadProgress(qint64, qint64)
{
}
