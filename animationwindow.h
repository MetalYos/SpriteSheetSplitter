#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QPushButton>
#include <QImage>
#include <QListWidget>
#include <QScrollArea>
#include <QLabel>
#include <QTimer>
#include "eventsservice.h"
#include "animationswindowviewmodel.h"
#include "image.h"

class AnimationWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AnimationWindow(QWidget *parent = nullptr);

private:
    void InitGui();

    void CreateAnimationsDock();
    void CreateTimelineDock();
    void CreatePlayControlsWidget(QDockWidget* timelineDock);
    void CreateCentralWidget();

    bool SetFrameImage();
    void SetFrameLabel();
    void AddAnimationToList(int index);

    // Event Handles
    void OnSpriteSheetLoaded(EventParams& data);
    void OnCreateAnimationPressed(EventParams& data);

private slots:
    void OnGoToStartPressed();
    void OnGoToEndPressed();
    void OnPrevFramePressed();
    void OnNextFramePressed();
    void OnPlayPausePressed();
    void OnTimeout();

private:
    AnimationsWindowViewModel& viewModel;
    QPushButton* playPauseButton;
    QListWidget* animsList;

    QLabel* titleLabel;
    QScrollArea* centralWidgetScrollArea;
    QLabel* imageLabel;
    QImage* qimage;

    QTimer* timer;
};

#endif // ANIMATIONWINDOW_H
