#pragma once

namespace gryllidae
{
    enum class EventType{
        None=0,
        DrawBBox=1
    };


    class Event
    {
    private:
        /* data */
    public:
        virtual ~Event()=default;
        bool Handled=false;

        virtual EventType GetEventType() const = 0; 
    };


} // namespace gryllidae
