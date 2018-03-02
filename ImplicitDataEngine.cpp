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
  std::cout << "calling reinit" << std::endl;
  Reinit();
//  cleanDirty(); // this cleans the datatrackers without updating the data from parent values
}

void ImplicitDataEngine::addInput(sofa::core::objectmodel::BaseData* data,
                                  bool trackOnly)
{
  if (!d_autolink.getValue() || trackOnly || data->isSet())
  {
    trackData(data);
    sofa::core::objectmodel::DDGNode::addInput(data);
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
      trackData(data);
      sofa::core::objectmodel::DDGNode::addInput(data);
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
    sofa::core::objectmodel::DDGNode::addOutput(data);
    data->setDirtyValue();
    setDirtyValue();
}

/// Prevent Engine from updating this data from its parent values
void ImplicitDataEngine::removeInput(sofa::core::objectmodel::BaseData* data)
{
    sofa::core::objectmodel::DDGNode::delInput(data);
}

void ImplicitDataEngine::removeOutput(sofa::core::objectmodel::BaseData* data)
{
    sofa::core::objectmodel::DDGNode::delOutput(data);
}

void ImplicitDataEngine::handleEvent(sofa::core::objectmodel::Event* e)
{
  if (sofa::core::objectmodel::IdleEvent::checkEventType(e) ||
      sofa::simulation::AnimateBeginEvent::checkEventType(e))
  {
      std::cout << getName() << " updateIfDirty()" << std::endl;
      updateIfDirty(); // is isDirty() true????????
  }
}

void ImplicitDataEngine::trackData(sofa::core::objectmodel::BaseData* data)
{
    data->cleanDirty();
    m_dataTracker.trackData(*data);
    m_dataTracker.clean(*data);
}

void ImplicitDataEngine::untrackData(
    sofa::core::objectmodel::BaseData* /*data*/)
{
    // need a way to remove a data from a DataTracker...
}

}  // namespace common
}  // namespace sofaor
