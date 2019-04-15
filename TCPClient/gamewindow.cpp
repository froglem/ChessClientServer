#include "gamewindow.h"
#include "ui_gamewindow.h"


#include <QGraphicsView>

GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(new QGraphicsView(this));
}

GameWindow::~GameWindow()
{
    delete ui;
}
