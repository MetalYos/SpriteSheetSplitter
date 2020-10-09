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
#include <sstream>
#include "mainwindowviewmodel.h"
#include "constants.h"
#include "framedetection.h"
#include "utils.h"
#include "jsonhelper.h"
#include "qtutils.h"
#include "backgroundcolorlistwidget.h"

#include <iostream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      openAction(nullptr), exportAction(nullptr),
      framesWidget(nullptr), originPreviewWidget(nullptr), framesList(nullptr),
      centralWidget(nullptr), spriteSheetLabel(nullptr)
{
    ui->setupUi(this);

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
    QDockWidget* framesDock = new QDockWidget(tr("Frames"), this);
    framesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    CreateFramesWidget(framesDock);

    addDockWidget(Qt::RightDockWidgetArea, framesDock);
    DisableFramesDock();
}

void MainWindow::CreateFramesWidget(QDockWidget* framesDock)
{
    // Create Frames dock inner main widget
    framesWidget = new QWidget(framesDock);

    QVBoxLayout* mainDockLayout = new QVBoxLayout();

    // Create list of Frames widget
    framesList = new QListWidget(framesDock);
    framesList->setMinimumHeight(300);
    connect(framesList, &QListWidget::currentRowChanged, this, &MainWindow::OnSelectedFrameChanged);
    mainDockLayout->addWidget(framesList);

    // Create list controls widget (add and remove buttons)
    QHBoxLayout* framesListButtonsLayout = new QHBoxLayout();

    QPushButton* addButton = new QPushButton(tr("Add"), framesWidget);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::OnAddFrameButtonClicked);

    QPushButton* removeButton = new QPushButton(tr("Remove"), framesWidget);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::OnRemoveFrameButtonClicked);

    QPushButton* clearButton = new QPushButton(tr("Clear"), framesWidget);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::OnClearFramesButtonClicked);

    framesListButtonsLayout->addWidget(addButton);
    framesListButtonsLayout->addWidget(removeButton);
    framesListButtonsLayout->addWidget(clearButton);
    mainDockLayout->addLayout(framesListButtonsLayout);

    // Create the calculate frames widget
    CreateCalcFrameWidget(mainDockLayout);

    framesWidget->setLayout(mainDockLayout);
    framesDock->setWidget(framesWidget);
}

void MainWindow::CreateCalcFrameWidget(QVBoxLayout* framesDockLayout)
{
    calcFrameWidget = new QGroupBox(tr("Calculate Frame/s"), framesWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout();

    bgColorListWidget = new BackgroundColorListWidget(this, calcFrameWidget);
    mainLayout->addWidget(bgColorListWidget);

    QFormLayout* formLayout = new QFormLayout();
    toleranceSpinBox = new QSpinBox(calcFrameWidget);
    toleranceSpinBox->setMinimum(Constants::FRAME_TOLERANCE_MIN);
    toleranceSpinBox->setMaximum(Constants::FRAME_TOLERANCE_MAX);
    toleranceSpinBox->setValue(Constants::FRAME_TOLERANCE_INIT);
    formLayout->addRow(tr("Tolerance: "), toleranceSpinBox);

    mainLayout->addLayout(formLayout);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    QPushButton* calcFrameButton = new QPushButton(tr("Calculate Frame"), calcFrameWidget);
    connect(calcFrameButton, &QPushButton::clicked, this, &MainWindow::OnCalculateFrameButtonClicked);

    QPushButton* calcAllFramesButton = new QPushButton(tr("Calculate All Frames"), calcFrameWidget);
    connect(calcAllFramesButton, &QPushButton::clicked, this, &MainWindow::OnCalculateAllFramesButtonClicked);

    buttonsLayout->addWidget(calcFrameButton);
    buttonsLayout->addWidget(calcAllFramesButton);

    mainLayout->addLayout(buttonsLayout);

    calcFrameWidget->setLayout(mainLayout);
    framesDockLayout->addWidget(calcFrameWidget);
}

void MainWindow::CreateCentralWidget()
{
    centralWidget = new QWidget(this);

    QHBoxLayout* centralLayout = new QHBoxLayout();

    spriteSheetLabel = new ImageLabel(centralWidget);
    spriteSheetLabel->setAlignment(Qt::AlignCenter);
    centralLayout->setAlignment(spriteSheetLabel, Qt::AlignCenter);

    centralLayout->addWidget(spriteSheetLabel);

    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);
}

