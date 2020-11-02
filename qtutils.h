#ifndef QTUTILS_H
#define QTUTILS_H

#include <QWidget>
#include <QColor>
#include "utils.h"

namespace QtUtils
{
    void SetCursor(QWidget* widget, const char* path, int hotX = -1, int hotY = -1);
    GraphicsUtils::PixelColor QColorToPixelColor(const QColor& color);
    QColor UChar3ToQColor(unsigned char color[3]);
}

#endif // QTUTILS_H
