/*
 * Copyright (c) 2005-2015 Imperas Software Ltd., www.imperas.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150901.0
//
////////////////////////////////////////////////////////////////////////////////

// MODEL IO:
//    Slave Port bport1
//    Net output  irq0;
//    Net output  irq1;
//    Net output  cardin;
//    Net output  wprot;

#ifndef ARM_OVPWORLD_ORG_PERIPHERAL_MMCIPL181__1_0
#define ARM_OVPWORLD_ORG_PERIPHERAL_MMCIPL181__1_0
#include "ovpworld.org/modelSupport/tlmPeripheral/1.0/tlm2.0/tlmPeripheral.hpp"
using namespace sc_core;

class MmciPL181 : public icmPeripheral
{
  private:
    const char *getModel() {
        return icmGetVlnvString (NULL, "arm.ovpworld.org", "peripheral", "MmciPL181", "1.0", "pse");
    }

  public:
    icmSlavePort        bport1;
    icmOutputNetPort irq0;
    icmOutputNetPort irq1;
    icmOutputNetPort cardin;
    icmOutputNetPort wprot;

    MmciPL181(sc_module_name name, icmAttrListObject *initialAttrs = 0 )
        : icmPeripheral(name, getModel(), 0, initialAttrs)
        , bport1(this, "bport1", 0x1000) // static
        , irq0(this, "irq0")
        , irq1(this, "irq1")
        , cardin(this, "cardin")
        , wprot(this, "wprot")
    {
    }

}; /* class MmciPL181 */

#endif