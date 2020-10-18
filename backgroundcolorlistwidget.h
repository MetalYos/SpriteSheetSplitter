#ifndef BACKGROUNDCOLORLISTWIDGET_H
#define BACKGROUNDCOLORLISTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>
#include "frameswindowviewmodel.h"
#include "utils.h"
#include "eventsservice.h"

class BackgroundColorListWidget : public QWidget
{
    Q_OBJECT

public:
    BackgroundColorListWidget(QWidget* parent = nullptr);
    GraphicsUtils::PixelColor GetItemColor(int index);
    std::vector<GraphicsUtils::PixelColor> GetBgColors() const;

private:
    void InitGui();
    int AddColorItemRow();
    void RemoveColorItem(int index);

    void OnEndBgColorPick(EventParams& data);

private slots:
    void OnAddBgColorClicked();
    void OnRemoveBgColorClicked();
    void OnSelectBgColorClicked();

private:
    FramesWindowViewModel& viewModel;

    struct RowWidget
    {
        QHBoxLayout* Layout;
        QPushButton* ColorButton;
        QPushButton* AddButton;
        QPushButton* RemoveButton;

        void ClearRow()
        {
            if (ColorButton != nullptr)
                delete ColorButton;
            if (AddButton != nullptr)
                delete AddButton;
            if (RemoveButton != nullptr)
                delete RemoveButton;
            if (Layout != nullptr)
                delete Layout;
        }
    };

    QWidget* _parent;
    QVBoxLayout* _mainLayout;
    std::vector<RowWidget> _rows;
    int _pressedOnRowIndex;
};

#endif // BACKGROUNDCOLORLISTWIDGET_H
