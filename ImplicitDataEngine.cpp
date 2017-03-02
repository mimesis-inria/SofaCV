#include "ImplicitDataEngine.h"

namespace sofa
{
namespace OR
{
namespace common
{
int ImplicitDataEngine::tagID = 0;

bool ImplicitDataEngine::checkInputs()
{
  bool hasDirtyValues = false;

  // Calling callbacks of all dirty data registered with "addDataCallback" and
  // cleaning them too
  for (std::map<core::objectmodel::BaseData*, trackPair>::value_type& t :
       m_trackers)
  {
    t.first->updateIfDirty();
    if (t.second.first->isDirty())
    {
      m_callback = t.second.second;
      (this->*m_callback)(t.first);
      t.first->cleanDirty();
      t.second.first->clean();
    }
  }

  // Calling callbacks of all dirty inputs registered with "addInput" returning
  // true if any of the inputs is dirty
  for (std::map<core::objectmodel::BaseData*, trackPair>::value_type& t :
       m_inputs)
  {
    t.first->updateIfDirty();
    if (t.second.first->isDirty())
    {
      m_callback = t.second.second;
      (this->*m_callback)(t.first);
      hasDirtyValues = true;
    }
  }
  return hasDirtyValues;
}

void ImplicitDataEngine::clean()
{
  // Cleaning all inputs
  for (std::map<core::objectmodel::BaseData*, trackPair>::value_type& t :
       m_inputs)
  {
    t.first->cleanDirty();
    t.second.first->clean();
  }
  // Setting modified output values to dirty.
  // TODO: Only set dirtyValue if the output was changed from within, so that
  // other engines get notified from the change
  for (std::map<core::objectmodel::BaseData*, core::DataTracker*>::value_type&
           p : m_outputs)
  {
    if (p.second->isDirty())
    {
      p.second->clean();
      p.first->setDirtyValue();
      std::cout << p.first->getName() << std::endl;
    }
  }
}

bool ImplicitDataEngine::_bindData(core::objectmodel::BaseData* data,
                                   const std::string& alias)
{
  const std::multimap<std::string, core::objectmodel::BaseData*>& dataMap =
      this->getDataAliases();

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

void ImplicitDataEngine::_trackData(core::objectmodel::BaseData* data,
                                    DataCallback callback, TrackMap& map)
{
  core::DataTracker* tracker = new core::DataTracker();
  tracker->trackData(*data);
  data->setDirtyValue();
  map.insert(trackedData(data, trackPair(tracker, callback)));
}

void ImplicitDataEngine::addDataCallback(core::objectmodel::BaseData* data,
                                         DataCallback callback)
{
  _trackData(data, callback, m_trackers);
}

void ImplicitDataEngine::addInput(core::objectmodel::BaseData* data,
                                  bool trackOnly, DataCallback callback)
{
  if (trackOnly || data->isSet())
  {
    _trackData(data, callback, m_inputs);
    return;
  }
  ImplicitDataEngine* engine = getPreviousEngineInGraph();
  if (engine)
  {
    bool isBinded = false;
    if (d_isLeft.getValue())
    {
      isBinded = engine->_bindData(data, data->getName() + "1_out");
      if (!isBinded)
        isBinded = engine->_bindData(data, data->getName() + "_out");
    }
    else
      isBinded = engine->_bindData(data, data->getName() + "2_out");

    if (!isBinded)
    {
      msg_warning(getName() + "::bindInputData()")
          << "couldn't bind input data " << data->getName()
          << " implicitly. Link is broken";
    }
    else
      _trackData(data, callback, m_inputs);
  }
  else
    msg_warning(getName() + "::bindInputData()")
        << "couldn't bind input data " << data->getName()
        << " implicitly. Link is broken";
}

void ImplicitDataEngine::addOutput(core::objectmodel::BaseData* data)
{
  core::DataTracker* tracker = new core::DataTracker();
  tracker->trackData(*data);
  data->cleanDirty();
  tracker->clean();
  m_outputs.insert(std::make_pair(data, tracker));
}

void ImplicitDataEngine::removeInput(core::objectmodel::BaseData* data)
{
  m_inputs.erase(m_inputs.find(data));
}
void ImplicitDataEngine::removeOutput(core::objectmodel::BaseData* data)
{
  m_outputs.erase(m_outputs.find(data));
}

void ImplicitDataEngine::removeDataCallback(core::objectmodel::BaseData* data)
{
  m_trackers.erase(m_trackers.find(data));
}

}  // namespace common
}  // namespace OR
}  // namespace sofa
