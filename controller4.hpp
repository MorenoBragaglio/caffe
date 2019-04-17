/***************************************************************************
 *            controller4-urgent.hpp
 *
 *  Copyright  2017 Luca Geretti
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

using namespace Ariadne;

inline AtomicHybridAutomaton getController4()
{

    // Declare some constants. Note that system parameters should be given as variables.
    RealConstant T("T",2.0_decimal);
    RealConstant hmax_middle("hmax_middle",6);
    RealConstant delta("delta",0.1_decimal);

    // Declare the shared system variables
    RealVariable aperture_V_IN_T3("aperture_V_IN_T3");
    RealVariable aperture_V_OUT_T3("aperture_V_OUT_T3");
    RealVariable height_T4("height_T4");

    // Declare the events we use
    DiscreteEvent start_towards_T4_A("start_towards_T4_A");
    DiscreteEvent finished_towards_T4_A("finished_towards_T4_A");
    DiscreteEvent start_towards_T3("start_towards_T3");
    DiscreteEvent finished_towards_T3("finished_towards_T3");
    
    AtomicHybridAutomaton controller4("controller4");

    // Declare the values the controller4 can variable can have
    AtomicDiscreteLocation fill_T3("fill_T3");
    AtomicDiscreteLocation towards_T3("towards_T3");
    AtomicDiscreteLocation fill_T4_A("fill_T4_A");
    AtomicDiscreteLocation towards_T4_A("towards_T4_A");
    
    // Since aperture is a known constant when the controller4 is open or closed,
    // specify aperture by an algebraic equation.
    controller4.new_mode(fill_T3,{
            let(aperture_V_IN_T3)=0,
            let(aperture_V_OUT_T3)=0});
    controller4.new_mode(towards_T4_A,{
            dot(aperture_V_IN_T3)=1/T,
            dot(aperture_V_OUT_T3)=1/T});
    controller4.new_mode(fill_T4_A,{
            let(aperture_V_IN_T3)=1,
            let(aperture_V_OUT_T3)=1});
    controller4.new_mode(towards_T3,{
            dot(aperture_V_IN_T3)=-1/T,
            dot(aperture_V_OUT_T3)=-1/T});


    controller4.new_transition(fill_T3,start_towards_T4_A,towards_T4_A,{
            next(aperture_V_IN_T3)=aperture_V_IN_T3,
            next(aperture_V_OUT_T3)=aperture_V_OUT_T3},
            height_T4<=0+delta,EventKind::URGENT);
    controller4.new_transition(towards_T4_A,finished_towards_T4_A,fill_T4_A,
            aperture_V_IN_T3>=1-delta,EventKind::URGENT);
    controller4.new_transition(fill_T4_A,start_towards_T3,towards_T3,{
            next(aperture_V_IN_T3)=aperture_V_IN_T3,
            next(aperture_V_OUT_T3)=aperture_V_OUT_T3},
            height_T4>=hmax_middle-delta,EventKind::URGENT);
    controller4.new_transition(towards_T3,finished_towards_T3,fill_T3,
            aperture_V_IN_T3<=0+delta,EventKind::URGENT);

    return controller4;
}
