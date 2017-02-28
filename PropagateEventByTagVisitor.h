
#ifndef SOFA_SIMULATION_PROPAGATEEVENTBYTAGVISITOR_H
#define SOFA_SIMULATION_PROPAGATEEVENTBYTAGVISITOR_H

#include <sofa/simulation/Visitor.h>
#include <sofa/core/objectmodel/Event.h>
#include <sofa/core/ExecParams.h>


namespace sofa
{

namespace OR {
namespace common {
class ImplicitDataEngine;
}
}

namespace simulation
{

/**
Visitor used to propagate an event in the the data structure.
Propagation is done top-down until the event is handled.

    @author The SOFA team </www.sofa-framework.org>
*/
class SOFA_SIMULATION_CORE_API PropagateEventByTagVisitor : public sofa::simulation::Visitor
{
public:
    PropagateEventByTagVisitor(const core::ExecParams* params, sofa::core::objectmodel::Event* e);

    ~PropagateEventByTagVisitor();

    Visitor::Result processNodeTopDown(simulation::Node* node);
    void processObject(simulation::Node*, core::objectmodel::BaseObject* obj);

    virtual const char* getClassName() const { return "PropagateEventByTagVisitor"; }
    virtual std::string getInfos() const { return std::string(m_event->getClassName());  }
protected:
    sofa::core::objectmodel::Event* m_event;

private:
//    bool compareEnginesByTag(sofa::OR::common::ImplicitDataEngine* e1, sofa::OR::common::ImplicitDataEngine* e2);
};


}

}

#endif // SOFA_SIMULATION_PROPAGATEEVENTBYTAGVISITOR_H
