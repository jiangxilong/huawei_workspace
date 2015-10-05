/*
 *
 * Copyright (c) 2005-2015 Imperas Software Ltd., www.imperas.com
 *
 * The contents of this file are provided under the Software License
 * Agreement that you accepted before downloading this file.
 *
 * This source forms part of the Software and can be used for educational,
 * training, and demonstration purposes but cannot be used for derivative
 * works except in cases where the derivative works require OVP technology
 * to run.
 *
 * For open source models released under licenses that you can use for
 * derivative works, please visit www.OVPworld.org or www.imperas.com
 * for the location of the open source models.
 *
 */


////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                              Version 99999999
//                          Fri Sep 20 12:37:13 2013
//
////////////////////////////////////////////////////////////////////////////////

// This file constructs the platform.
// This file should NOT need to be edited.

#include "ovpworld.org/modelSupport/tlmDecoder/1.0/tlm2.0/tlmDecoder.hpp"
#include "ovpworld.org/memory/ram/1.0/tlm2.0/tlmMemory.hpp"
#include "arm.ovpworld.org/processor/arm/1.0/tlm2.0/arm_Cortex-A9UP.igen.hpp"
#include "arm.ovpworld.org/peripheral/UartPL011/1.0/tlm2.0/pse.igen.hpp"


////////////////////////////////////////////////////////////////////////////////
//                            alwaysNonSecure Class
////////////////////////////////////////////////////////////////////////////////

class alwaysNonSecure : public sc_core::sc_module {

  public:
    alwaysNonSecure (sc_core::sc_module_name name);

    icmTLMPlatform        Platform;
    decoder             <2,3> tzBus;
    decoder             <3,2> pBus;
    ram                   ram0;
    arm_Cortex_A9UP       cpu;
    UartPL011             uart0;


    icmAttrListObject *attrsForcpu() {
        icmAttrListObject *userAttrs = new icmAttrListObject;
        userAttrs->addAttr("variant", "Cortex-A9UP");
        userAttrs->addAttr("compatibility", "nopSVC");
        userAttrs->addAttr("endian", "little");
        return userAttrs;
    }

    icmAttrListObject *attrsForuart0() {
        icmAttrListObject *userAttrs = new icmAttrListObject;
        userAttrs->addAttr("outfile", "uart0.log");
        return userAttrs;
    }
}; /* alwaysNonSecure */

////////////////////////////////////////////////////////////////////////////////
//                         alwaysNonSecure Constructor
////////////////////////////////////////////////////////////////////////////////

alwaysNonSecure::alwaysNonSecure ( sc_core::sc_module_name name)
    : sc_core::sc_module (name)
    , Platform ("icm", ICM_VERBOSE | ICM_STOP_ON_CTRLC| ICM_ENABLE_IMPERAS_INTERCEPTS )
    , tzBus("tzBus")
    , pBus("pBus")
    , ram0 ("ram0", "sp1", 0x200000)
    , cpu ( "cpu", 0, ICM_ATTR_DEFAULT,attrsForcpu()    )
    , uart0 ("uart0", attrsForuart0())
{

    // tzBus masters
    cpu.INSTRUCTION.socket(tzBus.target_socket[0]);
    cpu.DATA.socket(tzBus.target_socket[1]);

    // tzBus slaves
    tzBus.initiator_socket[0](pBus.target_socket[0]); // Bridge
    tzBus.setDecode(0, 0x0, 0xffffffff);

    tzBus.initiator_socket[1](pBus.target_socket[1]); // Bridge
    tzBus.setDecode(1, 0x10000000000ULL, 0x100001fffffULL);

    tzBus.initiator_socket[2](pBus.target_socket[2]); // Bridge
    tzBus.setDecode(2, 0x10010000000ULL, 0x10010000fffULL);


    // pBus masters

    // pBus slaves
    pBus.initiator_socket[0](uart0.bport1.socket); // Peripheral
    pBus.setDecode(0, 0x10000000, 0x10000fff);

    pBus.initiator_socket[1](ram0.sp1); // Memory
    pBus.setDecode(1, 0x0, 0x1fffff);

}