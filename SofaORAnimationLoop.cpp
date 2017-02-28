/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
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
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "SofaORAnimationLoop.h"
#include "PropagateEventByTagVisitor.h"

#include <sofa/core/ObjectFactory.h>

#include <sofa/simulation/AnimateBeginEvent.h>
#include <sofa/simulation/AnimateEndEvent.h>
#include <sofa/simulation/AnimateVisitor.h>
#include <sofa/simulation/BehaviorUpdatePositionVisitor.h>
#include <sofa/simulation/UpdateBoundingBoxVisitor.h>
#include <sofa/simulation/UpdateContextVisitor.h>
#include <sofa/simulation/UpdateMappingEndEvent.h>
#include <sofa/simulation/UpdateMappingVisitor.h>

#include <sofa/helper/system/SetDirectory.h>
//#include <sofa/helper/system/PipeProcess.h>
#include <sofa/helper/AdvancedTimer.h>

#include <sofa/core/visual/VisualParams.h>

#include <math.h>
#include <stdlib.h>
#include <algorithm>

namespace sofa
{
namespace simulation
{
SOFA_DECL_CLASS(SofaORAnimationLoop)

int SofaORAnimationLoopClass = core::RegisterObject(
                                   "The default SOFAOR animation loop, created "
                                   "for data-driven visitor pipelines")
                                   .add<SofaORAnimationLoop>();

SofaORAnimationLoop::SofaORAnimationLoop(simulation::Node* _gnode)
    : Inherit(), gnode(_gnode)
{
    gnode->addAnimationLoop(this);
  // assert(gnode);
}

SofaORAnimationLoop::~SofaORAnimationLoop() {}
void SofaORAnimationLoop::init()
{
  if (!gnode) gnode = dynamic_cast<simulation::Node*>(this->getContext());
}

void SofaORAnimationLoop::setNode(simulation::Node* n) { gnode = n; }
void SofaORAnimationLoop::step(const core::ExecParams* params, SReal dt)
{
    std::cout << "YAHOOOOOO!!!" << std::endl;
  if (dt == 0) dt = this->gnode->getDt();

#ifdef SOFA_DUMP_VISITOR_INFO
  simulation::Visitor::printNode("Step");
#endif
  AnimateBeginEvent ev(dt);
  PropagateEventByTagVisitor act(params, &ev);
  gnode->execute(act);

#ifdef SOFA_DUMP_VISITOR_INFO
  simulation::Visitor::printCloseNode("Step");
#endif
}

}  // namespace simulation

}  // namespace sofa
