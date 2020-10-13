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
#include "frame.h"
#include "imagelabel.h"

// Forward Declerations
class BackgroundColorListWidget;

class FramesWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FramesWindow(QWidget *parent = nullptr);
    ~FramesWindow();

private:
    void CreateToolBar();
    void CreateFramesDock();
    void CreateCentralWidget();

    // Events Service Handles
    void OnSelectedFrameInList(void* data);

private slots:
    void OnOpenSpriteSheet();
    void OnImportMetaData();
    void OnExportMetaData();
    void OnIsolateFrameToggled(bool checked);

private:
    // Actions
    QAction* openAction;
    QAction* importAction;
    QAction* exportAction;
    QAction* isolateFrameAction;

    // Central Widget
    QScrollArea* centralWidgetScrollArea;
    ImageLabel* spriteSheetLabel;
};

#endif // FRAMESWINDOW_H
