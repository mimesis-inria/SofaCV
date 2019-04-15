#include "BaseGrabber.h"

void sofacv::acquisition::BaseGrabber::handleEvent(sofa::core::objectmodel::Event *e)
{
    if (sofa::simulation::AnimateBeginEvent::checkEventType(e))
    {
        update(); /// always call update() for grabbers. It's
        /// decided internally whether or not they should do something
        return;
    }
    ImplicitDataEngine::handleEvent(e);
}
