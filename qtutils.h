#ifndef QTUTILS_H
#define QTUTILS_H

#include <QWidget>
#include <QColor>
#include "utils.h"

namespace QtUtils
{
    void SetCursor(QWidget* widget, const char* path, int hotX = -1, int hotY = -1);
    GraphicsUtils::PixelColor QColorToPixelColor(const QColor& color);
}

#endif // QTUTILS_H
