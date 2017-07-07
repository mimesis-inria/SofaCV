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

#include <SofaORCommon/cvDMatch.h>
#include <SofaORCommon/cvKeypoint.h>
#include <SofaORCommon/cvMat.h>

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
class ImplicitDataEngine : public virtual sofa::core::objectmodel::BaseObject
{
 public:
  typedef void (ImplicitDataEngine::*DataCallback)(
			sofa::core::objectmodel::BaseData*);
	SOFA_CLASS(ImplicitDataEngine, sofa::core::objectmodel::BaseObject);

  /// Constructor. d_isLeft is only for stereo (L/R) data engines
  /// (leave untouched otherwise)
  ImplicitDataEngine()
			: d_autolink(initData(&d_autolink, false, "autolink",
														"if set to true, allows implicit link setting "
														"between components' data. This makes scene "
														"writing less cumbersome but can potentially lead "
														"to undefined / unexpected "
														"behaviors. To use sparsely & wisely!")),
				d_isLeft(initData(&d_isLeft, true, "left",
													"set to true by default, allows for distinction "
													"between left and right data when dealing with "
													"stereo data",
													true, true))
  {
		f_listening.setValue(true);
  }
  virtual ~ImplicitDataEngine() {}
	virtual void init() {}
	/// Engine's general computations
  virtual void update() {}
	/// Runs an IdleEvent visitor to propagate the engine's output to the other
	/// engines
  virtual void reinit()
  {
    update();
    std::cout << std::endl << "Propagating from " << getName() << std::endl;
		sofa::core::objectmodel::IdleEvent ie;
		sofa::simulation::PropagateEventVisitor v(
				sofa::core::ExecParams::defaultInstance(), &ie);
    this->getContext()->getRootContext()->executeVisitor(&v);
  }

 protected:
  /// default callback function for bindInputData. BaseData is the dirty data,
  /// ImplicitDataEngine is the data's owner
	void defaultDataCallback(sofa::core::objectmodel::BaseData*) {}
	/// Adds a new input to this engine, binds it to its parent if not set
	/// through XML, and sets a callback if provided
  void addInput(
			sofa::core::objectmodel::BaseData* data, bool trackOnly = false,
      DataCallback callback = &ImplicitDataEngine::defaultDataCallback);
	/// Removes the data from the inputs of the engine
	void removeInput(sofa::core::objectmodel::BaseData* data);

	/// Sets a callback method for data
	void addDataCallback(sofa::core::objectmodel::BaseData* data,
                       DataCallback callback);
	/// Removes the callback method for a specific data
	void removeDataCallback(sofa::core::objectmodel::BaseData* data);

	/// Sets a data as an output for this engine
	void addOutput(sofa::core::objectmodel::BaseData* data);
	/// Removes a data from this engine's list of outputs
	void removeOutput(sofa::core::objectmodel::BaseData* data);

  /// default handleEvent behavior. Can be overloaded.
  /// First checks for dirty data and call their respective callbacks
  /// Then calls update
  virtual void handleEvent(sofa::core::objectmodel::Event* e)
  {
		if ((sofa::core::objectmodel::IdleEvent::checkEventType(e) ||
				 sofa::simulation::AnimateBeginEvent::checkEventType(e)) &&
				checkInputs())
		{
			update();
			clean();
    }
  }

 public:
	sofa::Data<bool> d_autolink;  ///< If false, engine won't try to implicitely
																///bind the inputs to previously declared
																///engine's outputs in graph
	sofa::Data<bool> d_isLeft;

 protected:
	/// Checks if inputs are dirty, to determine whether or not to call the engine's update method
  bool checkInputs();
	/// Checks if datas are dirty, and calls their callbacks if necessary
	void checkData(bool call_callback = true);
	/// removes the dirty flags on engine's inputs and outputs
	void clean();

 private:
	typedef std::pair<sofa::core::DataTracker*, DataCallback> trackPair;
	typedef std::pair<sofa::core::objectmodel::BaseData*, trackPair*> trackedData;
	typedef std::map<sofa::core::objectmodel::BaseData*, trackPair*> TrackMap;

	void _trackData(sofa::core::objectmodel::BaseData* data,
									DataCallback callback, TrackMap& map);
	bool _bindData(sofa::core::objectmodel::BaseData* data,
								 const std::string& alias);
  ImplicitDataEngine* getPreviousEngineInGraph();

  TrackMap m_inputs;
  TrackMap m_trackers;
  DataCallback m_callback;
	std::map<sofa::core::objectmodel::BaseData*, sofa::core::DataTracker*>
			m_outputs;
};

}  // namespace common
}  // namespace sofaor

#endif  // SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H
