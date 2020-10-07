#ifndef BACKGROUNDCOLORLISTWIDGET_H
#define BACKGROUNDCOLORLISTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <vector>
#include "mainwindow.h"
#include "utils.h"

class BackgroundColorListWidget : public QWidget
{
    Q_OBJECT

public:
    BackgroundColorListWidget(MainWindow* mainWindow, QWidget* parent = nullptr);
    int AddColorItemRow();
    void RemoveColorItem(int index);
    void SetColorInItem(int index, const GraphicsUtils::PixelColor& color);
    GraphicsUtils::PixelColor GetItemColor(int index);

private:
    void InitGui();

private:
    struct RowWidget
    {
        QPushButton* ColorButton;
        QPushButton* AddButton;
        QPushButton* RemoveButton;
    };

    QWidget* _parent;
    MainWindow* _mainWindow;
    QVBoxLayout* _mainLayout;
    std::vector<RowWidget> _rows;
};

#endif // BACKGROUNDCOLORLISTWIDGET_H
