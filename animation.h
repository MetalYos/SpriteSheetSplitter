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

    int GetCurrentFrameIndex() const;
    Frame* GetCurrentFrame() const;
    size_t GetNumberOfFrames() const;

    void GoToStart();
    void GoToEnd();
    void GoToNextFrame();
    void GoToPreviousFrame();

private:
    std::vector<Frame*> frames;
    bool isLooped;
    int currentFrameIndex;
    AnimationStates currentState;
};

#endif // ANIMATION_H
