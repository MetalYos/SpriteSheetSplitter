#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent), tabWidget(nullptr), buttonBox(nullptr),
      frameInitWidth(nullptr), frameInitHeight(nullptr),
      frameTolerance(nullptr), frameColorTolerance(nullptr),
      animationFps(nullptr)
{
    InitGui();

    setMinimumWidth(400);
    setMinimumHeight(400);
}

void SettingsDialog::InitGui()
{
    tabWidget = new QTabWidget();

    CreateFrameAlgoSettingsTab();
    CreateAnimsSettingsTab();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::OnOkButtonPressed);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::OnCancelButtonPressed);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Settings"));
}

void SettingsDialog::CreateFrameAlgoSettingsTab()
{
    QWidget* frameAlgoWidget = new QWidget(tabWidget);

    QFormLayout* frameMainLayout = new QFormLayout();

    frameInitWidth = new QSpinBox(frameAlgoWidget);
    frameInitWidth->setMinimum(1);
    frameInitWidth->setMaximum(10000);
    frameInitWidth->setValue(Settings::GetInt(Settings::Fields::FRAME_INIT_WIDTH));
    frameMainLayout->addRow(tr("Frame Initial Width"), frameInitWidth);

    frameInitHeight = new QSpinBox(frameAlgoWidget);
    frameInitHeight->setMinimum(1);
    frameInitHeight->setMaximum(10000);
    frameInitHeight->setValue(Settings::GetInt(Settings::Fields::FRAME_INIT_HEIGHT));
    frameMainLayout->addRow(tr("Frame Initial Height"), frameInitHeight);

    frameTolerance = new QSpinBox(frameAlgoWidget);
    frameTolerance->setMinimum(Settings::GetInt(Settings::Fields::FRAME_TOLERANCE_MIN));
    frameTolerance->setMaximum(Settings::GetInt(Settings::Fields::FRAME_TOLERANCE_MAX));
    frameTolerance->setValue(Settings::GetInt(Settings::Fields::FRAME_TOLERANCE_INIT));
    frameMainLayout->addRow(tr("Frame Tolerance"), frameTolerance);

    frameColorTolerance = new QSpinBox(frameAlgoWidget);
    frameColorTolerance->setMinimum(0);
    frameColorTolerance->setMaximum(50);
    frameColorTolerance->setValue(Settings::GetInt(Settings::Fields::FRAME_COLOR_TOLERANCE));
    frameMainLayout->addRow(tr("Background Color Tolerance"), frameColorTolerance);

    frameAlgoWidget->setLayout(frameMainLayout);

    tabWidget->addTab(frameAlgoWidget, tr("Frame Detection Settings"));
}

void SettingsDialog::CreateAnimsSettingsTab()
{
    QWidget* animWidget = new QWidget(tabWidget);

    QFormLayout* animMainLayout = new QFormLayout();

    animationFps = new QSpinBox(animWidget);
    animationFps->setMinimum(1);
    animationFps->setMaximum(1000);
    animationFps->setValue(Settings::GetInt(Settings::Fields::ANIMATION_FPS));
    animMainLayout->addRow(tr("Animation Frames Per Second (FPS)"), animationFps);

    animWidget->setLayout(animMainLayout);

    tabWidget->addTab(animWidget, tr("Animation Settings"));
}

void SettingsDialog::OnOkButtonPressed()
{
    close();
}

void SettingsDialog::OnCancelButtonPressed()
{
    close();
}
