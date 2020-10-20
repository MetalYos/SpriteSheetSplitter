#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolBar>
#include <QStackedWidget>
#include <QPushButton>
#include "settings.h"
#include "settingsdialog.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <functional>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      mainWidget(nullptr),
      selectFramesViewAction(nullptr), selectAnimsViewAction(nullptr),
      framesWindow(nullptr), animsWindow(nullptr)
{
    ui->setupUi(this);
    showMaximized();

    CreateMenu();
    CreateToolBar();
    CreateStatusBar();
    CreateCentralWidget();

    EventsService::Instance().Subscribe(EventsTypes::CreateAnimationPressed,
                                        std::bind(&MainWindow::OnCreateAnimationPressed, this, std::placeholders::_1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateMenu()
{
    CreateFileMenu();
}

void MainWindow::CreateFileMenu()
{
    QAction* settingsAction = new QAction("&Settings", this);
    settingsAction->setStatusTip(tr("Open Settings window"));
    connect(settingsAction, &QAction::triggered, this, &MainWindow::OnSettingsTriggered);

    QAction* quitAction = new QAction("&Quit", this);
    quitAction->setStatusTip(tr("Quit the application"));
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    QMenu* fileMenu;
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(settingsAction);
    fileMenu->addAction(quitAction);
}

void MainWindow::CreateToolBar()
{
    QToolBar* toolBar = addToolBar(tr("ViewSelection"));
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    selectFramesViewAction = new QAction(tr("Frames View"), this);
    connect(selectFramesViewAction, &QAction::triggered, this, &MainWindow::OnSelectFramesView);
    selectAnimsViewAction = new QAction(tr("Animations View"), this);
    connect(selectAnimsViewAction, &QAction::triggered, this, &MainWindow::OnSelectAnimsView);

    toolBar->addAction(selectFramesViewAction);
    toolBar->addAction(selectAnimsViewAction);

    addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::CreateStatusBar()
{
    statusBar()->showMessage(tr("Ready..."));
}

void MainWindow::CreateCentralWidget()
{
    mainWidget = new QStackedWidget(this);
    framesWindow = new FramesWindow(mainWidget);
    animsWindow = new AnimationWindow(mainWidget);
    mainWidget->addWidget(framesWindow);
    mainWidget->addWidget(animsWindow);
    mainWidget->setCurrentIndex(0);

    setCentralWidget(mainWidget);
}

void MainWindow::OnCreateAnimationPressed(EventParams& data)
{
    mainWidget->setCurrentIndex(1);
}

void MainWindow::OnSelectFramesView()
{
    mainWidget->setCurrentIndex(0);
}

void MainWindow::OnSelectAnimsView()
{
    mainWidget->setCurrentIndex(1);
}

void MainWindow::OnSettingsTriggered()
{
    SettingsDialog* dlg = new SettingsDialog(this);

    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}
