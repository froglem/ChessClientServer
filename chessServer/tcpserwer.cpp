#include "tcpserwer.h"
#include "ui_tcpserwer.h"
#include <algorithm>

#include <QMessageBox>

TCPSerwer::TCPSerwer(QWidget *parent) :
    QWidget(parent),  ui(new Ui::TCPSerwer),
    m_server(new QTcpServer(this))

{
    ui->setupUi(this);
    if(!m_server->listen(QHostAddress::LocalHost, 55555)){
        QMessageBox::information(this, "Error", "Connection problem");
        exit(-1);
    }
    connect(m_server, &QTcpServer::newConnection,
        this, &TCPSerwer::newConnection);
    connect(this, &TCPSerwer::opponentDC, this, &TCPSerwer::sendOpponentDCInfo);
}

TCPSerwer::~TCPSerwer()
{
    delete ui;
}

void TCPSerwer::newConnection()
{
    while (m_server->hasPendingConnections()){
        QTcpSocket *socket = m_server->nextPendingConnection();
        connect(socket, &QTcpSocket::disconnected,
            this, &TCPSerwer::removeConnection);
        connect(socket, &QTcpSocket::readyRead,
            this, &TCPSerwer::ProcessData);
        Player* new_player = new Player(socket);
        m_players.append(new_player);
        ui->log->appendPlainText("Connected: " + socket->peerAddress().toString() +
                                 " " + QString::number(socket->peerPort()));       
    }
}

void TCPSerwer::removeConnection()
{
     QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
     ui->log->appendPlainText("Disconnected: " + socket->peerAddress().toString() +
                              " " + QString::number(socket->peerPort()));
     auto player = std::find_if(m_players.begin(), m_players.end(),
                  [socket](auto p){return p->playerSocket() == socket;});
     if((*player)->opponentSocket() != nullptr){
          emit opponentDC((*player)->opponentSocket());
     }
     auto playerName = (*player)->playerName();
     if(!playerName.isEmpty()){
        m_usernames.removeOne(playerName);
        auto players = ui->playersList->findItems(playerName, Qt::MatchExactly);
        auto row = ui->playersList->row(players[0]);
        auto player = ui->playersList->takeItem(row);
        delete player;
     }
     m_players.removeOne(*player);
     socket->deleteLater();
}

void TCPSerwer::ProcessData()
{
    auto socket = qobject_cast<QTcpSocket*>(sender());
    auto player = *(std::find_if(m_players.begin(), m_players.end(),
                 [socket](auto p){return p->playerSocket() == socket;}));
    QDataStream dataStream(socket);
    qint8 actionData{};
    dataStream >> actionData;
    Action action = static_cast<Action>(actionData);
    switch (action) {

    case Action::SendNickName:{
        QString name{};
        dataStream >> name;
        if(m_usernames.contains(name)){
            dataStream << static_cast<quint8>(Action::NickNameInvalid);
        }
        else {
            m_usernames.append(name);
            player->setPlayerName(name);
            ui->playersList->addItem(name);
            dataStream << static_cast<quint8>(Action::NickNameValid);
        }
        break;
        }
    case Action::PlayerList:{
        dataStream << m_usernames;
    }

    }
}

void TCPSerwer::sendOpponentDCInfo(QTcpSocket * socket)
{
    QDataStream dataStream(socket);
    qint8 actionData = static_cast<qint8>(Action::OpponentDC);
    dataStream >> actionData;
}
