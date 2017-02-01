#ifndef SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H
#define SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H

#include <SofaORCommon/cvDMatch.h>
#include <SofaORCommon/cvKeypoint.h>
#include <SofaORCommon/cvMat.h>

#include <sofa/core/DataEngine.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/AnimateBeginEvent.h>

namespace sofa
{
namespace OR
{
namespace common
{
class ImplicitDataEngine : public core::DataEngine
{
 public:
  SOFA_CLASS(ImplicitDataEngine, core::DataEngine);

  ImplicitDataEngine()
      : d_isLeft(initData(&d_isLeft, true, "left",
                          "set to true by default, allows for distinction "
                          "between left and right data when dealing with "
                          "stereo data",
                          true, true))
  {
  }
  virtual ~ImplicitDataEngine() {}
 private:
  bool _bindData(core::objectmodel::BaseData* data, const std::string& alias)
  {
    std::cout << "_bindData " << getName() << std::endl;
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

  ImplicitDataEngine* getPreviousEngineInGraph()
  {
    std::vector<ImplicitDataEngine*> engines;
    getContext()->get<ImplicitDataEngine>(
        &engines);
    for (size_t i = 0; i < engines.size(); ++i)
      if (engines[i]->getName() == getName())
        return (i) ? (engines[i - 1]) : (NULL);
    return NULL;
  }

 protected:
  // to call instead of addInput(&data);
  void bindInputData(core::objectmodel::BaseData* data)
  {
    if (!data->isSet())
    {
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
          msg_error("BindData")
              << "couldn't bind input implicitly. Link is broken";
        }
        else
          addInput(data);
      }
      else
        msg_error("BindData")
            << "couldn't bind input implicitly. Link is broken";
    }
    else
      addInput(data);
  }

  Data<bool> d_isLeft;

 protected:
  void getInputFromContext() {}
  virtual void handleEvent(sofa::core::objectmodel::Event* e)
  {
    if (sofa::simulation::AnimateBeginEvent::checkEventType(e)) this->update();
  }
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_PROCESSOR_IMPLICITDATAENGINE_H
