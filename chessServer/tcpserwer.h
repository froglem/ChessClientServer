#ifndef TCPSERWER_H
#define TCPSERWER_H
#include "player.h"

#include <QWidget>
#include <QtNetwork>

enum class Action : qint8
{SendNickName, NickNameValid, NickNameInvalid, OpponentDC,
PlayerList};


namespace Ui {
class TCPSerwer;
}

class TCPSerwer : public QWidget
{
    Q_OBJECT

public:
    explicit TCPSerwer(QWidget *parent = nullptr);
    ~TCPSerwer();

private:
    Ui::TCPSerwer *ui;
    QTcpServer* m_server;
    QList<Player*> m_players;
    QList<QString> m_usernames{};

private slots:
    void newConnection();
    void removeConnection();
    void ProcessData();
    void sendOpponentDCInfo(QTcpSocket*);

signals:
    void opponentDC(QTcpSocket*);
};

#endif // TCPSERWER_H
