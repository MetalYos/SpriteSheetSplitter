#include "imagelabel.h"
#include <QMainWindow>
#include <QStatusBar>
#include <QPainter>
#include <QPen>
#include <sstream>
#include "frameswindowviewmodel.h"
#include "settings.h"
#include "eventsservice.h"
#include "qtutils.h"

ImageLabel::ImageLabel(QWidget* parent) :
    QLabel(parent),
    viewModel(FramesWindowViewModel::Instance()),
    qimage(nullptr), _isBgColorSelectionMode(false),
    _isIsolateFrameMode(false), _isCtrlPressed(false)
{
    mouseButtons[0] = false;
    mouseButtons[1] = false;
    mouseButtons[2] = false;

    lastCursor = cursor();
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFocusPolicy(Qt::StrongFocus);

    EventsService::Instance().Subscribe(EventsTypes::StartBgColorPick,
                                        std::bind(&ImageLabel::OnStartBgColorPick, this, std::placeholders::_1));
    EventsService::Instance().Subscribe(EventsTypes::RedrawImage,
                                        std::bind(&ImageLabel::OnRedrawImage, this, std::placeholders::_1));
    EventsService::Instance().Subscribe(EventsTypes::IsolateSelectedFrame,
                                        std::bind(&ImageLabel::OnIsolateSelectedFrame, this, std::placeholders::_1));
}

bool ImageLabel::LoadImage(const Image* image)
{
    if (image == nullptr)
        return false;

    if (image != nullptr)
    {
        QImage* temp = new QImage(image->Pixels(),
                           image->Width(),
                           image->Height(),
                           image->BytesPerLine(),
                           QImage::Format_ARGB32);
        qimage = new QImage(temp->rgbSwapped());
        delete temp;
        QPixmap pixmap = QPixmap::fromImage(*qimage);
        pixmap.scaled(size(), Qt::KeepAspectRatio);
        setPixmap(pixmap);

        lastMousePos = std::pair<int, int>(width() / 2, height() / 2);

        setStyleSheet("border: 1px solid black;");

        return true;
    }
    return false;
}

void ImageLabel::OnStartBgColorPick(EventParams& data)
{
    // Enable Color Selection Mode
    _isBgColorSelectionMode = true;

    // Set Cursor to be of a drip
    QtUtils::SetCursor(this, Settings::GetStr(Settings::Fields::COLOR_PICKER_CURSOR_PATH).c_str(), 2, 30);
}

void ImageLabel::OnRedrawImage(EventParams& data)
{
    update();
}

void ImageLabel::OnIsolateSelectedFrame(EventParams& data)
{
    IsolateSelectedFrameParams& params = dynamic_cast<IsolateSelectedFrameParams&>(data);
    _isIsolateFrameMode = params.Isolate;
    update();
}

QSize ImageLabel::sizeHint() const
{
    int width = 0;
    int height = 0;

    auto image = viewModel.GetImage();
    if (image != nullptr)
    {
        width = image->Width();
        height = image->Height();
    }

    return QSize(width, height);
}

void ImageLabel::mousePressEvent(QMouseEvent* event)
{
    mouseButtons[Settings::LEFT_MOUSE_BUTTON] = (event->button() == Qt::LeftButton);
    mouseButtons[Settings::RIGHT_MOUSE_BUTTON] = (event->button() == Qt::RightButton);
    mouseButtons[Settings::MIDDLE_MOUSE_BUTTON] = (event->button() == Qt::MiddleButton);

    if (mouseButtons[Settings::LEFT_MOUSE_BUTTON])
    {
        bool nonSelected = true;
        lastMousePos = std::pair<int, int>(event->x(), event->y());
        if (_isBgColorSelectionMode)
        {
            // Get Color
            auto image = viewModel.GetImage();
            auto color = image->GetPixelColor(event->x(), event->y());
            EndBgColorPickParams params;
            params.Color = color;
            EventsService::Instance().Publish(EventsTypes::EndBgColorPick, params);
            setCursor(Qt::ArrowCursor);
            _isBgColorSelectionMode = false;
        }
        else
        {
            for (int i = viewModel.GetFrames().size() - 1; i >= 0; i--)
            {
                Frame* frame = viewModel.GetFrame(i);
                if (frame != nullptr && frame->ContainesPoint(event->x(), event->y()))
                {
                    viewModel.SelectFrame(i, !_isCtrlPressed);
                    nonSelected = false;

                    SelectedFrameOnImageParams params;
                    params.FrameIndex = i;
                    params.IsCtrlPressed = _isCtrlPressed;
                    EventsService::Instance().Publish(EventsTypes::SelectedFrameOnImage, params);
                    break;
                }
            }
        }

        if (nonSelected)
        {
            viewModel.DeselectAllFrames();

            SelectedFrameOnImageParams params;
            params.FrameIndex = -1;
            EventsService::Instance().Publish(EventsTypes::SelectedFrameOnImage, params);
        }

        update();
    }

    QLabel::mousePressEvent(event);
}

