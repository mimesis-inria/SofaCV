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

#include "ImplicitDataEngine.h"

namespace sofaor
{
namespace common
{
// Removes the dirty flag from tracked internal data
void ImplicitDataEngine::cleanTrackers(bool call_callback)
{
  TrackMap dataToUpdate;

  // Calling callbacks of all dirty data registered with "addDataCallback" and
  // cleaning them too
  for (TrackMap::value_type& t : m_trackers)
  {
    if (t.second.first.isDirty() || t.first->isDirty())
    {
      if (call_callback)
        dataToUpdate.insert(t);
      else
        t.second.first.clean();
    }
  }
  for (TrackMap::value_type& t : dataToUpdate)
  {
    t.second.first.clean();
    if (t.second.second)
    {
      t.second.second->call(t.first);
    }
  }
}

// Removes the dirty flag from input data
bool ImplicitDataEngine::cleanInputs()
{
  bool hasDirtyValues = false;

  // Calling callbacks of all dirty inputs registered with "addInput" returning
  // true if any of the inputs is dirty
  for (TrackMap::value_type& t : m_inputs)
  {
    if (t.first->isDirty() || t.second.first.isDirty())
    {
      t.second.first.clean();
      t.first->updateIfDirty();
      t.first->cleanDirty();
      if (t.second.second) t.second.second->call(t.first);
      hasDirtyValues = true;
    }
  }
  return hasDirtyValues;
}

void ImplicitDataEngine::setDirtyOutputs()
{
  for (TrackMap::value_type& t : m_outputs)
  {
    if (t.second.first.isDirty())
    {
      t.second.first.clean();
      t.first->setDirtyValue();
      t.first->setDirtyOutputs();
    }
  }
}

bool ImplicitDataEngine::_bindData(sofa::core::objectmodel::BaseData* data,
                                   const std::string& alias)
{
  const std::multimap<std::string, sofa::core::objectmodel::BaseData*>&
      dataMap = this->getDataAliases();

  for (auto& d : dataMap)
    if (d.first == alias)
    {
      data->setParent(d.second, "@" + this->getPathName() + "." + alias);
      return true;
    }
  ImplicitDataEngine* engine = getPreviousEngineInGraph();
  if (engine)
    return engine->_bindData(data, alias);
  else
    return false;
}

ImplicitDataEngine* ImplicitDataEngine::getPreviousEngineInGraph()
{
  std::vector<ImplicitDataEngine*> engines;
  getContext()->get<ImplicitDataEngine>(&engines);
  for (size_t i = 0; i < engines.size(); ++i)
    if (engines[i]->getName() == getName())
      return (i) ? (engines[i - 1]) : (NULL);
  return NULL;
}

void ImplicitDataEngine::_trackData(sofa::core::objectmodel::BaseData* data,
                                    CallbackFunctor* callback, TrackMap& map)
{
  sofa::core::DataTracker tracker;
  tracker.trackData(*data);
  map.insert(std::make_pair(data, std::make_pair(tracker, callback)));
}

void ImplicitDataEngine::addDataCallback(
    sofa::core::objectmodel::BaseData* data, CallbackFunctor* callback)
{
  _trackData(data, callback, m_trackers);
}

ImplicitDataEngine::ImplicitDataEngine()
    : d_autolink(initData(&d_autolink, false, "autolink",
                          "if set to true, allows implicit link setting "
                          "between components' data. This makes scene "
                          "writing less cumbersome but can potentially lead "
                          "to undefined / unexpected "
                          "behaviors. To use sparsely & wisely!"))
{
  f_listening.setValue(true);
}

void ImplicitDataEngine::reinit()
{
  cleanTrackers();
  update();
  setDirtyOutputs();
  sofa::core::objectmodel::IdleEvent ie;
  sofa::simulation::PropagateEventVisitor v(
      sofa::core::ExecParams::defaultInstance(), &ie);
  this->getContext()->getRootContext()->executeVisitor(&v);
}

void ImplicitDataEngine::addInput(sofa::core::objectmodel::BaseData* data,
                                  bool trackOnly, CallbackFunctor* callback)
{
  if (!d_autolink.getValue() || trackOnly || data->isSet())
  {
    _trackData(data, callback, m_inputs);
    return;
  }
  ImplicitDataEngine* engine = getPreviousEngineInGraph();
  if (engine)
  {
    bool isBinded = false;
    isBinded = engine->_bindData(data, data->getName() + "_out");

    if (!isBinded)
    {
      msg_warning(getName() + "::bindInputData()")
          << "couldn't bind input data " << data->getName()
          << " implicitly. Link is broken";
    }
    else
    {
      _trackData(data, callback, m_inputs);
      msg_advice(getName() + "::" + data->getName())
          << "linked to " << data->getLinkPath()
          << " implicitly. Please ensure that this makes sense. Otherwise, set "
             "autolink to false";
    }
  }
  else
    msg_warning(getName() + "::bindInputData()")
        << "couldn't bind input data " << data->getName()
        << " implicitly. Link is broken";
}

void ImplicitDataEngine::addOutput(sofa::core::objectmodel::BaseData* data)
{
  if (m_inputs.find(data) != m_inputs.end())
  {
    msg_error(getName() + "::addOutput()")
        << "Cannot add an input data as output";
    return;
  }
  _trackData(data, 0, m_outputs);
  if (data->isDirty())
  {
    data->setDirtyValue();
    data->setDirtyOutputs();
  }
}

void ImplicitDataEngine::removeInput(sofa::core::objectmodel::BaseData* data)
{
  if (m_inputs.find(data) != m_inputs.end())
    m_inputs.erase(m_inputs.find(data));
}

void ImplicitDataEngine::removeOutput(sofa::core::objectmodel::BaseData* data)
{
  if (m_outputs.find(data) != m_outputs.end())
    m_outputs.erase(m_outputs.find(data));
}

void ImplicitDataEngine::handleEvent(sofa::core::objectmodel::Event* e)
{
  if (sofa::core::objectmodel::IdleEvent::checkEventType(e) ||
      sofa::simulation::AnimateBeginEvent::checkEventType(e))
  {
    if (cleanInputs()) update();
    setDirtyOutputs();
  }
}

void ImplicitDataEngine::removeDataCallback(
    sofa::core::objectmodel::BaseData* data)
{
  if (m_trackers.find(data) != m_trackers.end())
    m_trackers.erase(m_trackers.find(data));
}

}  // namespace common
}  // namespace sofaor
