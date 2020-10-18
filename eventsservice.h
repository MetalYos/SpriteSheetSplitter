#ifndef EVENTSSERVICE_H
#define EVENTSSERVICE_H

#include <map>
#include <functional>
#include <vector>
#include "frame.h"
#include "image.h"
#include "utils.h"

enum EventsTypes
{
    CreateAnimationPressed,
    SpriteSheetLoaded,
    StartBgColorPick,
    EndBgColorPick,
    RedrawImage,
    SelectedFrameOnImage,
    SelectedFrameInList,
    IsolateSelectedFrame,
    None
};

struct EventParams
{
    virtual ~EventParams() = default;
};

struct CreateAnimationPressedParams : public EventParams
{
    std::vector<Frame*> Frames;
};

struct SpriteSheetLoadedParams : public EventParams
{
    const Image* SpriteSheetImage;
    std::vector<Frame*> Frames;
};

struct StartBgColorPickParams : public EventParams {};

struct EndBgColorPickParams : public EventParams
{
    GraphicsUtils::PixelColor Color;
};

struct RedrawImageParams : public EventParams {};

struct SelectedFrameOnImageParams : public EventParams
{
    int FrameIndex;
    bool IsCtrlPressed;
};

struct SelectedFrameInListParams : public EventParams
{
    int FrameIndex;
};

struct IsolateSelectedFrameParams : public EventParams
{
    bool Isolate;
};

typedef std::function<void(EventParams&)> EventCallback;

class EventsService
{
public:
    static EventsService& Instance()
    {
        static EventsService instance;
        return instance;
    }

    int Subscribe(EventsTypes type, EventCallback callback);
    void UnSubscribe(EventsTypes type, int id);
    void Publish(EventsTypes type, EventParams& data);

private:
    EventsService();

private:
    static int _subId;
    std::map<int, EventCallback> _callbacks[(int)EventsTypes::None];
};

#endif // EVENTSSERVICE_H