void ImageLabel::mouseReleaseEvent(QMouseEvent* event)
{
    mouseButtons[Settings::LEFT_MOUSE_BUTTON] = !(event->button() == Qt::LeftButton);
    mouseButtons[Settings::RIGHT_MOUSE_BUTTON] = !(event->button() == Qt::RightButton);
    mouseButtons[Settings::MIDDLE_MOUSE_BUTTON] = !(event->button() == Qt::MiddleButton);
}

void ImageLabel::mouseMoveEvent(QMouseEvent* event)
{
    if (qimage == nullptr)
        return;

    auto image = viewModel.GetImage();
    bool toUpdate = false;

    for (auto frame : viewModel.GetFrames())
    {
        if (frame->ContainesPoint(event->x(), event->y()))
        {
            if (mouseButtons[Settings::LEFT_MOUSE_BUTTON])
            {
                frame->Move(event->x() - lastMousePos.first, event->y() - lastMousePos.second,
                        image->Width(), image->Height());
                lastCursor = cursor();
                setCursor(Qt::DragMoveCursor);
                toUpdate = true;
            }
            else
            {
                lastCursor = cursor();
                setCursor(Qt::PointingHandCursor);
            }
        }
        else
        {
            setCursor(lastCursor);
        }
    }

    if (event->x() >= 0 && event->x() <= image->Width() &&
        event->y() >= 0 && event->y() <= image->Height())
    {

        // Get Main window widget
        QWidget* p = parentWidget();
        while (p->parentWidget() != nullptr)
            p = p->parentWidget();

        // Show coordinates and color in status bar
        std::stringstream ss;
        ss << "(" << event->x() << ", " << event->y() << ")";
        auto color = image->GetPixelColor(event->x(), event->y());
        ss << "    (" << (int)color.R << ", " << (int)color.G << ", " << (int)color.B << ", " << (int)color.A << ")";
        ((QMainWindow*)p)->statusBar()->showMessage(ss.str().c_str());
    }

    lastMousePos = std::pair<int, int>(event->x(), event->y());

    if (toUpdate)
        update();
}

void ImageLabel::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);
    auto& frames = viewModel.GetFrames();

    for (size_t i = 0; i < frames.size(); i++)
    {
        Frame* frame = frames[i];
        bool selected = viewModel.IsFrameSelected(i);
        auto pen = painter.pen();

        if (selected)
            painter.setPen(QPen(Qt::yellow, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        else
        {
            if (_isIsolateFrameMode)
                continue;
        }

        painter.drawRect(frame->Left(), frame->Top(), frame->Width(), frame->Height());
        painter.fillRect(frame->Left(), frame->Top(), frame->Width(), frame->Height(),
                         QColor::fromRgb(100, 100, 100, 100));
        painter.setPen(pen);
    }
}

void ImageLabel::keyPressEvent(QKeyEvent* event)
{
    _isCtrlPressed = (Qt::Key::Key_Control == event->key());

    QWidget::keyPressEvent(event);
}

void ImageLabel::keyReleaseEvent(QKeyEvent* event)
{
    if (Qt::Key::Key_Control == event->key())
        _isCtrlPressed = false;

    QWidget::keyReleaseEvent(event);
}
