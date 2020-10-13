#include "animationwindow.h"

#include <QLabel>
#include <QDockWidget>

AnimationWindow::AnimationWindow(QWidget *parent) : QMainWindow(parent)
{
    InitGui();
}

void AnimationWindow::InitGui()
{
    CreateAnimationsDock();
    CreateTimelineDock();

    setWindowFlags(Qt::Widget);

    QLabel* label = new QLabel(tr("Animations View!"), this);

    setCentralWidget(label);

}

void AnimationWindow::CreateAnimationsDock()
{
    QDockWidget* animsDock = new QDockWidget(tr("Animations"), this);
    animsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    animsDock->setMinimumWidth(300);
    addDockWidget(Qt::RightDockWidgetArea, animsDock);
}

void AnimationWindow::CreateTimelineDock()
{
    QDockWidget* timelineDock = new QDockWidget(tr("Timeline"), this);
    timelineDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    timelineDock->setMinimumHeight(100);
    addDockWidget(Qt::BottomDockWidgetArea, timelineDock);
}
