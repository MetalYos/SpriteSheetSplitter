#ifndef EVENTSSERVICE_H
#define EVENTSSERVICE_H

#include <map>
#include <functional>

typedef std::function<void(void*)> EventCallback;

enum EventsTypes
{
    SpriteSheetLoaded,
    StartBgColorPick,
    EndBgColorPick,
    RedrawImage,
    SelectedFrameOnImage,
    SelectedFrameInList,
    IsolateSelectedFrame,
    None
};

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
    void Publish(EventsTypes type, void* data = nullptr);

private:
    EventsService();

private:
    static int _subId;
    std::map<int, EventCallback> _callbacks[(int)EventsTypes::None];
};

#endif // EVENTSSERVICE_H
