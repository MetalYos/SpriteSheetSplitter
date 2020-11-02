#include "qtutils.h"

#include <QPixmap>
#include <QCursor>

namespace QtUtils
{
    void SetCursor(QWidget* widget, const char* path, int hotX, int hotY)
    {
        QPixmap cursorPixmap(path, "PNG");
        QCursor cursor(cursorPixmap, hotX, hotY);
        widget->setCursor(cursor);
    }

    GraphicsUtils::PixelColor QColorToPixelColor(const QColor& color)
    {
        GraphicsUtils::PixelColor pColor;
        pColor.R = color.red();
        pColor.G = color.green();
        pColor.B = color.blue();
        pColor.A = color.alpha();
        return pColor;
    }

    QColor UChar3ToQColor(unsigned char color[3])
    {
        return QColor(color[0], color[1], color[2]);
    }
}
