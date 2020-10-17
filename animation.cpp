#include "animation.h"

Animation::Animation()
    : isLooped(false), currentFrame(nullptr),
      currentState(AnimationStates::STOPPED)
{

}

Animation::Animation(const std::vector<Frame*>& frames)
    : frames(frames), isLooped(false), currentFrame(nullptr),
      currentState(AnimationStates::STOPPED)
{

}

Animation::~Animation()
{

}

bool Animation::IsLooped() const
{
    return isLooped;
}

void Animation::SetLooped(bool isLooped)
{
    isLooped = true;
}

Animation::AnimationStates Animation::GetState() const
{
    return currentState;
}

void Animation::SetState(AnimationStates state)
{
    currentState = state;
}
