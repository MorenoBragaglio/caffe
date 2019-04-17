/***************************************************************************
 *            controller5-urgent.hpp
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

inline AtomicHybridAutomaton getController5()
{

    // Declare some constants. Note that system parameters should be given as variables.
    RealConstant hmax("hmax",4);
    RealConstant hmax_middle("hmax_middle",6);
    RealConstant delta("delta",0.1_decimal);

    // Declare the shared system variables
    RealVariable dummy("dummy");
    RealVariable height_T4("height_T4");

    // Declare the events we use
    DiscreteEvent L("L");
    DiscreteEvent H("H");

    AtomicHybridAutomaton controller5("controller5");

    // Declare the values the controller5 can variable can have
    AtomicDiscreteLocation not_to_empty("not_to_empty");
    AtomicDiscreteLocation to_empty("to_empty");
 
    
    // Since aperture is a known constant when the controller5 is open or closed,
    // specify aperture by an algebraic equation.
    controller5.new_mode(not_to_empty,{
            let(dummy)=1});
    controller5.new_mode(to_empty,{
            let(height_T4)=0});    


    controller5.new_transition(not_to_empty,H,to_empty);
    controller5.new_transition(to_empty,L,not_to_empty,
            true,EventKind::URGENT); 
   
    
    
    return controller5;
}
