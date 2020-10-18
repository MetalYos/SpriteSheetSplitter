#include "animationswindowviewmodel.h"

AnimationsWindowViewModel::AnimationsWindowViewModel()
    : selectedAnimIndex(-1)
{

}

AnimationsWindowViewModel::~AnimationsWindowViewModel()
{
    ClearAnimations();
}

void AnimationsWindowViewModel::SetImage(const Image* image)
{
    this->image = image;
}

const Image* AnimationsWindowViewModel::GetImage() const
{
    return image;
}

void AnimationsWindowViewModel::AddAnimation(Animation* anim)
{
    animations.push_back(anim);
    SelectLastAnimation();
}

void AnimationsWindowViewModel::RemoveAnimation(int index)
{
    int i = 0;
    for (auto it = animations.begin(); it != animations.end(); ++it)
    {
        if (i == index)
        {
            animations.erase(it);
            break;
        }
        i++;
    }
}

void AnimationsWindowViewModel::RemoveSelectedAnimation()
{
    if (selectedAnimIndex < 0 || selectedAnimIndex >= (int)animations.size())
        return;

    animations.erase(animations.begin() + selectedAnimIndex);
    selectedAnimIndex = (selectedAnimIndex >= (int)animations.size()) ? animations.size() - 1 : selectedAnimIndex;
}

void AnimationsWindowViewModel::ClearAnimations()
{
    while (animations.size() > 0)
    {
        Animation* anim = animations.back();
        animations.pop_back();
        delete anim;
    }
    selectedAnimIndex = -1;
}

Animation* AnimationsWindowViewModel::GetAnimation(int index)
{
    if (index < 0 || index >= (int)animations.size())
        return nullptr;

    return *(animations.begin() + index);
}

void AnimationsWindowViewModel::SelectAnimation(int index)
{
    if (index < 0 || index >= (int)animations.size())
        return;
    selectedAnimIndex = index;
}

Animation* AnimationsWindowViewModel::GetSelectedAnimation()
{
    return GetAnimation(selectedAnimIndex);
}

int AnimationsWindowViewModel::GetSelectedAnimationIndex()
{
    return selectedAnimIndex;
}

std::vector<Animation*>& AnimationsWindowViewModel::GetAnimations()
{
    return animations;
}

int AnimationsWindowViewModel::GetAnimationIndex(Animation* animation) const
{
    for (size_t i = 0; i < animations.size(); i++)
    {
        if (animation == animations[i])
            return i;
    }
    return -1;
}

void AnimationsWindowViewModel::DelesectAllAnimations()
{
    selectedAnimIndex = -1;
}

void AnimationsWindowViewModel::SelectLastAnimation()
{
    selectedAnimIndex = animations.size() - 1;
}

bool AnimationsWindowViewModel::IsAnimationSelected(int index) const
{
    return (selectedAnimIndex == index);
}

bool AnimationsWindowViewModel::IsAnimationSelected(Animation* anim)
{
    return (GetAnimationIndex(anim) == selectedAnimIndex);
}

void AnimationsWindowViewModel::SetAnimationState(Animation::AnimationStates state)
{
    if (selectedAnimIndex < 0)
        return;
    animations[selectedAnimIndex]->SetState(state);
}

Animation::AnimationStates AnimationsWindowViewModel::GetAnimationState() const
{
    if (selectedAnimIndex < 0)
        return Animation::AnimationStates::STOPPED;
    else
        return animations[selectedAnimIndex]->GetState();
}

void AnimationsWindowViewModel::GoToStart()
{
    GetSelectedAnimation()->GoToStart();
}

void AnimationsWindowViewModel::GoToEnd()
{
    GetSelectedAnimation()->GoToEnd();
}

void AnimationsWindowViewModel::GoToPreviousFrame()
{
    GetSelectedAnimation()->GoToPreviousFrame();
}

void AnimationsWindowViewModel::GoToNextFrame()
{
    GetSelectedAnimation()->GoToNextFrame();
}

void AnimationsWindowViewModel::Update()
{
    auto anim = GetSelectedAnimation();
    if (anim == nullptr)
        return;

    if (anim->GetState() == Animation::AnimationStates::PLAYING)
    {
        anim->GoToNextFrame();
    }
}
