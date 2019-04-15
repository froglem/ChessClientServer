#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewindow.h"

#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),  ui(new Ui::MainWindow), m_socket(new QTcpSocket(this))
{
    ui->setupUi(this);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectButton);
    connect(ui->challangeButton, &QPushButton::clicked, this, &MainWindow::challange);
    connect(this, &MainWindow::ussernameSet, this, &MainWindow::showUssername);
    connect(this, &MainWindow::connected, this, &MainWindow::connectionSet);
    connect(ui->onlineList, &QListWidget::itemSelectionChanged,
            [this](){
            ui->challangeButton->setEnabled(ui->onlineList->selectedItems().size());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetOnline()
{
    ui->onlineList->clear();
    QDataStream stream(m_socket);
    stream << static_cast<qint8>(Action::PlayerList);
    auto ok = m_socket->waitForReadyRead(3000);
    if(!ok){
        QMessageBox::information(this, "Error", "Server Connection problem");
        return;
    }
    QSet<QString> onlineNames{};
    stream >> onlineNames;
    onlineNames.remove(m_username);
    for(const auto &item: onlineNames){
        ui->onlineList->addItem(item);
    }

}

void MainWindow::connectButton()
{
    if(ui->connectButton->text() == "Connect"){
        bool ok;
        QString text = QInputDialog::getText(this, "Insert Username",
                                             "User name:", QLineEdit::Normal,
                                             "Player", &ok);
        text = text.trimmed();
        if (!ok || text.isEmpty()){
             QMessageBox::information(this, "Error", "Wrong Username");
             return;
        }
        m_socket->connectToHost("127.0.0.1", 55555);
        if (m_socket->waitForConnected(-1)) qDebug("Connected!");
        else {
            QMessageBox::information(this, "Error", "Server Connection problem");
            return;
        }
        QDataStream stream(m_socket);
        stream << static_cast<qint8>(Action::SendNickName) << text;
        ok = m_socket->waitForReadyRead(5000);
        if(!ok){
            QMessageBox::information(this, "Error", "Server Connection problem");
            return;
        }
        quint8 usernameValidationInt;
        stream >> usernameValidationInt;
        Action usernameValidation = static_cast<Action>(usernameValidationInt);

        if(usernameValidation == Action::NickNameInvalid){
            QMessageBox::information(this, "Error", "Username already exist");
            return;
        }

        m_username = text;

        emit ussernameSet();
        emit connected();
    }
    else {
        resetOnline();
    }
}

void MainWindow::showUssername()
{
    ui->username->setText("Username: \n" + m_username);
}

void MainWindow::connectionSet()
{
    ui->connectButton->setText("Refresh");
    resetOnline();
}

void MainWindow::challange()
{
    bool ok;
    QDataStream stream(m_socket);
    stream << static_cast<qint8>(Action::SendGameInvite)
           << ui->onlineList->selectedItems()[0]->text();
    ok = m_socket->waitForReadyRead(5000);
    if(!ok){
        QMessageBox::information(this, "Error", "Server Connection problem");
        return;
    }
}

