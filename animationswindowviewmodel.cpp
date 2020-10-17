#include "animationswindowviewmodel.h"

AnimationsWindowViewModel::AnimationsWindowViewModel()
    : selectedAnimIndex(-1)
{
    Animation* tempAnimation = new Animation();
    AddAnimation(tempAnimation);
    SetAnimationState(Animation::AnimationStates::STOPPED);
}

AnimationsWindowViewModel::~AnimationsWindowViewModel()
{
    ClearAnimations();
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
