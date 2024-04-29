#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QObject::connect(&socket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);
    QObject::connect(&socket_, &QAbstractSocket::stateChanged, this, &Widget::updateUI);
    QObject::connect(ui->cbUseSSL, &QCheckBox::stateChanged, this, &Widget::on_cbUseSSL_stateChanged);


    loadSettings(); // 설정 불러오기
    updateUI(); // 초기 UI 상태 업데이트
}


void Widget::loadSettings()
{
    QSettings settings("MyCompany", "MyApp");
    this->restoreGeometry(settings.value("geometry").toByteArray());
    ui->leHost->setText(settings.value("host", "localhost").toString());
    ui->lePort->setText(settings.value("port", "80").toString());
    useSSL = settings.value("useSSL", false).toBool();

}

void Widget::saveSettings()
{
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("host", ui->leHost->text());
    settings.setValue("port", ui->lePort->text());
    settings.setValue("useSSL", useSSL);
}



Widget::~Widget()
{
    saveSettings();
    delete ui;
}

void Widget::updateUI()
{
    bool isConnected = socket_.state() == QAbstractSocket::ConnectedState;
    ui->pbConnect->setEnabled(!isConnected);
    ui->pbDisconnect->setEnabled(isConnected);
    ui->pbSend->setEnabled(isConnected);
}

void Widget::on_cbUseSSL_stateChanged(int state)
{
    // 체크 상태에 따라 useSSL 값을 설정합니다.
    if (state == Qt::Checked) {
        useSSL = true;
    } else {
        useSSL = false;
    }
}


void Widget::doConnected()
{

    QString msg = "connected to " + socket_.peerAddress().toString() + "\r\n";
    ui->pteMessage->insertPlainText(msg);


}


void Widget::doDisconnected()
{
    QString msg = "Disconnected to " + socket_.peerAddress().toString() + "\r\n";
    ui->pteMessage->insertPlainText(msg);

}


void Widget::doReadyRead()
{
    QByteArray ba = socket_.readAll();
    ui->pteMessage->insertPlainText(ba);

}


void Widget::on_pbConnect_clicked()
{
    //socket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
    //socket_.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort());

    if(useSSL) {
        socket_.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort());
    } else {
        socket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
    }
}


void Widget::on_pbDisconnect_clicked()
{
    socket_.close();
}


void Widget::on_pbSend_clicked()
{
    socket_.write(ui->pteSend->toPlainText().toUtf8());
}


void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}

