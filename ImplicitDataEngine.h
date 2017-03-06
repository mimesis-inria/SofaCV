#ifndef SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H
#define SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H

#include <SofaORCommon/cvDMatch.h>
#include <SofaORCommon/cvKeypoint.h>
#include <SofaORCommon/cvMat.h>

#include <sofa/core/DataEngine.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/objectmodel/IdleEvent.h>
#include <sofa/simulation/AnimateBeginEvent.h>
#include <sofa/simulation/AnimateEndEvent.h>
#include <sofa/simulation/PropagateEventVisitor.h>

namespace sofa
{
namespace OR
{
namespace common
{
/**
 *  \brief from a set of Data inputs computes a set of Data outputs while hiding
 * the data tracking mechanisms
 *
 * Implementation good rules:
 *
 * void init()
 * {
 *    bindInputData(&d_data); // indicate all inputs you want to bind
 *
 *    // add this line before bindInputData() if you want to
 *    // force this data to be explicitely linked in the xml scene
 *    if (d_data.isSet())
 *      bindInputData(&d_optionalData);
 *
 *    // use this call to use a callback function
 *    // called when the input is modified, just before update()
 *    bindInputData(&d_data,
 * (ImplicitDataEngine::DataCallback)&DerivedClass::func);
 * }
 *
 * // optional (called each time a data is modified in the gui)
 * // it is not always desired. You should use data callbacks instead
 * void reinit()
 * {
 *   // perform some action
 *   ImplicitDataEngine::reinit();
 * }
 *
 * void update()
 * {
 *  // perform your general computations
 * }
 *
 */
class ImplicitDataEngine : public core::objectmodel::BaseObject
{
  static int tagID;

 public:
  typedef void (ImplicitDataEngine::*DataCallback)(
      core::objectmodel::BaseData*);
  SOFA_CLASS(ImplicitDataEngine, core::objectmodel::BaseObject);

  /// Constructor. d_isLeft is only for stereo (L/R) data engines
  /// (leave untouched otherwise)
  ImplicitDataEngine()
      : d_isLeft(initData(&d_isLeft, true, "left",
                          "set to true by default, allows for distinction "
                          "between left and right data when dealing with "
                          "stereo data",
                          true, true))
  {
      f_listening.setValue(true);
  }
  virtual ~ImplicitDataEngine() {}
  virtual void init()
  {
  }
  /// perform your general computations here
  virtual void update() {}
  /// Sets m_needsRefresh to true to propagate modifications to other engines
  /// Override with an empty reinit to avoid idle refreshing (in DataGrabbers
  /// for instance)
  virtual void reinit()
  {
    update();
    std::cout << std::endl << "Propagating from " << getName() << std::endl;
    core::objectmodel::IdleEvent ie;
    simulation::PropagateEventVisitor v(core::ExecParams::defaultInstance(),
                                        &ie);
    this->getContext()->getRootContext()->executeVisitor(&v);
  }

 protected:
  /// default callback function for bindInputData. BaseData is the dirty data,
  /// ImplicitDataEngine is the data's owner
  void defaultDataCallback(core::objectmodel::BaseData*) {}
  /// Add a new input to this engine, and binds it to its parent if not set
  /// through XML
  void addInput(
      core::objectmodel::BaseData* data, bool trackOnly = false,
      DataCallback callback = &ImplicitDataEngine::defaultDataCallback);
  void removeInput(core::objectmodel::BaseData* data);

  void addDataCallback(core::objectmodel::BaseData* data,
                       DataCallback callback);
  void removeDataCallback(core::objectmodel::BaseData* data);

  void addOutput(core::objectmodel::BaseData* data);
  void removeOutput(core::objectmodel::BaseData* data);

  /// default handleEvent behavior. Can be overloaded.
  /// First checks for dirty data and call their respective callbacks
  /// Then calls update
  virtual void handleEvent(sofa::core::objectmodel::Event* e)
  {
    if (sofa::core::objectmodel::IdleEvent::checkEventType(e))
    {
      if (checkInputs())
      {
        update();
        clean();
      }
    }
    if (sofa::simulation::AnimateBeginEvent::checkEventType(e))
    {
      if (checkInputs())
      {
        update();
        clean();
      }
    }
  }

 public:
  Data<bool> d_isLeft;

 protected:
  bool checkInputs();
	void checkData();
	void clean();

 private:
  typedef std::pair<core::DataTracker*, DataCallback> trackPair;
  typedef std::pair<core::objectmodel::BaseData*, trackPair> trackedData;
  typedef std::map<core::objectmodel::BaseData*, trackPair> TrackMap;

  void _trackData(core::objectmodel::BaseData* data, DataCallback callback,
                  TrackMap& map);
  bool _bindData(core::objectmodel::BaseData* data, const std::string& alias);
  ImplicitDataEngine* getPreviousEngineInGraph();

  TrackMap m_inputs;
  TrackMap m_trackers;
  DataCallback m_callback;
  std::map<core::objectmodel::BaseData*, core::DataTracker*> m_outputs;
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H
