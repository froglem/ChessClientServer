#include "player.h"

Player::Player(QTcpSocket* playerSocket):
    m_playerSocket(playerSocket)
{

}

QTcpSocket *Player::playerSocket() const
{
    return m_playerSocket;
}

QTcpSocket *Player::opponentSocket() const
{
    return m_opponentSocket;
}

QString Player::playerName() const
{
    return m_playerName;
}

void Player::setPlayerName(QString &name)
{
    m_playerName = name;
}
