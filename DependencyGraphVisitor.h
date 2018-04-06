#ifndef SOFA_DATA_DEPENDENCY_GRAPH_VISITOR_H
#define SOFA_DATA_DEPENDENCY_GRAPH_VISITOR_H

#include <SofaSimulationGraph/DAGSimulation.h>
#include <sofa/simulation/Visitor.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/DDGNode.h>


#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

class DependencyGraphVisitor : public sofa::simulation::Visitor
{
 public:
  DependencyGraphVisitor(const sofa::core::ExecParams* params)
      : sofa::simulation::Visitor(params)
  {
  }

  void printInput(sofa::core::objectmodel::DDGNode* data, int level)
  {
    for (int i = 0 ; i < level ; ++i)
      std::cout << "\t";
    std::cout << data->getOwner()->getName() << "::" << data->getName() << std::endl;

    level++;
    for (auto input : data->getInputs())
    {
      printInput(input, level);
    }
  }

  sofa::simulation::Visitor::Result processNodeTopDown(sofa::simulation::Node* node)
  {
    std::vector<sofa::core::objectmodel::BaseObject*> objs;
    node->getContext()->getObjects(objs);
    std::cout << "Number of objects in node: " << objs.size() << std::endl;

    for (const auto& obj : objs)
    {
        std::cout << obj->getName() << std::endl;
      // retrieve all data fields in component
      for (const auto& p : obj->getDataFields())
      {
        // keep only those with no outputs (leafs)
        if (p->getOutputs().empty() && !p->getInputs().empty())
        {
          printInput(p, 0);
        }
      }
    }
    return RESULT_CONTINUE;
  }
};

#endif  // SOFA_DATA_DEPENDENCY_GRAPH_VISITOR_H

