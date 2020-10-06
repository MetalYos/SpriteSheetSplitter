#include "imagelabel.h"
#include <QMainWindow>
#include <QStatusBar>
#include <QPainter>
#include <QPen>
#include <sstream>
#include "mainwindowviewmodel.h"
#include "constants.h"

ImageLabel::ImageLabel(QWidget* parent) :
    QLabel(parent), imageRaw(nullptr), image(nullptr)
{
    mouseButtons[0] = false;
    mouseButtons[1] = false;
    mouseButtons[2] = false;

    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

bool ImageLabel::LoadImage(const std::string& filename)
{
    if (imageRaw != nullptr)
        delete imageRaw;

    imageRaw = Image::LoadImage(filename);
    if (imageRaw != nullptr)
    {
        QImage* temp = new QImage(imageRaw->Pixels(),
                           imageRaw->Width(),
                           imageRaw->Height(),
                           imageRaw->BytesPerLine(),
                           QImage::Format_ARGB32);
        image = new QImage(temp->rgbSwapped());
        delete temp;
        QPixmap pixmap = QPixmap::fromImage(*image);
        pixmap.scaled(size(), Qt::KeepAspectRatio);
        setPixmap(pixmap);

        lastMousePos = std::pair<int, int>(width() / 2, height() / 2);

        setStyleSheet("border: 1px solid black;");

        return true;
    }
    return false;
}

const Image& ImageLabel::GetImage() const
{
    return *imageRaw;
}

QSize ImageLabel::sizeHint() const
{
    return QSize(ImageWidth(), ImageHeight());
}

void ImageLabel::mousePressEvent(QMouseEvent* event)
{
    mouseButtons[Constants::LEFT_MOUSE_BUTTON] = (event->button() == Qt::LeftButton);
    mouseButtons[Constants::RIGHT_MOUSE_BUTTON] = (event->button() == Qt::RightButton);
    mouseButtons[Constants::MIDDLE_MOUSE_BUTTON] = (event->button() == Qt::MiddleButton);

    if (mouseButtons[Constants::LEFT_MOUSE_BUTTON])
    {
        lastMousePos = std::pair<int, int>(event->x(), event->y());
        bool nonSelected = true;
        for (int i = MainWindowViewModel::Instance().GetFrames().size() - 1; i >= 0; i--)
        {
            Frame* frame = MainWindowViewModel::Instance().GetFrame(i);
            if (frame != nullptr && frame->ContainesPoint(event->x(), event->y()))
            {
                MainWindowViewModel::Instance().SelectFrame(i);
                nonSelected = false;
                break;
            }
        }

        if (nonSelected)
        {
            MainWindowViewModel::Instance().DeselectAllFrames();
        }

        update();
    }
}

void ImageLabel::mouseReleaseEvent(QMouseEvent* event)
{
    mouseButtons[Constants::LEFT_MOUSE_BUTTON] = !(event->button() == Qt::LeftButton);
    mouseButtons[Constants::RIGHT_MOUSE_BUTTON] = !(event->button() == Qt::RightButton);
    mouseButtons[Constants::MIDDLE_MOUSE_BUTTON] = !(event->button() == Qt::MiddleButton);
}

void ImageLabel::mouseMoveEvent(QMouseEvent* event)
{
    if (imageRaw == nullptr)
        return;

    setCursor(Qt::ArrowCursor);
    bool toUpdate = false;

    for (auto frame : MainWindowViewModel::Instance().GetFrames())
    {
        if (frame->ContainesPoint(event->x(), event->y()))
        {
            if (frame == MainWindowViewModel::Instance().GetSelectedFrame())
            {
                if (mouseButtons[Constants::LEFT_MOUSE_BUTTON])
                {
                    frame->Move(event->x() - lastMousePos.first, event->y() - lastMousePos.second,
                            ImageWidth(), ImageHeight());
                    setCursor(Qt::DragMoveCursor);
                    toUpdate = true;
                }
            }
            else
            {
                setCursor(Qt::PointingHandCursor);
            }
        }
    }

    if (event->x() >= 0 && event->x() <= ImageWidth() &&
        event->y() >= 0 && event->y() <= ImageHeight())
    {

        // Get Main window widget
        QWidget* p = parentWidget();
        while (p->parentWidget() != nullptr)
            p = p->parentWidget();

        // Show coordinates and color in status bar
        std::stringstream ss;
        ss << "(" << event->x() << ", " << event->y() << ")";
        auto color = imageRaw->GetPixelColor(event->x(), event->y());
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
    auto& frames = MainWindowViewModel::Instance().GetFrames();

    for (size_t i = 0; i < frames.size(); i++)
    {
        Frame* frame = frames[i];
        bool selected = ((int)i == MainWindowViewModel::Instance().GetSelectedFrameIndex());
        auto pen = painter.pen();

        if (selected)
            painter.setPen(QPen(Qt::yellow, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        painter.drawRect(frame->Left(), frame->Top(), frame->Width(), frame->Height());
        painter.fillRect(frame->Left(), frame->Top(), frame->Width(), frame->Height(),
                         QColor::fromRgb(100, 100, 100, 100));
        painter.setPen(pen);
    }
}

int ImageLabel::ImageWidth() const
{
    if (!imageRaw)
        return 0;

    return imageRaw->Width();
}

int ImageLabel::ImageHeight() const
{
    if (!imageRaw)
        return 0;

    return imageRaw->Height();
}
