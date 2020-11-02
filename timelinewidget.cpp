#include "timelinewidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <sstream>
#include "settings.h"
#include "qtutils.h"

TimelineWidget::TimelineWidget(QWidget *parent) : QWidget(parent),
    _numFrames(60), _firstFrame(1), _currentFrame(1), _frameSpacing(5), _frameToFrameSpacing(0),
    _frameRectWidth(20)
{
    _lastFrame = _firstFrame + (_numFrames - 1);

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

    DrawBackgrounds(painter);
    DrawFrameNumbers(painter);
    DrawCurrentFrame(painter);
}

void TimelineWidget::DrawBackgrounds(QPainter& painter)
{
    QRect topRect(0, 0, width(), height() * Settings::GetFloat(Settings::Fields::TIMELINE_TOP_SCALE));
    QRect bottomRect(0, topRect.bottom(), width(), height() - topRect.height());

    // Draw rectangles
    painter.setPen(Qt::NoPen);
    painter.fillRect(topRect, QtUtils::UChar3ToQColor(Settings::GetUChar3(Settings::Fields::TIMELINE_TOP_BG_COLOR)));
    painter.fillRect(bottomRect, QtUtils::UChar3ToQColor(Settings::GetUChar3(Settings::Fields::TIMELINE_BOTTOM_BG_COLOR)));
}

void TimelineWidget::DrawFrameNumbers(QPainter& painter)
{
    int padding = (int)(width() * Settings::GetFloat(Settings::TIMELINE_PADDING_SCALE));
    painter.setPen(Qt::white);

    int left = padding;
    int spacing = (width() - 2 * padding) / _numFrames;
    for (int frame = _firstFrame; frame <= _lastFrame; frame++)
    {
        if ((frame == _firstFrame) || (frame == _lastFrame) || (frame % _frameSpacing == 0))
        {
            std::stringstream ss;
            QRect textRect(left, 0, _frameRectWidth, height() * Settings::GetFloat(Settings::Fields::TIMELINE_TOP_SCALE));
            ss << frame;
            painter.drawText(textRect, Qt::AlignCenter, ss.str().c_str());
            painter.drawLine(textRect.center().x(), textRect.height(), textRect.center().x(), height());
        }
        left += spacing;
    }
}

void TimelineWidget::DrawCurrentFrame(QPainter& painter)
{
    painter.setPen(Qt::white);
    QColor selectedColor = QtUtils::UChar3ToQColor(Settings::GetUChar3(Settings::Fields::TIMELINE_SELECTED_FRAME_COLOR));

    // Find left position of frame
    int padding = (int)(width() * Settings::GetFloat(Settings::TIMELINE_PADDING_SCALE));
    int spacing = (width() - 2 * padding) / _numFrames;
    int left = padding + spacing * (_currentFrame - 1);

    std::stringstream ss;
    QRect textRect(left, 0, _frameRectWidth, height() * Settings::GetFloat(Settings::Fields::TIMELINE_TOP_SCALE));
    ss << _currentFrame;
    painter.fillRect(textRect, selectedColor);
    painter.drawText(textRect, Qt::AlignCenter, ss.str().c_str());

    QPen pen = painter.pen();
    pen.setColor(selectedColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(textRect.center().x(), textRect.height(), textRect.center().x(), height());
}
