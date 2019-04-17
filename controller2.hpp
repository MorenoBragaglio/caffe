/***************************************************************************
 *            controller2-urgent.hpp
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

inline AtomicHybridAutomaton getController2()
{

    // Declare some constants. Note that system parameters should be given as variables.
    RealConstant T("T",1.5_decimal);
    RealConstant hmax("hmax",4);
    RealConstant delta("delta",0.1_decimal);

    // Declare the shared system variables
    RealVariable aperture_V_OUT_T1("aperture_V_OUT_T1");
    RealVariable height_T2("height_T2");
    RealVariable height_T1("height_T1");

    // Declare the events we use
    DiscreteEvent D("D");
    DiscreteEvent E("E");
    DiscreteEvent F("F");
    DiscreteEvent v_out_t1_can_open("v_out_t1_can_open");
    DiscreteEvent v_out_t1_is_closed("v_out_t1_is_closed");

    AtomicHybridAutomaton controller2("controller2");

    // Declare the values the controller2 can variable can have
    AtomicDiscreteLocation v_out_t1_closed("v_out_t1_closed");
    AtomicDiscreteLocation v_out_t1_opening("v_out_t1_opening");
    AtomicDiscreteLocation v_out_t1_opened("v_out_t1_opened");
    AtomicDiscreteLocation v_out_t1_closing("v_out_t1_closing");
    
    
    // Since aperture is a known constant when the controller2 is open or closed,
    // specify aperture by an algebraic equation.
    controller2.new_mode(v_out_t1_closed,{
            let(aperture_V_OUT_T1)=0});
    controller2.new_mode(v_out_t1_opening,{
            dot(aperture_V_OUT_T1)=1/T});
    controller2.new_mode(v_out_t1_opened,{
            let(aperture_V_OUT_T1)=1});
    controller2.new_mode(v_out_t1_closing,{
            dot(aperture_V_OUT_T1)=-1/T});
    


    controller2.new_transition(v_out_t1_closed,v_out_t1_is_closed,v_out_t1_opening,{
            next(aperture_V_OUT_T1)=aperture_V_OUT_T1},
            false,EventKind::URGENT);
    controller2.new_transition(v_out_t1_opening,v_out_t1_is_closed,v_out_t1_opened,
            false,EventKind::URGENT);
    controller2.new_transition(v_out_t1_opened,v_out_t1_is_closed,v_out_t1_closing,{
            next(aperture_V_OUT_T1)=aperture_V_OUT_T1},
            false,EventKind::URGENT);



    controller2.new_transition(v_out_t1_closed,v_out_t1_can_open,v_out_t1_opening,{
            next(aperture_V_OUT_T1)=aperture_V_OUT_T1});
    controller2.new_transition(v_out_t1_opening,D,v_out_t1_opened,
            aperture_V_OUT_T1>=1-delta,EventKind::URGENT); 
    controller2.new_transition(v_out_t1_opened,E,v_out_t1_closing,{
            next(aperture_V_OUT_T1)=aperture_V_OUT_T1},
            height_T2>=hmax-delta,EventKind::URGENT);
            //height_T1<=0+delta,EventKind::URGENT);
    controller2.new_transition(v_out_t1_closing,v_out_t1_is_closed,v_out_t1_closed,
            aperture_V_OUT_T1<=0+delta,EventKind::URGENT);   

    
    
    return controller2;
}
