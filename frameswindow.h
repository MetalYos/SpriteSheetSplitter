#ifndef FRAMESWINDOW_H
#define FRAMESWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QScrollArea>
#include "frameswindowviewmodel.h"
#include "frame.h"
#include "imagelabel.h"
#include "eventsservice.h"

// Forward Declerations
class BackgroundColorListWidget;

class FramesWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FramesWindow(QWidget *parent = nullptr);
    ~FramesWindow();

private:
    void InitGui();

    void CreateToolBar();
    void CreateFramesDock();
    void CreateCentralWidget();

    // Events Service Handles
    void OnSelectedFrameInList(EventParams& data);

private slots:
    void OnOpenSpriteSheet();
    void OnImportMetaData();
    void OnExportMetaData();
    void OnIsolateFrameToggled(bool checked);
    void OnCreateNewAnimation();

private:
    FramesWindowViewModel& viewModel;

    // Actions
    QAction* openAction;
    QAction* importAction;
    QAction* exportAction;
    QAction* isolateFrameAction;
    QAction* createAnimAction;

    // Central Widget
    QScrollArea* centralWidgetScrollArea;
    ImageLabel* spriteSheetLabel;
};

#endif // FRAMESWINDOW_H
