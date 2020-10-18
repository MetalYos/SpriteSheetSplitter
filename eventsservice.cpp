#include "eventsservice.h"

int EventsService::_subId = 0;

EventsService::EventsService()
{

}

int EventsService::Subscribe(EventsTypes type, EventCallback callback)
{
    if (type == EventsTypes::None)
        return -1;

    _callbacks[(int)type][_subId] = callback;
    return _subId++;
}

void EventsService::UnSubscribe(EventsTypes type, int id)
{
    if (type == EventsTypes::None)
        return;

    _callbacks[(int)type].erase(id);
}

void EventsService::Publish(EventsTypes type, EventParams& data)
{
    if (type == EventsTypes::None)
        return;

    auto pairs = _callbacks[(int)type];
    for (auto pair : pairs)
    {
        pair.second(data);
    }
}
