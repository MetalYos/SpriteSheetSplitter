#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QCursor>
#include "image.h"

class ImageLabel : public QLabel
{
public:
    ImageLabel(QWidget* parent = nullptr);

    bool LoadImage(const std::string& filename);
    const Image& GetImage() const;

    int ImageWidth() const;
    int ImageHeight() const;

private:
    void OnStartBgColorPick(void* data);

    QSize sizeHint() const override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    Image* imageRaw;
    QImage* image;
    QCursor lastCursor;

    bool mouseButtons[3];
    std::pair<int, int> lastMousePos;
    bool _isBgColorSelectionMode;
};

#endif // IMAGELABEL_H
