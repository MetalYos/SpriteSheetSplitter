#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include <QPainter>

class TimelineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    void OnTimerEvent();

private:
    void InitGui();
    void DrawBackgrounds(QPainter& painter);
    void DrawFrameNumbers(QPainter& painter);
    void DrawCurrentFrame(QPainter& painter);

    void paintEvent(QPaintEvent* event) override;

private slots:

private:
    int _numFrames;
    int _firstFrame;
    int _lastFrame;
    int _currentFrame;
    int _frameSpacing;
    int _frameToFrameSpacing;
    int _frameRectWidth;
};

#endif // TIMELINEWIDGET_H
