#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagelabel.h"
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

// Forward Declerations
class BackgroundColorListWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void CreateMenu();
    void CreateFileMenu();
    void CreateViewMenu();
    void CreateToolBar();
    void CreateStatusBar();
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
    Ui::MainWindow *ui;

    // Actions
    QAction* openAction;
    QAction* importAction;
    QAction* exportAction;
    QAction* isolateFrameAction;

    // Central Widget
    QScrollArea* centralWidgetScrollArea;
    ImageLabel* spriteSheetLabel;
};
#endif // MAINWINDOW_H
