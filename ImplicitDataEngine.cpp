#include "ImplicitDataEngine.h"

namespace sofa
{
namespace OR
{
namespace common
{

int ImplicitDataEngine::tagID = 0;


bool ImplicitDataEngine::checkData()
{
  bool hasDirtyValues = false;
  for (std::map<core::objectmodel::BaseData*, trackPair>::value_type& t :
       m_trackers)
  {
    // First update data to retrieve dirty values from input data. Then check if
    // dirty, and if so, call callback
    t.first->update();
    if (t.second.first->isDirty())
    {
//      std::cout << t.first->getName() << " is DIRTY" << std::endl;
      m_callback = t.second.second;
      (this->*m_callback)(t.first);
      hasDirtyValues = true;
    }
  }
  return hasDirtyValues;
}

void ImplicitDataEngine::cleanData()
{
  for (std::map<core::objectmodel::BaseData*, trackPair>::value_type& t :
       m_trackers)
  {
    if (t.second.first->isDirty())
    {
//      std::cout << t.first->getName() << " CLEANING DIRT VALUE!!" << std::endl;
      t.second.first->clean();
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
                                    DataCallback callback)
{
  core::DataTracker* tracker = new core::DataTracker();
  tracker->trackData(*data);
  m_trackers.insert(trackedData(data, trackPair(tracker, callback)));
}

void ImplicitDataEngine::trackData(core::objectmodel::BaseData* data,
                                   bool trackOnly, DataCallback callback)
{
  if (trackOnly || data->isSet())
  {
    _trackData(data, callback);
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
      _trackData(data, callback);
  }
  else
    msg_warning(getName() + "::bindInputData()")
        << "couldn't bind input data " << data->getName()
        << " implicitly. Link is broken";
}

}  // namespace common
}  // namespace OR
}  // namespace sofa
