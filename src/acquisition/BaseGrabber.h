#ifndef SOFACV_ACQUISITION_BASEGRABBER_H
#define SOFACV_ACQUISITION_BASEGRABBER_H

#include "SofaCVPlugin.h"
#include "ImplicitDataEngine.h"

namespace sofacv
{
namespace acquisition
{
/**
 * @brief The BaseGrabber interface
 *
 * All data grabbers should inherit this interface, as it provides a variation
 * to the ImplicitDataEngine's handleEvent function for data grabbers.
 */
class SOFA_SOFACV_API BaseGrabber : virtual public ImplicitDataEngine
{
 public:
  BaseGrabber() {}
  virtual ~BaseGrabber() {}

 protected:
  /// different handleEvent behavior for Data grabbers:
  /// checkData will always return false, as there is no input to this engine
  /// so:
  /// All grabbers should inherit a DataGrabber iface, itself deriving from
  /// ImplicitDataEngine
  /// A variant of handleEvent should be implemented there, that would look like
  /// this:
  virtual void handleEvent(sofa::core::objectmodel::Event* e)
  {
    if (sofa::simulation::AnimateBeginEvent::checkEventType(e))
    {
      update(); /// always call update() for grabbers. It's
                /// decided internally whether or not they should do something
      return;
    }
    ImplicitDataEngine::handleEvent(e);
  }
};

}  // namespace acquisition
}  // namespace sofacv
#endif  // SOFACV_ACQUISITION_BASEGRABBER_H
