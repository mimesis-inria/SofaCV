#include "PropagateEventByTagVisitor.h"
#include "ImplicitDataEngine.h"

namespace sofa
{

namespace simulation
{


PropagateEventByTagVisitor::PropagateEventByTagVisitor(const core::ExecParams* params, sofa::core::objectmodel::Event* e)
    : sofa::simulation::Visitor(params)
    , m_event(e)
{}


PropagateEventByTagVisitor::~PropagateEventByTagVisitor()
{}

bool compareEnginesByTag(sofa::OR::common::ImplicitDataEngine* e1, sofa::OR::common::ImplicitDataEngine* e2)
{
    const core::objectmodel::Tag& t1 = *(e1->getTags().begin());
    const core::objectmodel::Tag& t2 = *(e2->getTags().begin());
    return (t1 < t2);
}

Visitor::Result PropagateEventByTagVisitor::processNodeTopDown(simulation::Node* node)
{
    std::vector<sofa::OR::common::ImplicitDataEngine*> engines;
    node->getTreeObjects<sofa::OR::common::ImplicitDataEngine>(&engines);

    std::sort(engines.begin(), engines.end(), &compareEnginesByTag);

    for (sofa::OR::common::ImplicitDataEngine* engine : engines)
        this->processObject(node, engine);

    if( m_event->isHandled() )
        return Visitor::RESULT_PRUNE;
    else
        return Visitor::RESULT_CONTINUE;
}

void PropagateEventByTagVisitor::processObject(simulation::Node*, core::objectmodel::BaseObject* obj)
{
    if( obj->f_listening.getValue()==true )
        obj->handleEvent( m_event );
}


}

}

