/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
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
#include "DependencyGraphAnimationLoop.h"
#include "DependencyGraphVisitor.h"

#include <sofa/simulation/DefaultAnimationLoop.h>
#include <sofa/core/ObjectFactory.h>

#include <sofa/simulation/AnimateVisitor.h>
#include <sofa/simulation/UpdateContextVisitor.h>
#include <sofa/simulation/UpdateMappingVisitor.h>
#include <sofa/simulation/PropagateEventVisitor.h>
#include <sofa/simulation/BehaviorUpdatePositionVisitor.h>
#include <sofa/simulation/AnimateBeginEvent.h>
#include <sofa/simulation/AnimateEndEvent.h>
#include <sofa/simulation/UpdateMappingEndEvent.h>
#include <sofa/simulation/UpdateBoundingBoxVisitor.h>

#include <sofa/helper/system/SetDirectory.h>
//#include <sofa/helper/system/PipeProcess.h>
#include <sofa/helper/AdvancedTimer.h>

#include <sofa/core/visual/VisualParams.h>

#include <stdlib.h>
#include <math.h>
#include <algorithm>


namespace sofa
{

namespace simulation
{

SOFA_DECL_CLASS(DependencyGraphAnimationLoop)

int DependencyGraphAnimationLoopClass = core::RegisterObject("The Default animation loop, running the DependencyGraphVisitor")
        .add< DependencyGraphAnimationLoop >()
        ;

void DependencyGraphAnimationLoop::init()
{
  DefaultAnimationLoop::init();
  DependencyGraphVisitor visitor(sofa::core::ExecParams::defaultInstance());
  this->gnode->execute(visitor);
}

void DependencyGraphAnimationLoop::step(const core::ExecParams* params, SReal dt)
{
  DependencyGraphVisitor visitor(params);
  this->gnode->execute(visitor);

  DefaultAnimationLoop::step(params, dt);
}

} // namespace simulation
} // namespace sofa
