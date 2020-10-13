#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QAction>
#include "frameswindow.h"

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
    void CreateCentralWidget();

private slots:
    void OnSelectFramesView();
    void OnSelectAnimsView();

private:
    Ui::MainWindow *ui;

    QStackedWidget* mainWidget;

    QAction* selectFramesViewAction;
    QAction* selectAnimsViewAction;

    FramesWindow* framesWindow;
};
#endif // MAINWINDOW_H
