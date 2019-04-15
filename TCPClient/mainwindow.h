#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDataStream>

namespace Ui {
class MainWindow;
}

enum class Action : qint8
{SendNickName, NickNameValid, NickNameInvalid, OpponentDC,
PlayerList, SendGameInvite};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_socket;
    QString m_username;

    void resetOnline();

private slots:
    void connectButton();
    void showUssername();
    void connectionSet();
    void challange();

signals:
    void ussernameSet();
    void connected();

};

#endif // MAINWINDOW_H
