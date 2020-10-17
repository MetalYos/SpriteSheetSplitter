#include "frameswindow.h"
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
#include <QStringList>
#include "settings.h"
#include "utils.h"
#include "jsonhelper.h"
#include "qtutils.h"
#include "backgroundcolorlistwidget.h"
#include "framesdock.h"
#include "eventsservice.h"

#include <sstream>
#include <iostream>
#include <fstream>

FramesWindow::FramesWindow(QWidget *parent)
    : QMainWindow(parent),
      viewModel(FramesWindowViewModel::Instance()),
      openAction(nullptr), importAction(nullptr), exportAction(nullptr),
      isolateFrameAction(nullptr), createAnimAction(nullptr),
      centralWidgetScrollArea(nullptr), spriteSheetLabel(nullptr)
{
    InitGui();

    EventsService::Instance().Subscribe(EventsTypes::SelectedFrameInList,
                                        std::bind(&FramesWindow::OnSelectedFrameInList, this, std::placeholders::_1));
}

FramesWindow::~FramesWindow()
{

}

void FramesWindow::InitGui()
{
    setWindowFlags(Qt::Widget);

    CreateToolBar();
    CreateFramesDock();
    CreateCentralWidget();
}

void FramesWindow::CreateToolBar()
{
    QToolBar* toolBar = addToolBar(tr("File"));

    openAction = new QAction("&Open SpriteSheet", this);
    openAction->setStatusTip(tr("Select a SpriteSheet to process"));
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &FramesWindow::OnOpenSpriteSheet);

    importAction = new QAction("&Import MetaData", this);
    importAction->setStatusTip(tr("Import the metadata of a spritesheet"));
    connect(importAction, &QAction::triggered, this, &FramesWindow::OnImportMetaData);

    exportAction = new QAction("&Export MetaData", this);
    exportAction->setStatusTip(tr("Export the processed metadata of the spritesheet"));
    exportAction->setShortcuts(QKeySequence::Save);
    connect(exportAction, &QAction::triggered, this, &FramesWindow::OnExportMetaData);

    isolateFrameAction = new QAction("&Isolate Frame", this);
    isolateFrameAction->setStatusTip(tr("Isolate selected frame"));
    isolateFrameAction->setDisabled(true);
    isolateFrameAction->setCheckable(true);
    connect(isolateFrameAction, &QAction::toggled, this, &FramesWindow::OnIsolateFrameToggled);

    createAnimAction = new QAction("&Create Animation", this);
    createAnimAction->setStatusTip(tr("Create Animation from selected frames"));
    createAnimAction->setDisabled(true);
    connect(createAnimAction, &QAction::triggered, this, &FramesWindow::OnCreateNewAnimation);

    toolBar->addAction(openAction);
    toolBar->addAction(importAction);
    toolBar->addAction(exportAction);
    toolBar->addAction(isolateFrameAction);
    toolBar->addAction(createAnimAction);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    addToolBar(Qt::TopToolBarArea, toolBar);
}

void FramesWindow::CreateFramesDock()
{
    FramesDock* framesDock = new FramesDock(tr("Frames"), this);
    framesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, framesDock);
}

void FramesWindow::CreateCentralWidget()
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

void FramesWindow::OnSelectedFrameInList(void* data)
{
    if (data == nullptr)
        return;

    int* index = static_cast<int*>(data);
    Frame* frame = viewModel.GetFrame(*index);
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

void FramesWindow::OnOpenSpriteSheet()
{
    // Get the image filepath
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open a SpriteSheet"), "",
                                                    tr("Portable Network Graphics (*.png);;All Files(*)"));
    if (fileName.isEmpty())
        return;

    // Load image and pusblish the relevant event
    if (viewModel.LoadImage(fileName.toStdString()))
    {
        spriteSheetLabel->LoadImage(viewModel.GetImage());
        viewModel.ClearFrames();
        EventsService::Instance().Publish(EventsTypes::SpriteSheetLoaded, nullptr);

        // Enable create animation action
        createAnimAction->setDisabled(false);
    }
}

void FramesWindow::OnImportMetaData()
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
    if (viewModel.LoadImage(spriteSheetPath))
    {
        spriteSheetLabel->LoadImage(viewModel.GetImage());
        viewModel.SetFrames(results.second);
        std::pair<const Image*, std::vector<Frame*>> output;
        output.first = viewModel.GetImage();
        output.second = results.second;
        EventsService::Instance().Publish(EventsTypes::SpriteSheetLoaded, &output);

        // Enable create animation action
        createAnimAction->setDisabled(false);
    }
}

void FramesWindow::OnExportMetaData()
{
    auto frames = viewModel.GetFrames();
    if (frames.size() == 0)
        return;

    auto sSheetPath = viewModel.GetImage()->Filepath();
    auto sSheetFilename = StringUtils::GetFilename(sSheetPath);
    auto sSheetName = StringUtils::GetFilenameWithoutExt(sSheetFilename);
    auto outputPath = StringUtils::GetFilepathWithoutFile(sSheetPath) + sSheetName + ".json";

    QMessageBox msgBox;
    QString statusBarMessage;
    if (JsonHelper::ExportJson(outputPath, sSheetName, sSheetFilename, frames))
    {
        msgBox.setText("Export finished successfully");
    }
    else
    {
        msgBox.setText("Export failed!");
    }
    msgBox.exec();
}

void FramesWindow::OnIsolateFrameToggled(bool checked)
{
    EventsService::Instance().Publish(EventsTypes::IsolateSelectedFrame, &checked);
}

void FramesWindow::OnCreateNewAnimation()
{
    auto frames = viewModel.GetSelectedFrames();

    EventsService::Instance().Publish(EventsTypes::CreateAnimationPressed, &frames);
}
