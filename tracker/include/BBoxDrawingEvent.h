#include "Event.h"
#include "Box.h"
#include "common.h"

namespace gryllidae
{
    class BBoxDrawingEvent: public Event
    {
    private:
    public:
        std::vector<BBox> BBoxes;
    public:
        // static EventType GetStaticType() { return EventType::type; }
        EventType GetEventType() const override { return EventType::DrawBBox;}
        BBoxDrawingEvent(std::vector<BBox> bboxes) {BBoxes.assign(bboxes.begin(),bboxes.end());}
    };
} // namespace gryllidae
