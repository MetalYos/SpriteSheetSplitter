#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include "image.h"

class ImageLabel : public QLabel
{
public:
    ImageLabel(QWidget* parent);

    bool LoadImage(const std::string& filename);
    const Image& GetImage() const;

    int ImageWidth() const;
    int ImageHeight() const;

private:
    QSize sizeHint() const override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    Image* imageRaw;
    QImage* image;

    bool mouseButtons[3];
    std::pair<int, int> lastMousePos;
};

#endif // IMAGELABEL_H
