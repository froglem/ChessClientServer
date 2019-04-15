#ifndef PLAYER_H
#define PLAYER_H
#include <QTcpSocket>
#include <QString>

enum class gameState{free, inGame, Pending};

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
    gameState m_gameState{gameState::free};
};

#endif // PLAYER_H
