/***************************************************************************
 *            controller3-urgent.hpp
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

inline AtomicHybridAutomaton getController3()
{

    // Declare some constants. Note that system parameters should be given as variables.
    RealConstant T("T",2.0_decimal);
    RealConstant hmax("hmax",4);
    RealConstant hmax_middle("hmax_middle",6);
    RealConstant delta("delta",0.1_decimal);

    // Declare the shared system variables
    RealVariable aperture_V_OUT_T2("aperture_V_OUT_T2");
    RealVariable height_T4("height_T4");

    // Declare the events we use
    DiscreteEvent F("F");
    DiscreteEvent G("G");
    DiscreteEvent H("H");
    DiscreteEvent I("I");
    DiscreteEvent v_out_t1_is_closed("v_out_t1_is_closed");

    AtomicHybridAutomaton controller3("controller3");

    // Declare the values the controller3 can variable can have
    AtomicDiscreteLocation v_out_t2_opened("v_out_t2_opened");
    AtomicDiscreteLocation v_out_t2_closing("v_out_t2_closing");
    AtomicDiscreteLocation v_out_t2_closed("v_out_t2_closed");
    AtomicDiscreteLocation waiting_v_out_t3("waiting_v_out_t3");
    AtomicDiscreteLocation v_out_t2_opening("v_out_t2_opening");

    // Since aperture is a known constant when the controller3 is open or closed,
    // specify aperture by an algebraic equation.
    controller3.new_mode(v_out_t2_closed,{
            let(aperture_V_OUT_T2)=0});
    controller3.new_mode(waiting_v_out_t3,{
            dot(aperture_V_OUT_T2)=0});
    controller3.new_mode(v_out_t2_opening,{
            dot(aperture_V_OUT_T2)=1/T});
    controller3.new_mode(v_out_t2_opened,{
            let(aperture_V_OUT_T2)=1});
    controller3.new_mode(v_out_t2_closing,{
            dot(aperture_V_OUT_T2)=-1/T});
    

    
    controller3.new_transition(v_out_t2_closing,H,v_out_t2_closed,
            false,EventKind::URGENT);
    controller3.new_transition(v_out_t2_closed,H,waiting_v_out_t3,{
            next(aperture_V_OUT_T2)=aperture_V_OUT_T2},
            false,EventKind::URGENT);
    controller3.new_transition(waiting_v_out_t3,H,v_out_t2_opening,{
            next(aperture_V_OUT_T2)=aperture_V_OUT_T2},
            false,EventKind::URGENT);
    controller3.new_transition(v_out_t2_opening,H,v_out_t2_opened,
            false,EventKind::URGENT);


    controller3.new_transition(v_out_t2_closed,I,waiting_v_out_t3,{
            next(aperture_V_OUT_T2)=aperture_V_OUT_T2},
            false,EventKind::URGENT);
    controller3.new_transition(waiting_v_out_t3,I,v_out_t2_opening,{
            next(aperture_V_OUT_T2)=aperture_V_OUT_T2},
            false,EventKind::URGENT);
    controller3.new_transition(v_out_t2_opening,I,v_out_t2_opened,
            false,EventKind::URGENT);
    controller3.new_transition(v_out_t2_opened,I,v_out_t2_closing,{
            next(aperture_V_OUT_T2)=aperture_V_OUT_T2},
            false,EventKind::URGENT);

    controller3.new_transition(v_out_t2_closed,v_out_t1_is_closed,waiting_v_out_t3,{
            next(aperture_V_OUT_T2)=aperture_V_OUT_T2});
    controller3.new_transition(waiting_v_out_t3,F,v_out_t2_opening,{
            next(aperture_V_OUT_T2)=aperture_V_OUT_T2},
            height_T4>=hmax_middle,EventKind::URGENT);
    controller3.new_transition(v_out_t2_opening,G,v_out_t2_opened,
            aperture_V_OUT_T2>=1-delta,EventKind::URGENT);
    controller3.new_transition(v_out_t2_opened,H,v_out_t2_closing,{
            next(aperture_V_OUT_T2)=aperture_V_OUT_T2},
            height_T4>=hmax+hmax_middle-delta,EventKind::URGENT);
    controller3.new_transition(v_out_t2_closing,I,v_out_t2_closed,
            aperture_V_OUT_T2<=0+delta,EventKind::URGENT);


    return controller3;
}
