#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QCursor>
#include "image.h"

class ImageLabel : public QLabel
{
public:
    ImageLabel(QWidget* parent = nullptr);

    bool LoadImage(const Image* image);
    int ImageWidth() const;
    int ImageHeight() const;

private:
    // Event Handlers
    void OnStartBgColorPick(void* data);
    void OnRedrawImage(void* data);
    void OnIsolateSelectedFrame(void* data);

    QSize sizeHint() const override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    QImage* qimage;
    QCursor lastCursor;

    bool mouseButtons[3];
    std::pair<int, int> lastMousePos;
    bool _isBgColorSelectionMode;
    bool _isIsolateFrameMode;
    bool _isCtrlPressed;
};

#endif // IMAGELABEL_H
