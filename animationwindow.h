#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QMainWindow>

class AnimationWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AnimationWindow(QWidget *parent = nullptr);

private:
    void InitGui();

    void CreateAnimationsDock();
    void CreateTimelineDock();

};

#endif // ANIMATIONWINDOW_H
