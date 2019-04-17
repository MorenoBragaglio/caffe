/***************************************************************************
 *            twowatertanks.cpp
 *
 *  Copyright  2017	Luca Geretti
 *
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "ariadne.hpp"
#include "t1.hpp"
#include "t2.hpp"
#include "t3.hpp"
#include "t4.hpp"
#include "controller1.hpp"
#include "controller2.hpp"
#include "controller3.hpp"
#include "controller4.hpp"
#include "controller5.hpp"

using namespace Ariadne;
using std::cout; using std::endl;

Int main(Int argc, const char* argv[])
{
    Nat evolver_verbosity=get_verbosity(argc,argv);

    // Declare the shared system variables
    RealVariable height_T1("height_T1");
    RealVariable height_T2("height_T2");
    RealVariable height_T3("height_T3");
    RealVariable height_T4("height_T4");

    //automi creati
    StringVariable controller1("controller1");
    StringVariable controller2("controller2");
    StringVariable controller3("controller3");
    StringVariable controller4("controller4");
    StringVariable controller5("controller5");
    StringVariable T4("T4");
    
    //stati iniziali
    StringConstant v_in_t1_opened("v_in_t1_opened");
    StringConstant v_out_t1_closed("v_out_t1_closed");
    StringConstant v_out_t2_closed("v_out_t2_closed");
    StringConstant draining("draining");
    StringConstant fill_T3("fill_T3");
    StringConstant not_to_empty("not_to_empty");

    //costanti
    RealConstant delta("delta",0.02_decimal);
    
    HybridAutomaton tank1_automaton = getT1();
    HybridAutomaton tank2_automaton = getT2();
    HybridAutomaton tank3_automaton = getT3();
    HybridAutomaton tank4_automaton = getT4();
    HybridAutomaton controller1_automaton = getController1();
    HybridAutomaton controller2_automaton = getController2();
    HybridAutomaton controller3_automaton = getController3();
    HybridAutomaton controller4_automaton = getController4();
    HybridAutomaton controller5_automaton = getController5();

    CompositeHybridAutomaton caffe_system({tank1_automaton,tank2_automaton,tank3_automaton,tank4_automaton,controller1_automaton,controller2_automaton,controller3_automaton,controller4_automaton,controller5_automaton});

    cout << caffe_system << endl;

    // Create a GeneralHybridEvolver object
    GeneralHybridEvolver evolver(caffe_system);
    evolver.verbosity = evolver_verbosity;

    // Set the evolution parameters
    evolver.configuration().set_maximum_enclosure_radius(3.05);
    evolver.configuration().set_maximum_step_size(0.20);

    // Declare the type to be used for the system evolution
    typedef GeneralHybridEvolver::OrbitType OrbitType;

    std::cout << "Computing evolution... " << std::flush;

    HybridSet initial_set({controller1|v_in_t1_opened,controller2|v_out_t1_closed,controller3|v_out_t2_closed,controller4|fill_T3,controller5|not_to_empty},{height_T1==2,height_T2==0,height_T3==10,height_T4==0});
    HybridTime evolution_time(50.0,30);
    OrbitType orbit = evolver.orbit(initial_set,evolution_time,Semantics::UPPER);
    std::cout << "done." << std::endl;

    std::cout << "Plotting trajectory... "<<std::flush;
    // plot height tank 1
    Axes2d time_height_T1_axes(0<=TimeVariable()<=evolution_time.continuous_time(),-0.13<=height_T1<=4.2);
    plot("caffè tank - height_T1",time_height_T1_axes, Colour(0.0,0.5,1.0), orbit);

    // plot height tank 2
    Axes2d time_height_T2_axes(0<=TimeVariable()<=evolution_time.continuous_time(),-0.1<=height_T2<=4.2);
    plot("caffè tank - height_T2",time_height_T2_axes, Colour(0.0,0.5,1.0), orbit);


   // plot height tank 2
    Axes2d time_height_T3_axes(0<=TimeVariable()<=evolution_time.continuous_time(),-0.1<=height_T3<=10.1);
    plot("caffè tank - height_T3",time_height_T3_axes, Colour(0.0,0.5,1.0), orbit);


    // plot height tank 4
    Axes2d time_height_T4_axes(0<=TimeVariable()<=evolution_time.continuous_time(),-0.1<=height_T4<=10.1);
    plot("caffè tank - height_T4",time_height_T4_axes, Colour(0.0,0.5,1.0), orbit);


    std::cout << "done." << std::endl;
}
