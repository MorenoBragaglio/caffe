/***************************************************************************
 *            tank.hpp
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

inline HybridAutomaton getT4()
{

    //PER ORA LASCIO COSI, IL PRIMO GIRO COMPLETO DEVE FUNZIONARE LO STESSO
    //UNA VOLTA CHE VA IL PRIMO GIRO S PENSERA' A RESETTARE IL TUTTO SVUOTANDO T4
    RealConstant rate("rate",0.3_decimal);

    RealVariable aperture_V_OUT_T2("aperture_V_OUT_T2");
    RealVariable aperture_V_OUT_T3("aperture_V_OUT_T3");
    RealVariable aperture_V_OUT_T4("aperture_V_OUT_T4");
    RealVariable height_T4("height_T4");

    // Create the tank object
    HybridAutomaton t4("t4");

    // Declare a trivial discrete location.
    DiscreteLocation draining;

    // The water level is always given by the same dynamic
    // The inflow is controlled by the valve aperture, the outflow depends on the
    // pressure, which is proportional to the water height.
    t4.new_mode(draining,{dot(height_T4) = rate*aperture_V_OUT_T2 + rate*aperture_V_OUT_T3});


    return t4;
}