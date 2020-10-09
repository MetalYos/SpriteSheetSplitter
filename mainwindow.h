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
    void CreateToolBar();
    void CreateStatusBar();
    void CreateFramesDock();
    void CreateFramesWidget(QDockWidget* framesDock);
    void CreateCalcFrameWidget(QVBoxLayout* framesDockLayout);
    void CreateCentralWidget();

    void EnableFramesDock();
    void DisableFramesDock();

    void LoadSpriteSheet(const QString& filepath);
    void AddFrameToList(Frame* frame);
    void ClearFramesList();

    // Events Service Handles
    void OnEndBgColorPick(void* data);

private slots:
    void OnOpenSpriteSheet();
    void OnImportMetaData();
    void OnExportMetaData();
    void OnSelectedFrameChanged(int frameIndex);
    void OnAddFrameButtonClicked();
    void OnRemoveFrameButtonClicked();
    void OnClearFramesButtonClicked();
    void OnCalculateFrameButtonClicked();
    void OnCalculateAllFramesButtonClicked();

private:
    Ui::MainWindow *ui;

    // Actions
    QAction* openAction;
    QAction* importAction;
    QAction* exportAction;

    // Frame Dock widgets
    QWidget* framesWidget;
    QGroupBox* calcFrameWidget;
    QGroupBox* originPreviewWidget;
    QListWidget* framesList;

    // Calc Frames Parameters
    BackgroundColorListWidget* bgColorListWidget;
    QSpinBox* toleranceSpinBox;

    // Central Widget
    QWidget* centralWidget;
    ImageLabel* spriteSheetLabel;
};
#endif // MAINWINDOW_H
