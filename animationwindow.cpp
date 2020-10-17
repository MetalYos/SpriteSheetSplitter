#include "animationwindow.h"

#include <QLabel>
#include <QDockWidget>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <functional>
#include "eventsservice.h"

AnimationWindow::AnimationWindow(QWidget *parent) :
    QMainWindow(parent),
    viewModel(AnimationsWindowViewModel::Instance()),
    animsList(nullptr), centralWidgetScrollArea(nullptr),
    imageLabel(nullptr), qimage(nullptr),
    image(nullptr)
{
    InitGui();

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
    QHBoxLayout* centralLayout = new QHBoxLayout();

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

bool AnimationWindow::LoadImage(const Image* image, Frame* frame)
{
    if (image == nullptr)
        return false;

    if (image != nullptr)
    {
        int startIndex = frame->Top() * image->BytesPerLine() + frame->Left();

        QImage* temp = new QImage(image->Pixels() + startIndex - 2,
                           frame->Width(),
                           frame->Height(),
                           image->BytesPerLine(),
                           QImage::Format_ARGB32);
        qimage = new QImage(temp->rgbSwapped());
        delete temp;
        QPixmap pixmap = QPixmap::fromImage(*qimage);
        pixmap.scaled(size(), Qt::KeepAspectRatio);
        imageLabel->setPixmap(pixmap);
        imageLabel->setStyleSheet("border: 1px solid black;");

        return true;
    }
    return false;
}

void AnimationWindow::OnGoToStartPressed()
{

}

void AnimationWindow::OnGoToEndPressed()
{

}

void AnimationWindow::OnPrevFramePressed()
{

}

void AnimationWindow::OnNextFramePressed()
{

}

void AnimationWindow::OnPlayPausePressed()
{
    if (viewModel.GetAnimationState() == Animation::AnimationStates::PLAYING ||
            viewModel.GetAnimationState() == Animation::AnimationStates::STOPPED)
    {
        viewModel.SetAnimationState(Animation::AnimationStates::PAUSED);
        playPauseButton->setText("||");
    }
    else
    {
        viewModel.SetAnimationState(Animation::AnimationStates::PLAYING);
        playPauseButton->setText(">");
    }
}

void AnimationWindow::OnSpriteSheetLoaded(void* data)
{
    if (data == nullptr)
        return;
    auto input = static_cast<std::pair<const Image*, std::vector<Frame*>>*>(data);
    image = input->first;
}

void AnimationWindow::OnCreateAnimationPressed(void* data)
{
    if (data == nullptr)
        return;

    std::vector<Frame*>* frames = static_cast<std::vector<Frame*>*>(data);
    Animation* anim = new Animation(*frames);

    viewModel.AddAnimation(anim);

    if (frames->size() > 0)
        LoadImage(image, (*frames)[0]);
}
