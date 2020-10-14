#include "framesdock.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <sstream>
#include <functional>
#include "backgroundcolorlistwidget.h"
#include "settings.h"
#include "mainwindowviewmodel.h"
#include "eventsservice.h"
#include "framedetection.h"

FramesDock::FramesDock(const QString& title, QWidget* parent)
    : QDockWidget(title, parent),
      tolerance(Settings::GetInt(Settings::Fields::FRAME_TOLERANCE_INIT))
{
    InitGui();
    MainWindowViewModel::Instance().SetFrameDetectionParameters(bgColorListWidget->GetBgColors(), tolerance);

    // Subscribe to relevant events
    EventsService::Instance().Subscribe(EventsTypes::SpriteSheetLoaded,
                                        std::bind(&FramesDock::OnSpriteSheetLoaded, this, std::placeholders::_1));
    EventsService::Instance().Subscribe(EventsTypes::SelectedFrameOnImage,
                                        std::bind(&FramesDock::OnSelectedFrameOnImage, this, std::placeholders::_1));
}

void FramesDock::SelectFrame(int index)
{
    if (index < 0 || index >= framesList->count())
        return;
    framesList->item(index)->setSelected(true);
}

void FramesDock::InitGui()
{
    CreateFramesWidget();

    setDisabled(true);
}

void FramesDock::CreateFramesWidget()
{
    // Create Frames dock inner main widget
    QWidget* framesWidget = new QWidget(this);

    QVBoxLayout* mainDockLayout = new QVBoxLayout();

    // Create list of Frames widget
    framesList = new QListWidget(this);
    framesList->setMinimumHeight(300);
    framesList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(framesList, &QListWidget::itemSelectionChanged, this, &FramesDock::OnFrameSelectionChanged);
    mainDockLayout->addWidget(framesList);

    // Create list controls widget (add and remove buttons)
    QHBoxLayout* framesListButtonsLayout = new QHBoxLayout();

    QPushButton* addButton = new QPushButton(tr("Add"), framesWidget);
    connect(addButton, &QPushButton::clicked, this, &FramesDock::OnAddFrameButtonClicked);

    QPushButton* removeButton = new QPushButton(tr("Remove"), framesWidget);
    connect(removeButton, &QPushButton::clicked, this, &FramesDock::OnRemoveFrameButtonClicked);

    QPushButton* clearButton = new QPushButton(tr("Clear"), framesWidget);
    connect(clearButton, &QPushButton::clicked, this, &FramesDock::OnClearFramesButtonClicked);

    framesListButtonsLayout->addWidget(addButton);
    framesListButtonsLayout->addWidget(removeButton);
    framesListButtonsLayout->addWidget(clearButton);
    mainDockLayout->addLayout(framesListButtonsLayout);

    // Create the calculate frames widget
    CreateCalcFrameWidget(framesWidget, mainDockLayout);

    framesWidget->setLayout(mainDockLayout);
    setWidget(framesWidget);
}

