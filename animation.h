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
        STOPPED,
        START
    };

public:
    Animation();
    ~Animation();

    bool IsLooped() const;
    void SetLooped(bool isLooped);

    AnimationStates GetState() const;

private:
    std::vector<Frame*> frames;
    bool isLooped;
    Frame* currentFrame;
    AnimationStates currentState;
};

#endif // ANIMATION_H
