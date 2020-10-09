#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QToolBar>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QListWidgetItem>
#include <QFormLayout>
#include <QMessageBox>
#include <QPoint>
#include <QScrollArea>
#include <QCheckBox>
#include <sstream>
#include "mainwindowviewmodel.h"
#include "settings.h"
#include "framedetection.h"
#include "utils.h"
#include "jsonhelper.h"
#include "qtutils.h"
#include "backgroundcolorlistwidget.h"
#include "framesdock.h"
#include "eventsservice.h"

#include <iostream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      openAction(nullptr), exportAction(nullptr),
      centralWidget(nullptr), spriteSheetLabel(nullptr)
{
    ui->setupUi(this);
    showMaximized();

    CreateMenu();
    CreateToolBar();
    CreateStatusBar();
    CreateFramesDock();
    CreateCentralWidget();
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
    openAction = new QAction("&Open SpriteSheet", this);
    openAction->setStatusTip(tr("Select a SpriteSheet to process"));
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::OnOpenSpriteSheet);

    importAction = new QAction("&Import MetaData", this);
    importAction->setStatusTip(tr("Import the metadata of a spritesheet"));
    connect(importAction, &QAction::triggered, this, &MainWindow::OnImportMetaData);

    exportAction = new QAction("&Export MetaData", this);
    exportAction->setStatusTip(tr("Export the processed metadata of the spritesheet"));
    exportAction->setShortcuts(QKeySequence::Save);
    connect(exportAction, &QAction::triggered, this, &MainWindow::OnExportMetaData);

    QAction* quitAction = new QAction("&Quit", this);
    quitAction->setStatusTip(tr("Quit the application"));
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    QMenu* fileMenu;
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(openAction);
    fileMenu->addAction(exportAction);
    fileMenu->addAction(quitAction);
}

void MainWindow::CreateToolBar()
{
    QToolBar* toolBar = addToolBar(tr("File"));
    toolBar->addAction(openAction);
    toolBar->addAction(importAction);
    toolBar->addAction(exportAction);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::CreateStatusBar()
{
    statusBar()->showMessage(tr("Ready..."));
}

void MainWindow::CreateFramesDock()
{
    FramesDock* framesDock = new FramesDock(tr("Frames"), this);
    framesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, framesDock);
}

void MainWindow::CreateCentralWidget()
{
    centralWidget = new QWidget(this);

    QHBoxLayout* centralLayout = new QHBoxLayout();

    QScrollArea* scrollArea = new QScrollArea(centralWidget);
    spriteSheetLabel = new ImageLabel(scrollArea);
    spriteSheetLabel->setAlignment(Qt::AlignCenter);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(spriteSheetLabel);
    centralLayout->addWidget(scrollArea);

    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);
}

void MainWindow::OnOpenSpriteSheet()
{
    // Get the image filepath
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open a SpriteSheet"), "",
                                                    tr("Portable Network Graphics (*.png);;All Files(*)"));
    if (fileName.isEmpty())
        return;

    // Load image and pusblish the relevant event
    if (MainWindowViewModel::Instance().LoadImage(fileName.toStdString()))
    {
        spriteSheetLabel->LoadImage(MainWindowViewModel::Instance().GetImage());
        MainWindowViewModel::Instance().ClearFrames();
        EventsService::Instance().Publish(EventsTypes::SpriteSheetLoaded, nullptr);
    }
}

void MainWindow::OnImportMetaData()
{
    QString filepath = QFileDialog::getOpenFileName(this,
                                                    tr("Import a SpriteSheet MetaData file"), "",
                                                    tr("JSON (*.json);;All Files(*)"));
    if (filepath.isEmpty())
        return;

    std::pair<std::string, std::vector<Frame*>> results;
    JsonHelper::ImportJson(filepath.toStdString(), results);

    // Load the new sprite sheet image
    auto spriteSheetPath = StringUtils::ReplaceFilename(filepath.toStdString(), results.first);
    if (MainWindowViewModel::Instance().LoadImage(spriteSheetPath))
    {
        spriteSheetLabel->LoadImage(MainWindowViewModel::Instance().GetImage());
        MainWindowViewModel::Instance().SetFrames(results.second);
        EventsService::Instance().Publish(EventsTypes::SpriteSheetLoaded, &results.second);
    }
}

void MainWindow::OnExportMetaData()
{
    auto frames = MainWindowViewModel::Instance().GetFrames();
    if (frames.size() == 0)
        return;

    auto sSheetPath = MainWindowViewModel::Instance().GetImage()->Filepath();
    auto sSheetFilename = StringUtils::GetFilename(sSheetPath);
    auto sSheetName = StringUtils::GetFilenameWithoutExt(sSheetFilename);
    auto outputPath = StringUtils::GetFilepathWithoutFile(sSheetPath) + sSheetName + ".json";

    QMessageBox msgBox;
    QString statusBarMessage;
    if (JsonHelper::ExportJson(outputPath, sSheetName, sSheetFilename, frames))
    {
        msgBox.setText("Export finished successfully");
        statusBarMessage = "File was exported to " + QString(outputPath.c_str());
    }
    else
    {
        msgBox.setText("Export failed!");
        statusBarMessage = "Export to file " + QString(outputPath.c_str()) + " failed!";
    }
    msgBox.exec();
    statusBar()->showMessage("File was exported to " + QString(outputPath.c_str()), 3);
}