void FramesDock::CreateCalcFrameWidget(QWidget* framesWidget, QVBoxLayout* framesDockLayout)
{
    QGroupBox* calcFrameWidget = new QGroupBox(tr("Calculate Frame/s"), framesWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout();

    bgColorListWidget = new BackgroundColorListWidget(calcFrameWidget);
    mainLayout->addWidget(bgColorListWidget);

    QFormLayout* formLayout = new QFormLayout();
    QSpinBox* toleranceSpinBox = new QSpinBox(calcFrameWidget);
    toleranceSpinBox->setMinimum(Settings::GetInt(Settings::Fields::FRAME_TOLERANCE_MIN));
    toleranceSpinBox->setMaximum(Settings::GetInt(Settings::Fields::FRAME_TOLERANCE_MAX));
    toleranceSpinBox->setValue(tolerance);
    connect(toleranceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FramesDock::OnToleranceValueChanged);
    formLayout->addRow(tr("Tolerance: "), toleranceSpinBox);
    mainLayout->addLayout(formLayout);

    // Add Adaptive Step checkbox
    QCheckBox* adaptiveStepCheckBox = new QCheckBox(calcFrameWidget);
    adaptiveStepCheckBox->setChecked(Settings::GetBool(Settings::Fields::FRAME_DETECTION_ALGO_USE_ADAPTIVE_STEP));
    adaptiveStepCheckBox->setText(tr("Use Adaptive Steps?"));
    connect(adaptiveStepCheckBox, &QCheckBox::stateChanged,
            this, &FramesDock::OnUseAdaptiveStepCheckboxStateChanged);
    mainLayout->addWidget(adaptiveStepCheckBox);

    // Add Steps Widget
    stepsWidget = new QWidget(calcFrameWidget);
    QFormLayout* stepsForm = new QFormLayout();
    QSpinBox* stepxSpinBox = new QSpinBox(stepsWidget);
    stepxSpinBox->setMinimum(1);
    stepxSpinBox->setMaximum(10000);
    stepxSpinBox->setValue(Settings::GetInt(Settings::Fields::FRAME_DETECTION_ALGO_STEP));
    stepsForm->addRow(tr("Steps X: "), stepxSpinBox);

    QSpinBox* stepySpinBox = new QSpinBox(stepsWidget);
    stepySpinBox->setMinimum(1);
    stepySpinBox->setMaximum(10000);
    stepySpinBox->setValue(Settings::GetInt(Settings::Fields::FRAME_DETECTION_ALGO_STEP));
    stepsForm->addRow(tr("Steps Y: "), stepySpinBox);

    stepsWidget->setLayout(stepsForm);

    if (Settings::GetBool(Settings::Fields::FRAME_DETECTION_ALGO_USE_ADAPTIVE_STEP))
        stepsWidget->setVisible(false);

    mainLayout->addWidget(stepsWidget);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    QPushButton* calcFrameButton = new QPushButton(tr("Calculate Frame"), calcFrameWidget);
    connect(calcFrameButton, &QPushButton::clicked, this, &FramesDock::OnCalculateFrameButtonClicked);

    QPushButton* calcAllFramesButton = new QPushButton(tr("Calculate All Frames"), calcFrameWidget);
    connect(calcAllFramesButton, &QPushButton::clicked, this, &FramesDock::OnCalculateAllFramesButtonClicked);

    buttonsLayout->addWidget(calcFrameButton);
    buttonsLayout->addWidget(calcAllFramesButton);

    mainLayout->addLayout(buttonsLayout);

    calcFrameWidget->setLayout(mainLayout);
    framesDockLayout->addWidget(calcFrameWidget);
}

void FramesDock::AddFrameToList(int index)
{
    // Add to frames list and select it
    std::stringstream ss;
    ss << "Frame " << index;
    framesList->addItem(ss.str().c_str());

    framesList->clearSelection();
    framesList->setCurrentRow(framesList->count() - 1);
}

void FramesDock::ClearFramesList()
{
    MainWindowViewModel::Instance().ClearFrames();
    framesList->clear();
}

void FramesDock::OnFrameSelectionChanged()
{
    auto selectedFrames = framesList->selectedItems();
    if (selectedFrames.size() == 0)
    {
        MainWindowViewModel::Instance().DeselectAllFrames();
        return;
    }

    std::vector<int> selectedIndices;
    for (auto selectedFrame : selectedFrames)
    {
        int frameIndex = framesList->row(selectedFrame);
        selectedIndices.push_back(frameIndex);
        EventsService::Instance().Publish(EventsTypes::SelectedFrameInList, &frameIndex);
    }
    MainWindowViewModel::Instance().SetSelectedFrames(selectedIndices);
    EventsService::Instance().Publish(EventsTypes::RedrawImage);
}

void FramesDock::OnAddFrameButtonClicked()
{
    auto image = MainWindowViewModel::Instance().GetImage();
    if (image == nullptr)
        return;

    int frameInitWidth = Settings::GetInt(Settings::Fields::FRAME_INIT_WIDTH);
    int frameInitHeight = Settings::GetInt(Settings::Fields::FRAME_INIT_HEIGHT);
    int left = image->Width() / 2 - frameInitWidth / 2;
    int top = image->Height() / 2 - frameInitHeight / 2;

    Frame* frame = new Frame(top, left, frameInitWidth, frameInitHeight);

    MainWindowViewModel::Instance().AddFrame(frame);
    AddFrameToList(MainWindowViewModel::Instance().GetFrames().size());
}

void FramesDock::OnRemoveFrameButtonClicked()
{
    auto selectedFrames = MainWindowViewModel::Instance().GetSelectedFrames();

    // Remove the selected frame from the list
    auto items = framesList->selectedItems();
    for (int i = 0; i < items.count(); i++)
    {
        auto item = items[i];
        delete framesList->takeItem(framesList->row(item));
    }

    // Remove the selected frame
    MainWindowViewModel::Instance().RemoveSelectedFrames();

    if (framesList->count() > 0)
        framesList->setCurrentRow(framesList->count() - 1);
}

void FramesDock::OnClearFramesButtonClicked()
{
    // Clear frames List widget
    framesList->clear();

    // Clear frames list
    MainWindowViewModel::Instance().ClearFrames();

    EventsService::Instance().Publish(EventsTypes::RedrawImage);
}

void FramesDock::OnUseAdaptiveStepCheckboxStateChanged(int state)
{
    bool isChecked = (state == Qt::Checked);
    Settings::Set(Settings::Fields::FRAME_DETECTION_ALGO_USE_ADAPTIVE_STEP, isChecked);
    stepsWidget->setVisible(!isChecked);
}

// TODO: move relevant parts to MainWindowViewModel
void FramesDock::OnCalculateFrameButtonClicked()
{

    MainWindowViewModel::Instance().DetectSelectedFrames();
    EventsService::Instance().Publish(EventsTypes::RedrawImage);
}

void FramesDock::OnCalculateAllFramesButtonClicked()
{
    MainWindowViewModel::Instance().DetectAllFrames();
    for (size_t i = 0; i < MainWindowViewModel::Instance().GetFrames().size(); i++)
        AddFrameToList(i + 1);
}

void FramesDock::OnSpriteSheetLoaded(void* data)
{
    // Clear frames List widget
    framesList->clear();

    // Load frames into list if available
    if (data != nullptr)
    {
        auto frames = static_cast<std::vector<Frame*>*>(data);
        for (size_t i = 0; i < frames->size(); i++)
            AddFrameToList(i + 1);
    }

    // Enable dock
    setDisabled(false);

    EventsService::Instance().Publish(EventsTypes::RedrawImage);
}

void FramesDock::OnSelectedFrameOnImage(void* data)
{
    if (data == nullptr)
    {
        ClearFramesList();
    }
    else
    {
        std::pair<int, bool>* index = static_cast<std::pair<int, bool>*>(data);
        if (index->second)
            framesList->clearSelection();
        SelectFrame(index->first);
    }
}

void FramesDock::OnToleranceValueChanged(int i)
{
    tolerance = i;
    MainWindowViewModel::Instance().SetFrameDetectionParameters(i);
}
