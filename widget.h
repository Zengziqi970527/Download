#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QFile>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    QNetworkAccessManager networkmanager;
    QNetworkReply *reply;
    QFile *downloaded;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void on_finished();
    void on_readyread();
    void on_downloaded(qint64 bytesRead,qint64 totalBytes);
    void on_btnOpenFile_clicked();

    void on_btnDownLoad_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
