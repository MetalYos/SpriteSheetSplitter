#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>

class TimelineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    void OnTimerEvent();

private:
    void InitGui();

    void paintEvent(QPaintEvent* event) override;

private slots:

private:
    int _numFrames;
    int _currentFrame;
    int _frameSpacing;
    int _frameToFrameSpacing;
};

#endif // TIMELINEWIDGET_H
