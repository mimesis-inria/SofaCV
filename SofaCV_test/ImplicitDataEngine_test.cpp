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

#include <SceneCreator/SceneCreator.h>
#include "SofaCV/SofaCV.h"
using sofacv::ImplicitDataEngine;
#include <SofaTest/Sofa_test.h>

#include <SofaSimulationGraph/DAGSimulation.h>
#include <sofa/core/objectmodel/DDGNode.h>
#include <sofa/simulation/DefaultAnimationLoop.h>
#include <sofa/simulation/Simulation.h>

using sofa::core::objectmodel::BaseObject;
using sofa::simulation::DefaultAnimationLoop;
using sofa::simulation::Node;
using sofa::simulation::Simulation;

#include <SofaSimulationCommon/SceneLoaderXML.h>
using sofa::core::ExecParams;
using sofa::simulation::SceneLoaderXML;

namespace sofa
{
class ImplicitDataEngine_test;

struct TestInputComponent : public ImplicitDataEngine
{
  SOFA_CLASS(TestInputComponent, ImplicitDataEngine);

 public:
  sofa::Data<int> d_a;
  sofa::Data<int> d_b;
  sofa::Data<int> d_c;

  TestInputComponent()
  {
      d_a.setName("a_out");
      d_b.setName("b_out");
      d_c.setName("c_out");
  }

  void init()
  {
    addOutput(&d_a);
    addOutput(&d_b);
    addOutput(&d_c);
  }
};

struct StdComponent : public core::objectmodel::BaseObject
{
public:
    SOFA_CLASS(StdComponent, sofa::core::objectmodel::BaseObject);

    sofa::Data<int> d_a;
    sofa::Data<int> d_b;
    sofa::Data<int> d_c;

    StdComponent()
        : d_a(initData(&d_a, 0, "a", "")),
          d_b(initData(&d_b, 0, "b", "")),
          d_c(initData(&d_c, 0, "c", ""))
    {
    }


    void handleEvent(sofa::core::objectmodel::Event* e)
    {
        if (sofa::simulation::AnimateBeginEvent::checkEventType(e))
        {
            d_a.setValue(d_a.getValue() + 1);
            d_b.setValue(d_b.getValue() + 1);
            d_c.setValue(d_c.getValue() + 1);
        }
    }
};

struct TestEngine : public ImplicitDataEngine
{
  friend class ImplicitDataEngine_test;

 public:
  sofa::Data<int> d_a;
  sofa::Data<int> d_b;
  sofa::Data<int> d_c;
  sofa::Data<int> d_a_out;
  sofa::Data<int> d_b_out;
  sofa::Data<int> d_c_out;

  SOFA_CLASS(TestEngine, ImplicitDataEngine);

  TestEngine()
      : d_a(initData(&d_a, 0, "a", "")),
        d_b(initData(&d_b, 0, "b", "")),
        d_c(initData(&d_c, 0, "c", ""))
  {
      d_a_out.setName("a_out");
      d_b_out.setName("a_out");
      d_c_out.setName("a_out");
  }

  void init() override
  {
//    SOFAOR_ADD_INPUT_CALLBACK(&d_a, &TestEngine::increment, false);
//    SOFAOR_ADD_INPUT_CALLBACK(&d_b, &TestEngine::decrement, true);
    addInput(&d_a);
    addInput(&d_b);
    addInput(&d_c);

    addOutput(&d_a_out);
    addOutput(&d_b_out);
    addOutput(&d_c_out);
  }

  void Update() override
  {
    d_a_out.setValue(d_a.getValue()+1);
    d_b_out.setValue(d_b.getValue()+1);
    d_c_out.setValue(d_c.getValue()+1);
  }

 private:
//  void increment(sofa::core::objectmodel::BaseData* data)
//  {
//    d_a.setValue(d_a.getValue() + 1);
//  }
//  void decrement(sofa::core::objectmodel::BaseData* data)
//  {
//    d_b.setValue(d_b.getValue() - 1);
//  }
};

struct ImplicitDataEngine_test : public sofa::BaseTest
{
  sofa::simulation::Node::SPtr root;
  sofa::StdComponent::SPtr e1;
  sofa::TestEngine::SPtr e2;
  sofa::TestEngine::SPtr e3;
  sofa::TestEngine::SPtr e4;
  sofa::TestEngine::SPtr e5;
  sofa::TestEngine::SPtr e6;
  sofa::TestEngine::SPtr e7;
  core::objectmodel::BaseObjectDescription desc;

  ImplicitDataEngine_test() {}

  void testInit() {}

