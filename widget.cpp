#include "widget.h"
#include "ui_widget.h"
#include<QNetworkRequest>
#include<QDir>
#include<QMessageBox>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_finished()
{
    QFileInfo fileinfo;
    fileinfo.setFile(downloaded->fileName());
    downloaded->close();
    delete downloaded;
    downloaded = Q_NULLPTR;
    reply->deleteLater();
    reply = Q_NULLPTR;
    ui->btnDownLoad->setEnabled(true);
}

void Widget::on_readyread()
{
    downloaded->write(reply->readAll());
}

void Widget::on_downloaded(qint64 bytesRead, qint64 totalBytes)
{
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}


void Widget::on_btnOpenFile_clicked()
{
    QString curPath =QDir::currentPath();
    QDir dir(curPath);
    QString sub = "temp";
    dir.mkdir(sub);
    ui->editFilePath->setText(curPath+"/"+sub+"/");
}


void Widget::on_btnDownLoad_clicked()
{
    QString urlInput = ui->editUrl->text().trimmed();
    if(urlInput.isEmpty()){
        QMessageBox::information(this,"错误","请输入下载文件地址");
        return;
    }
    QUrl url = QUrl::fromUserInput(urlInput);
    if(!url.isValid()){
        QMessageBox::information(this,"错误",QString("无效url:\n%1\n错误信息:\n%2\n").arg(urlInput,url.errorString()));
        return;
    }
    QString tempDir = ui->editFilePath->text().trimmed();
    if(tempDir.isEmpty()){
        QMessageBox::information(this,"错误","Please Input Save Path");
        return;
    }
    QString fileName = tempDir+url.fileName();
    if(QFile::exists(fileName))
        QFile::remove(fileName);
    downloaded = new QFile(fileName);
    if(!downloaded->open(QIODevice::WriteOnly)){
        QMessageBox::information(this,"错误","临时文件打开错误");
        return;
    }
    ui->btnDownLoad->setEnabled(false);
    reply = networkmanager.get(QNetworkRequest(url));
    connect(reply,&QNetworkReply::finished,this,&Widget::on_finished);
    connect(reply,&QNetworkReply::readyRead,this,&Widget::on_readyread);
    void(QNetworkReply::*download)(qint64,qint64) = &QNetworkReply::downloadProgress;
    void(Widget::*on_download)(qint64,qint64) = &Widget::on_downloaded;
    connect(reply,download,this,on_download);
}

