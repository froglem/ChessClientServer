#ifndef PLAYER_H
#define PLAYER_H
#include <QTcpSocket>
#include <QString>

class Player
{
public:
    Player(QTcpSocket* playerSocket);

    QTcpSocket* playerSocket() const;
    QTcpSocket* opponentSocket() const;
    QString playerName() const;
    void setPlayerName(QString& name);

private:
    QTcpSocket *m_playerSocket, *m_opponentSocket{nullptr};
    QString m_playerName{}, m_opponentName{};
    bool m_inGame{false};
};

#endif // PLAYER_H
