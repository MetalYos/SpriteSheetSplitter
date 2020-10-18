#include "animationwindow.h"

#include <QLabel>
#include <QDockWidget>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QFont>
#include <functional>
#include <sstream>
#include "eventsservice.h"
#include "settings.h"

AnimationWindow::AnimationWindow(QWidget *parent) :
    QMainWindow(parent),
    viewModel(AnimationsWindowViewModel::Instance()),
    animsList(nullptr), titleLabel(nullptr),
    centralWidgetScrollArea(nullptr),
    imageLabel(nullptr), qimage(nullptr)
{
    InitGui();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&AnimationWindow::OnTimeout));

    EventsService::Instance().Subscribe(EventsTypes::CreateAnimationPressed,
                                        std::bind(&AnimationWindow::OnCreateAnimationPressed, this, std::placeholders::_1));
    EventsService::Instance().Subscribe(EventsTypes::SpriteSheetLoaded,
                                        std::bind(&AnimationWindow::OnSpriteSheetLoaded, this, std::placeholders::_1));
}

void AnimationWindow::InitGui()
{
    setWindowFlags(Qt::Widget);

    CreateAnimationsDock();
    CreateTimelineDock();
    CreateCentralWidget();
}

void AnimationWindow::CreateAnimationsDock()
{
    QWidget* animsWidget = new QWidget(this);

    QDockWidget* animsDock = new QDockWidget(tr("Animations"), this);
    animsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QVBoxLayout* mainDockLayout = new QVBoxLayout();

    // Create list of Frames widget
    animsList = new QListWidget(animsDock);
    animsList->setMinimumHeight(300);
    mainDockLayout->addWidget(animsList);

    animsWidget->setLayout(mainDockLayout);
    animsDock->setWidget(animsWidget);
    addDockWidget(Qt::RightDockWidgetArea, animsDock);
}

void AnimationWindow::CreateTimelineDock()
{
    QDockWidget* timelineDock = new QDockWidget(tr("Timeline"), this);
    timelineDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    timelineDock->setMaximumHeight(100);
    timelineDock->setMinimumHeight(70);

    CreatePlayControlsWidget(timelineDock);

    addDockWidget(Qt::BottomDockWidgetArea, timelineDock);
}

void AnimationWindow::CreatePlayControlsWidget(QDockWidget* timelineDock)
{
    QWidget* playControlsWidget = new QWidget(this);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    layout->setAlignment(Qt::AlignHCenter);

    QPushButton* goToStartButton = new QPushButton("|<", playControlsWidget);
    goToStartButton->setMaximumWidth(50);
    connect(goToStartButton, &QPushButton::pressed, this, &AnimationWindow::OnGoToStartPressed);

    QPushButton* prevFrameButton = new QPushButton("<<", playControlsWidget);
    prevFrameButton->setMaximumWidth(50);
    connect(prevFrameButton, &QPushButton::pressed, this, &AnimationWindow::OnPrevFramePressed);

    playPauseButton = new QPushButton(">", playControlsWidget);
    playPauseButton->setMaximumWidth(50);
    connect(playPauseButton, &QPushButton::pressed, this, &AnimationWindow::OnPlayPausePressed);

    QPushButton* nextFrameButton = new QPushButton(">>", playControlsWidget);
    nextFrameButton->setMaximumWidth(50);
    connect(nextFrameButton, &QPushButton::pressed, this, &AnimationWindow::OnNextFramePressed);

    QPushButton* goToEndButton = new QPushButton(">|", playControlsWidget);
    goToEndButton->setMaximumWidth(50);
    connect(goToEndButton, &QPushButton::pressed, this, &AnimationWindow::OnGoToEndPressed);

    layout->addWidget(goToStartButton);
    layout->addWidget(prevFrameButton);
    layout->addWidget(playPauseButton);
    layout->addWidget(nextFrameButton);
    layout->addWidget(goToEndButton);

    playControlsWidget->setLayout(layout);
    timelineDock->setWidget(playControlsWidget);
}

