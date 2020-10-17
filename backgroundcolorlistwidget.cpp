#include "backgroundcolorlistwidget.h"

#include <QLabel>
#include <sstream>
#include "qtutils.h"
#include "eventsservice.h"

BackgroundColorListWidget::BackgroundColorListWidget(QWidget* parent) :
    QWidget(parent),
    viewModel(FramesWindowViewModel::Instance()),
    _parent(parent),
    _mainLayout(nullptr),
    _pressedOnRowIndex(0)
{
    InitGui();

    EventsService::Instance().Subscribe(EventsTypes::EndBgColorPick,
                             std::bind(&BackgroundColorListWidget::OnEndBgColorPick, this, std::placeholders::_1));
}

void BackgroundColorListWidget::InitGui()
{
    _mainLayout = new QVBoxLayout();

    QLabel* label = new QLabel(tr("Background Color Picker:"), this);
    _mainLayout->addWidget(label);

    AddColorItemRow();
    setLayout(_mainLayout);
}

void BackgroundColorListWidget::OnEndBgColorPick(void* data)
{
    if (data == nullptr)
        return;

    GraphicsUtils::PixelColor* bgColor = static_cast<GraphicsUtils::PixelColor*>(data);
    std::stringstream ss;
    ss << "background-color: rgba(" << (int)bgColor->R << "," <<
          (int)bgColor->G << "," <<
          (int)bgColor->B << "," <<
          (int)bgColor->A << ");";
    _rows[_pressedOnRowIndex].ColorButton->setStyleSheet(ss.str().c_str());

    viewModel.SetFrameDetectionParameters(GetBgColors());
}

int BackgroundColorListWidget::AddColorItemRow()
{
    RowWidget row;
    row.Layout = new QHBoxLayout();
    row.ColorButton = new QPushButton(this);
    row.ColorButton->setStyleSheet("background-color: white;");
    row.ColorButton->setMinimumWidth(50);
    connect(row.ColorButton, &QPushButton::clicked, this, &BackgroundColorListWidget::OnSelectBgColorClicked);
    row.Layout->addWidget(row.ColorButton);

    row.AddButton = new QPushButton(tr("+"), this);
    connect(row.AddButton, &QPushButton::clicked, this, &BackgroundColorListWidget::OnAddBgColorClicked);
    row.Layout->addWidget(row.AddButton);

    if (_rows.size() > 0)
    {
        row.RemoveButton = new QPushButton(tr("X"), this);
        connect(row.RemoveButton, &QPushButton::clicked, this, &BackgroundColorListWidget::OnRemoveBgColorClicked);
        row.Layout->addWidget(row.RemoveButton);
    }

    _mainLayout->addLayout(row.Layout);

    _rows.push_back(row);

    return _rows.size() - 1;
}

std::vector<GraphicsUtils::PixelColor> BackgroundColorListWidget::GetBgColors() const
{
    std::vector<GraphicsUtils::PixelColor> colors;
    for (auto row : _rows)
    {
        auto color = row.ColorButton->palette().color(QWidget::backgroundRole());
        colors.push_back(QtUtils::QColorToPixelColor(color));
    }
    return colors;
}

void BackgroundColorListWidget::OnAddBgColorClicked()
{
    AddColorItemRow();
}

void BackgroundColorListWidget::OnRemoveBgColorClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button != nullptr)
    {
        for (auto it = _rows.begin(); it != _rows.end(); ++it)
        {
            auto row = *it;
            if (row.RemoveButton == button)
            {
                row.ClearRow();
                _rows.erase(it);
                break;
            }
        }
    }
}

void BackgroundColorListWidget::OnSelectBgColorClicked()
{
    EventsService::Instance().Publish(EventsTypes::StartBgColorPick, nullptr);

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button != nullptr)
    {
        for (size_t i = 0; i < _rows.size(); i++)
        {
            if (_rows[i].ColorButton == button)
            {
                _pressedOnRowIndex = i;
                break;
            }
        }
    }
}

