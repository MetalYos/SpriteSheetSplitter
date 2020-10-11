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
#include <QCheckBox>
#include <QScrollBar>
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
      openAction(nullptr), importAction(nullptr), exportAction(nullptr),
      isolateFrameAction(nullptr),
      centralWidgetScrollArea(nullptr), spriteSheetLabel(nullptr)
{
    ui->setupUi(this);
    showMaximized();

    CreateMenu();
    CreateToolBar();
    CreateStatusBar();
    CreateFramesDock();
    CreateCentralWidget();

    EventsService::Instance().Subscribe(EventsTypes::SelectedFrameInList,
                                        std::bind(&MainWindow::OnSelectedFrameInList, this, std::placeholders::_1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateMenu()
{
    CreateFileMenu();
    CreateViewMenu();
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

void MainWindow::CreateViewMenu()
{
    isolateFrameAction = new QAction("&Isolate Frame", this);
    isolateFrameAction->setStatusTip(tr("Isolate selected frame"));
    isolateFrameAction->setDisabled(true);
    isolateFrameAction->setCheckable(true);
    connect(isolateFrameAction, &QAction::toggled, this, &MainWindow::OnIsolateFrameToggled);

    QMenu* viewMenu;
    viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction(isolateFrameAction);
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
    QWidget* centralWidget = new QWidget(this);

    QHBoxLayout* centralLayout = new QHBoxLayout();

    centralWidgetScrollArea = new QScrollArea(centralWidget);
    spriteSheetLabel = new ImageLabel(centralWidgetScrollArea);
    spriteSheetLabel->setAlignment(Qt::AlignCenter);
    centralWidgetScrollArea->setAlignment(Qt::AlignCenter);
    centralWidgetScrollArea->setWidgetResizable(true);
    centralWidgetScrollArea->setWidget(spriteSheetLabel);
    centralLayout->addWidget(centralWidgetScrollArea);

    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);
}

void MainWindow::OnSelectedFrameInList(void* data)
{
    if (data == nullptr)
        return;

    int* index = static_cast<int*>(data);
    Frame* frame = MainWindowViewModel::Instance().GetFrame(*index);
    if (frame == nullptr)
        return;

    // Enable Isolate Frame Action if disabled
    if (!isolateFrameAction->isEnabled())
        isolateFrameAction->setDisabled(false);

    // Change scroll bars if selected frame is outside of scroll area
    QPoint bottomRight = spriteSheetLabel->mapToParent(QPoint(frame->Right(), frame->Bottom()));
    QPoint topLeft = spriteSheetLabel->mapToParent(QPoint(frame->Left(), frame->Top()));
    int rightDiff = bottomRight.x() - centralWidgetScrollArea->size().width();
    int bottomDiff = bottomRight.y() - centralWidgetScrollArea->size().height();
    if (rightDiff > 0)
    {
        auto horScrollBar = centralWidgetScrollArea->horizontalScrollBar();
        horScrollBar->setValue(horScrollBar->value() + rightDiff + frame->Width());
    }
    else if (topLeft.x() < 0)
    {
        auto horScrollBar = centralWidgetScrollArea->horizontalScrollBar();
        horScrollBar->setValue(horScrollBar->value() + topLeft.x() - frame->Width());
    }

    if (bottomDiff > 0)
    {
        auto verScrollBar = centralWidgetScrollArea->verticalScrollBar();
        verScrollBar->setValue(verScrollBar->value() + bottomDiff + frame->Height());
    }
    else if (topLeft.y() < 0)
    {
        auto verScrollBar = centralWidgetScrollArea->verticalScrollBar();
        verScrollBar->setValue(verScrollBar->value() + topLeft.y() - frame->Height());
    }
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

void MainWindow::OnIsolateFrameToggled(bool checked)
{
    EventsService::Instance().Publish(EventsTypes::IsolateSelectedFrame, &checked);
}
