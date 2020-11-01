#include "timelinewidget.h"
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <sstream>

TimelineWidget::TimelineWidget(QWidget *parent) : QWidget(parent),
    _numFrames(60), _currentFrame(1), _frameSpacing(5)
{
    InitGui();
}

void TimelineWidget::InitGui()
{
    setMinimumWidth(300);
    setMinimumHeight(70);

    QVBoxLayout* layout = new QVBoxLayout();
    QLabel* dummy = new QLabel();
    layout->addWidget(dummy);
    setLayout(layout);
}

void TimelineWidget::OnTimerEvent()
{

}

void TimelineWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect topRect(0, 0, width(), height() * 0.4);
    QRect bottomRect(0, topRect.bottom(), width(), height() - topRect.height());
    QRect textRect;

    // Draw rectangles
    painter.setPen(Qt::NoPen);
    painter.fillRect(topRect, QColor(43, 43, 43));
    painter.fillRect(bottomRect, QColor(66, 66, 66));

    // Draw frame numbers
    int padding = (int)(width() * 0.02f);
    topRect.setLeft(topRect.left() + padding);
    bottomRect.setLeft(bottomRect.left() + padding);
    int actualWidth = width() - 2 * padding;
    int numIterations = _numFrames / _frameSpacing;
    int spacing = actualWidth / (numIterations);
    painter.setPen(Qt::white);

    // Draw first frame number
    painter.drawText(topRect, Qt::AlignVCenter, "1", &textRect);
    painter.drawLine(textRect.center().x(), bottomRect.top(), textRect.center().x(), bottomRect.bottom());
    topRect.setLeft(topRect.left() + spacing);
    bottomRect.setLeft(bottomRect.left() + spacing);

    int index = 1;
    for (int index = 1; index <= numIterations; index++)
    {
        std::stringstream sstream;
        sstream << index * _frameSpacing;
        painter.drawText(topRect, Qt::AlignVCenter, sstream.str().c_str(), &textRect);
        painter.drawLine(textRect.center().x(), bottomRect.top(), textRect.center().x(), bottomRect.bottom());
        topRect.setLeft(topRect.left() + spacing);
        bottomRect.setLeft(bottomRect.left() + spacing);
    }

    if ((_numFrames - (_frameSpacing * index)) > 0) {
        // Draw last frame
        std::stringstream sstream;
        sstream << _numFrames;
        painter.drawText(topRect, Qt::AlignVCenter, sstream.str().c_str(), &textRect);
        painter.drawLine(textRect.center().x(), bottomRect.top(),textRect.center().x(), bottomRect.bottom());
    }
}
