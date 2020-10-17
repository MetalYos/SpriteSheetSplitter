#ifndef ANIMATION_H
#define ANIMATION_H

#include "frame.h"
#include <vector>

class Animation
{
public:
    enum AnimationStates
    {
        PLAYING,
        PAUSED,
        STOPPED
    };

public:
    Animation();
    Animation(const std::vector<Frame*>& frames);
    ~Animation();

    bool IsLooped() const;
    void SetLooped(bool isLooped);

    AnimationStates GetState() const;
    void SetState(AnimationStates state);

private:
    std::vector<Frame*> frames;
    bool isLooped;
    Frame* currentFrame;
    AnimationStates currentState;
};

#endif // ANIMATION_H
