#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QSslSocket>
#include <QSettings> // 추가: 설정을 저장하고 불러오기 위함

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //QTcpSocket socket_; // 이제 QSslSocket을 사용할 수 있도록 변경
    QSslSocket socket_;
    bool useSSL = false; // SSL 사용 여부를 결정하는 변수

public slots:
    void doConnected();
    void doDisconnected();
    void doReadyRead();
    void updateUI(); // 소켓 상태에 따라 UI 업데이트하는 슬롯

private slots:
    void on_pbConnect_clicked();
    void on_pbDisconnect_clicked();
    void on_pbSend_clicked();
    void on_pbClear_clicked();
    void on_cbUseSSL_stateChanged(int arg1);

private:
    Ui::Widget *ui;
    void loadSettings(); // 설정을 불러오는 함수
    void saveSettings(); // 설정을 저장하는 함수
};

#endif // WIDGET_H
