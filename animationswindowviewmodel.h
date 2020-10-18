#ifndef ANIMATIONSWINDOWVIEWMODEL_H
#define ANIMATIONSWINDOWVIEWMODEL_H

#include "animation.h"
#include "image.h"
#include <vector>

class AnimationsWindowViewModel
{
public:
    static AnimationsWindowViewModel& Instance()
    {
        static AnimationsWindowViewModel instance;
        return instance;
    }
    ~AnimationsWindowViewModel();

    void SetImage(const Image* image);
    const Image* GetImage() const;

    // Animation Viewer Methods

    // Animations Methods
    void AddAnimation(Animation* anim);
    void RemoveAnimation(int index);
    void RemoveSelectedAnimation();
    void ClearAnimations();
    Animation* GetAnimation(int index);
    void SelectAnimation(int index);
    Animation* GetSelectedAnimation();
    int GetSelectedAnimationIndex();
    std::vector<Animation*>& GetAnimations();
    int GetAnimationIndex(Animation* animation) const;
    void DelesectAllAnimations();
    void SelectLastAnimation();
    bool IsAnimationSelected(int index) const;
    bool IsAnimationSelected(Animation* anim);

    // Animation Control Methods
    void SetAnimationState(Animation::AnimationStates state);
    Animation::AnimationStates GetAnimationState() const;
    void GoToStart();
    void GoToEnd();
    void GoToPreviousFrame();
    void GoToNextFrame();
    void Update();

private:
    AnimationsWindowViewModel();

private:
    std::vector<Animation*> animations;
    int selectedAnimIndex;
    const Image* image;
};

#endif // ANIMATIONSWINDOWVIEWMODEL_H