  void testStep1()
  {
    sofa::simulation::getSimulation()->init(this->root.get());
    e1->d_a.setValue(1);
    e1->d_b.setValue(2);
    e1->d_c.setValue(3);

    // Simulate an update from runSofa's GUI for instance:
    e1->reinit();

//    EXPECT_FALSE(e1->d_autolink.getValue());
    EXPECT_EQ(e1->d_a.getValue(), 1);
    EXPECT_EQ(e1->d_b.getValue(), 2);
    EXPECT_EQ(e1->d_c.getValue(), 3);

    EXPECT_FALSE(e2->d_autolink.getValue());
    EXPECT_EQ(e2->d_a.getValue(), 2);
    EXPECT_EQ(e2->d_b.getValue(), 0);
    EXPECT_EQ(e2->d_c.getValue(), 0);

    EXPECT_FALSE(e3->d_autolink.getValue());
    EXPECT_EQ(e3->d_a.getValue(), 0);
    EXPECT_EQ(e3->d_b.getValue(), 1);
    EXPECT_EQ(e3->d_c.getValue(), 0);

    EXPECT_FALSE(e4->d_autolink.getValue());
    EXPECT_EQ(e4->d_a.getValue(), 0);
    EXPECT_EQ(e4->d_b.getValue(), 0);
    EXPECT_EQ(e4->d_c.getValue(), 3);

    EXPECT_TRUE(e5->d_autolink.getValue());
    EXPECT_EQ(e5->d_a.getValue(), 1);
    EXPECT_EQ(e5->d_b.getValue(), 0);
    EXPECT_EQ(e5->d_c.getValue(), 3);

    EXPECT_FALSE(e6->d_autolink.getValue());
    EXPECT_EQ(e6->d_a.getValue(), 2);
    EXPECT_EQ(e6->d_b.getValue(), 1);
    EXPECT_EQ(e6->d_c.getValue(), 3);

    EXPECT_FALSE(e7->d_autolink.getValue());
    EXPECT_EQ(e7->d_a.getValue(), 2);
    EXPECT_EQ(e7->d_b.getValue(), -1);
    EXPECT_EQ(e7->d_c.getValue(), 3);
  }

  void SetUp()
  {
    using modeling::addNew;
    simulation::Simulation* simu;
    sofa::simulation::setSimulation(
        simu = new sofa::simulation::graph::DAGSimulation());

    root = simu->createNewGraph("root");
    root->addObject(
        sofa::core::objectmodel::New<DefaultAnimationLoop>(this->root.get()));
    e1 = addNew<StdComponent>(root);
    e2 = addNew<TestEngine>(root);
    e3 = addNew<TestEngine>(root);
    e4 = addNew<TestEngine>(root);
    e5 = addNew<TestEngine>(root);
    e6 = addNew<TestEngine>(root);
    e7 = addNew<TestEngine>(root);

    e1->setName("e1");
    e2->setName("e2");
    e3->setName("e3");
    e4->setName("e4");
    e5->setName("e5");
    e6->setName("e6");

    desc = core::objectmodel::BaseObjectDescription("EngineDesc");
    desc.setAttribute("name", "E1");
    desc.setAttribute("a", "0");
    desc.setAttribute("b", "0");
    desc.setAttribute("c", "0");
    e1->parse(&desc);

    desc = core::objectmodel::BaseObjectDescription("EngineDesc");
    desc.setAttribute("name", "E2");
    desc.setAttribute("a", "@E1.a_out");
    e2->parse(&desc);

    desc = core::objectmodel::BaseObjectDescription("EngineDesc");
    desc.setAttribute("name", "E3");
    desc.setAttribute("b", "@E1.b_out");
    e3->parse(&desc);

    desc = core::objectmodel::BaseObjectDescription("EngineDesc");
    desc.setAttribute("name", "E4");
    desc.setAttribute("c", "@E1.c_out");
    e4->parse(&desc);

    desc = core::objectmodel::BaseObjectDescription("EngineDesc");
    desc.setAttribute("name", "E5");
    desc.setAttribute("autolink", "true");
    e5->parse(&desc);

    desc = core::objectmodel::BaseObjectDescription("EngineDesc");
    desc.setAttribute("name", "E6");
    desc.setAttribute("a", "@E1.a_out");
    desc.setAttribute("b", "@E1.b_out");
    desc.setAttribute("c", "@E1.c_out");
    e6->parse(&desc);

    desc = core::objectmodel::BaseObjectDescription("EngineDesc");
    desc.setAttribute("name", "E7");
    desc.setAttribute("a", "@E5.a_out");
    desc.setAttribute("b", "@E5.b_out");
    desc.setAttribute("c", "@E5.c_out");
    e7->parse(&desc);
  }

  void TearDown() {}
};

/// Checks whether the autolink boolean is set to true on e2
TEST_F(ImplicitDataEngine_test, testInit) { this->testInit(); }
TEST_F(ImplicitDataEngine_test, testStep1) { this->testStep1(); }

}  // namespace sofa
