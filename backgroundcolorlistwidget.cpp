#include "backgroundcolorlistwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

BackgroundColorListWidget::BackgroundColorListWidget(MainWindow* mainWindow, QWidget* parent) :
    QWidget(parent), _parent(parent), _mainWindow(mainWindow), _mainLayout(nullptr)
{
    InitGui();
}

void BackgroundColorListWidget::InitGui()
{
    _mainLayout = new QVBoxLayout();

    QLabel* label = new QLabel(tr("Background Color Picker:"), this);
    _mainLayout->addWidget(label);

    AddColorItemRow();
    AddColorItemRow();
    setLayout(_mainLayout);
}

int BackgroundColorListWidget::AddColorItemRow()
{
    QHBoxLayout* layout = new QHBoxLayout();

    RowWidget row;
    row.ColorButton = new QPushButton(this);
    row.ColorButton->setStyleSheet("background-color: white;");
    row.ColorButton->setMinimumWidth(50);
    row.AddButton = new QPushButton(tr("+"), this);
    row.RemoveButton = new QPushButton(tr("X"), this);

    layout->addWidget(row.ColorButton);
    layout->addWidget(row.AddButton);
    layout->addWidget(row.RemoveButton);

    _mainLayout->addLayout(layout);

    _rows.push_back(row);

    return _rows.size() - 1;
}
