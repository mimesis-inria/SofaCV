/******************************************************************************
 *       SOFAOR, SOFA plugin for the Operating Room, development version       *
 *                        (c) 2017 INRIA, MIMESIS Team                         *
 *                                                                             *
 * This program is a free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation; either version 1.0 of the License, or (at     *
 * your option) any later version.                                             *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
 * for more details.                                                           *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.        *
 *******************************************************************************
 * Authors: Bruno Marques and external contributors (see Authors.txt)          *
 *                                                                             *
 * Contact information: contact-mimesis@inria.fr                               *
 ******************************************************************************/

#ifndef SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H
#define SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H

#include "cvDMatch.h"
#include "cvKeypoint.h"
#include "cvMat.h"

#include <sofa/core/DataEngine.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/objectmodel/IdleEvent.h>
#include <sofa/simulation/AnimateBeginEvent.h>
#include <sofa/simulation/AnimateEndEvent.h>
#include <sofa/simulation/PropagateEventVisitor.h>

namespace sofaor
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

class CallbackFunctor
{
 public:
  virtual void call(sofa::core::objectmodel::BaseData* o) = 0;
};

#define SOFAOR_CALLBACK_SYSTEM(T)                                              \
  typedef T SOFAOR_CLASS;                                                      \
  class Callback : public sofaor::common::CallbackFunctor                      \
  {                                                                            \
    typedef void (SOFAOR_CLASS::*Func)(sofa::core::objectmodel::BaseData * o); \
                                                                               \
    SOFAOR_CLASS* m_obj;                                                       \
    Func m_func;                                                               \
                                                                               \
   public:                                                                     \
    Callback(SOFAOR_CLASS* _this, Func f) : m_obj(_this), m_func(f) {}         \
    void call(sofa::core::objectmodel::BaseData* data = 0)                     \
    {                                                                          \
      (m_obj->*m_func)(data);                                                  \
    }                                                                          \
  }

#define SOFAOR_ADD_CALLBACK(data, callback) \
  addDataCallback(data, new Callback(this, callback))

#define SOFAOR_ADD_INPUT_CALLBACK(data, callback, trackOnly) \
  addInput(data, trackOnly, new Callback(this, callback))

class SOFA_SOFAORCOMMON_API ImplicitDataEngine : public virtual sofa::core::objectmodel::BaseObject
{
 public:
  SOFA_CLASS(ImplicitDataEngine, sofa::core::objectmodel::BaseObject);

  /// Constructor. d_isLeft is only for stereo (L/R) data engines
  /// (leave untouched otherwise)
  ImplicitDataEngine();
  virtual ~ImplicitDataEngine() {}
  virtual void init() {}
  /// Engine's general computations (Called anytime an input data is changed)
  virtual void update() {}
  /// Runs an IdleEvent visitor to propagate the engine's output to the other
  /// engines
  virtual void reinit();

 protected:
  /// Adds a new input to this engine, binds it to its parent if not set
  /// through XML, and sets a callback if provided
  /// trackOnly allows you to have an engine with autolink, but with some
  /// variables (flagged trackOnly) that would not be looked for using the
  /// autolink (optional)
  void addInput(sofa::core::objectmodel::BaseData* data, bool trackOnly = false,
                CallbackFunctor* functor = NULL);
  /// Removes the data from the inputs of the engine
  void removeInput(sofa::core::objectmodel::BaseData* data);

  /// Sets a callback method for data
  void addDataCallback(sofa::core::objectmodel::BaseData* data,
                       CallbackFunctor* callback);
  /// Removes the callback method for a specific data
  void removeDataCallback(sofa::core::objectmodel::BaseData* data);

  /// Sets a data as an output for this engine
  void addOutput(sofa::core::objectmodel::BaseData* data);
  /// Removes a data from this engine's list of outputs
  void removeOutput(sofa::core::objectmodel::BaseData* data);

  /// default handleEvent behavior. Can be overloaded.
  /// First checks for dirty data and call their respective callbacks
  /// Then calls update
  virtual void handleEvent(sofa::core::objectmodel::Event* e);

 public:
  sofa::Data<bool> d_autolink;  ///< If false, engine won't try to implicitely
                                /// bind the inputs to previously declared

  void setDirtyOutputs();

 protected:
  void cleanTrackers(bool call_callback = true);
  bool cleanInputs();

  //  /// Checks if inputs are dirty, to determine whether or not to call the
  //  /// engine's update method
  //  bool checkInputs();
  //  /// Checks if datas are dirty, and calls their callbacks if necessary
  //  void checkData(bool call_callback = true);
  //  /// removes the dirty flags on engine's inputs and outputs
  //  void clean();

 private:
  typedef std::pair<sofa::core::DataTracker, CallbackFunctor*> trackPair;
  //  typedef std::pair<sofa::core::objectmodel::BaseData*, trackPair*>
  //  trackedData;
  typedef std::map<sofa::core::objectmodel::BaseData*, trackPair> TrackMap;

  void _trackData(sofa::core::objectmodel::BaseData* data,
                  CallbackFunctor* callback, TrackMap& map);
  bool _bindData(sofa::core::objectmodel::BaseData* data,
                 const std::string& alias);
  ImplicitDataEngine* getPreviousEngineInGraph();

 protected:
  TrackMap m_inputs;
  TrackMap m_trackers;
  TrackMap m_outputs;
};

}  // namespace common
}  // namespace sofaor

#endif  // SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H