void AnimationWindow::CreateCentralWidget()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* centralLayout = new QVBoxLayout();

    titleLabel = new QLabel(centralWidget);
    QFont sansFont("Helvetica [Cronyx]", 12);
    titleLabel->setFont(sansFont);
    titleLabel->setText(tr("No Animation selected..."));
    centralLayout->addWidget(titleLabel);

    centralWidgetScrollArea = new QScrollArea(centralWidget);
    imageLabel = new QLabel(centralWidgetScrollArea);
    imageLabel->setAlignment(Qt::AlignCenter);
    centralWidgetScrollArea->setAlignment(Qt::AlignCenter);
    centralWidgetScrollArea->setWidgetResizable(true);
    centralWidgetScrollArea->setWidget(imageLabel);
    centralLayout->addWidget(centralWidgetScrollArea);

    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);
}

bool AnimationWindow::SetFrameImage()
{
    auto frame = viewModel.GetSelectedAnimation()->GetCurrentFrame();
    auto image = viewModel.GetImage();

    if (image == nullptr || frame == nullptr)
        return false;

    if (image != nullptr)
    {
        int startIndex = frame->Top() * image->BytesPerLine() + frame->Left() * image->NumOfChannels();

        QImage* temp = new QImage(image->Pixels() + startIndex,
                           frame->Width(),
                           frame->Height(),
                           image->BytesPerLine(),
                           QImage::Format_ARGB32);

        if (qimage != nullptr)
            delete qimage;

        qimage = new QImage(temp->rgbSwapped());
        delete temp;

        QPixmap pixmap = QPixmap::fromImage(*qimage);
        pixmap.scaled(size(), Qt::KeepAspectRatio);
        imageLabel->setPixmap(pixmap);

        std::stringstream ss;
        ss << "Frame " << viewModel.GetSelectedAnimation()->GetCurrentFrameIndex() + 1 <<
              " out of " << viewModel.GetSelectedAnimation()->GetNumberOfFrames();
        titleLabel->setText(ss.str().c_str());

        return true;
    }
    return false;
}

void AnimationWindow::AddAnimationToList(int index)
{
    std::stringstream ss;
    ss << "Animation " << index + 1;
    animsList->addItem(ss.str().c_str());

    animsList->clearSelection();
    animsList->setCurrentRow(index);
}

void AnimationWindow::OnGoToStartPressed()
{
    viewModel.GoToStart();
    SetFrameImage();
}

void AnimationWindow::OnGoToEndPressed()
{
    viewModel.GoToEnd();
    SetFrameImage();
}

void AnimationWindow::OnPrevFramePressed()
{
    viewModel.GoToPreviousFrame();
    SetFrameImage();
}

void AnimationWindow::OnNextFramePressed()
{
    viewModel.GoToNextFrame();
    SetFrameImage();
}

void AnimationWindow::OnPlayPausePressed()
{
    if (viewModel.GetAnimationState() == Animation::AnimationStates::PAUSED ||
            viewModel.GetAnimationState() == Animation::AnimationStates::STOPPED)
    {
        viewModel.SetAnimationState(Animation::AnimationStates::PLAYING);
        playPauseButton->setText("||");

        int duration = 1000 * (1.0 / Settings::GetInt(Settings::Fields::ANIMATION_FPS));
        timer->start(duration);
    }
    else
    {
        viewModel.SetAnimationState(Animation::AnimationStates::PAUSED);
        playPauseButton->setText(">");

        timer->stop();
    }
}

void AnimationWindow::OnTimeout()
{
    viewModel.Update();
    SetFrameImage();
}

void AnimationWindow::OnSpriteSheetLoaded(EventParams& data)
{
    SpriteSheetLoadedParams& params = dynamic_cast<SpriteSheetLoadedParams&>(data);
    if (params.SpriteSheetImage == nullptr)
        return;

    viewModel.SetImage(params.SpriteSheetImage);
}

void AnimationWindow::OnCreateAnimationPressed(EventParams& data)
{
    CreateAnimationPressedParams& params = dynamic_cast<CreateAnimationPressedParams&>(data);
    Animation* anim = new Animation(params.Frames);
    viewModel.AddAnimation(anim);

    AddAnimationToList(viewModel.GetSelectedAnimationIndex());
    SetFrameImage();
}
