#ifndef SOFA_SIMULATION_SOFAORANIMATIONLOOP_H
#define SOFA_SIMULATION_SOFAORANIMATIONLOOP_H

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/behavior/BaseAnimationLoop.h>
#include <sofa/core/ExecParams.h>
#include <sofa/simulation/simulationcore.h>
#include <sofa/simulation/Node.h>
#include <sofa/helper/AdvancedTimer.h>

namespace sofa
{

namespace simulation
{

/**
 *  \brief SOFAOR Animation Loop: To be set in every SOFAOR nodes. Visitors in this loop are called on every component of the tree according to their tags
 *		
 *
 */

class SOFA_SIMULATION_CORE_API SofaORAnimationLoop : public sofa::core::behavior::BaseAnimationLoop
{
public:
    typedef sofa::core::behavior::BaseAnimationLoop Inherit;
    typedef sofa::core::objectmodel::BaseContext BaseContext;
    typedef sofa::core::objectmodel::BaseObjectDescription BaseObjectDescription;
    SOFA_CLASS(SofaORAnimationLoop,sofa::core::behavior::BaseAnimationLoop);
protected:
    SofaORAnimationLoop(simulation::Node* gnode = NULL);

    virtual ~SofaORAnimationLoop();
public:
    /// Set the simulation node this animation loop is controlling
    virtual void setNode( simulation::Node* );

    /// Set the simulation node to the local context if not specified previously
    virtual void init();

    /// perform one animation step
    virtual void step(const core::ExecParams* params, SReal dt);


    /// Construction method called by ObjectFactory.
    template<class T>
    static typename T::SPtr create(T*, BaseContext* context, BaseObjectDescription* arg)
    {
        simulation::Node* gnode = dynamic_cast<simulation::Node*>(context);
        typename T::SPtr obj = sofa::core::objectmodel::New<T>(gnode);
        if (context) context->addObject(obj);
        if (arg) obj->parse(arg);
        return obj;
    }

protected :

    simulation::Node* gnode;  ///< the node controlled by the loop

};

} // namespace simulation

} // namespace sofa

#endif  /* SOFA_SIMULATION_SOFAORANIMATIONLOOP_H */