void MainWindow::EnableFramesDock()
{
    framesWidget->setEnabled(true);
}

void MainWindow::DisableFramesDock()
{
    framesWidget->setEnabled(false);
}

void MainWindow::LoadSpriteSheet(const QString& filepath)
{
    if (spriteSheetLabel->LoadImage(filepath.toStdString()))
    {
        EnableFramesDock();
        ClearFramesList();
    }
}

void MainWindow::AddFrameToList(Frame* frame)
{
    MainWindowViewModel::Instance().AddFrame(frame);

    // Add to frames list and select it
    std::stringstream ss;
    ss << "Frame " << MainWindowViewModel::Instance().GetFrames().size();
    framesList->addItem(ss.str().c_str());
    framesList->setCurrentRow(MainWindowViewModel::Instance().GetFrames().size() - 1);

    // repaint the spritesheet label
    spriteSheetLabel->update();
}

void MainWindow::ClearFramesList()
{
    MainWindowViewModel::Instance().ClearFrames();
    framesList->clear();
    spriteSheetLabel->update();
}

void MainWindow::OnOpenSpriteSheet()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open a SpriteSheet"), "",
                                                    tr("Portable Network Graphics (*.png);;All Files(*)"));
    if (fileName.isEmpty())
        return;

    LoadSpriteSheet(fileName);
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
    LoadSpriteSheet(QString(spriteSheetPath.c_str()));

    // Clear the current list frames
    ClearFramesList();
    for (Frame* frame : results.second)
    {
        AddFrameToList(frame);
    }
}

void MainWindow::OnExportMetaData()
{
    auto frames = MainWindowViewModel::Instance().GetFrames();
    if (frames.size() == 0)
        return;

    auto sSheetPath = spriteSheetLabel->GetImage().Filepath();
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

void MainWindow::OnSelectedFrameChanged(int frameIndex)
{
    if (frameIndex < 0)
        return;

    MainWindowViewModel::Instance().SelectFrame(frameIndex);
    spriteSheetLabel->update();
}

void MainWindow::OnAddFrameButtonClicked()
{
    int left = spriteSheetLabel->ImageWidth() / 2 - Constants::FRAME_INIT_WIDTH / 2;
    int top = spriteSheetLabel->ImageHeight() / 2 - Constants::FRAME_INIT_HEIGHT / 2;

    Frame* frame = new Frame(top, left, Constants::FRAME_INIT_WIDTH, Constants::FRAME_INIT_HEIGHT);

    AddFrameToList(frame);
}

void MainWindow::OnRemoveFrameButtonClicked()
{
    if (!MainWindowViewModel::Instance().GetSelectedFrame())
        return;

    // Remove the selected frame from the list
    auto item = framesList->selectedItems()[0];
    delete framesList->takeItem(framesList->row(item));

    // Remove the selected frame
    MainWindowViewModel::Instance().RemoveSelectedFrame();

    if (MainWindowViewModel::Instance().GetFrames().size() > 0)
        framesList->setCurrentRow(MainWindowViewModel::Instance().GetFrames().size() - 1);

    // repaint the spritesheet label
    spriteSheetLabel->update();
}

void MainWindow::OnClearFramesButtonClicked()
{
    ClearFramesList();
}

void MainWindow::OnCalculateFrameButtonClicked()
{
    auto frame = MainWindowViewModel::Instance().GetSelectedFrame();
    if (frame == nullptr)
        return;

    int tolerance = toleranceSpinBox->value();
    FrameDetection::Instance().SetParameters(&spriteSheetLabel->GetImage(),
                                             bgColorListWidget->GetBgColors(), tolerance);
    Frame* temp = FrameDetection::Instance().DetectFrame(frame->OriginAtParentCoords().first,
                                                         frame->OriginAtParentCoords().second);
    if (temp != nullptr)
    {
        frame->SetFrame(temp->Top(), temp->Left(), temp->Bottom(), temp->Right());
        delete temp;
        spriteSheetLabel->update();
    }
}

void MainWindow::OnCalculateAllFramesButtonClicked()
{
    int tolerance = toleranceSpinBox->value();
    FrameDetection::Instance().SetParameters(&spriteSheetLabel->GetImage(),
                                             bgColorListWidget->GetBgColors(), tolerance);

    auto frames = FrameDetection::Instance().DetectAllFrames();

    // Clear the current list frames
    ClearFramesList();
    for (Frame* frame : frames)
    {
        AddFrameToList(frame);
    }
}
