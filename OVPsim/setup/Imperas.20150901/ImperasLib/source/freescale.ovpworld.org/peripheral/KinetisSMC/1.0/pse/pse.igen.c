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


#include "pse.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

bport1_ab_dataT bport1_ab_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel >= 1) bhmMessage("I", "KinetisSMC", "Example");
//     Predefined macros PSE_DIAG_LOW, PSE_DIAG_MEDIUM and PSE_DIAG_HIGH may be used
Uns32 diagnosticLevel;

/////////////////////////// Diagnostic level callback //////////////////////////

static void setDiagLevel(Uns32 new) {
    diagnosticLevel = new;
}

///////////////////////////// MMR Generic callbacks ////////////////////////////

static PPM_VIEW_CB(view8) {  *(Uns8*)data = *(Uns8*)user; }

static PPM_READ_CB(read_8) {  return *(Uns8*)user; }

//////////////////////////////// Mask functions ////////////////////////////////

static PPM_WRITE_CB(bport1_ab_PMCTRL_maskWrite) {
    bport1_ab_data.PMCTRL.value = (bport1_ab_data.PMCTRL.value & 0x18) | (data & 0xe7);
}

static PPM_WRITE_CB(bport1_ab_PMPROT_maskWrite) {
    bport1_ab_data.PMPROT.value = (bport1_ab_data.PMPROT.value & 0xd5) | (data & 0x2a);
}

static PPM_WRITE_CB(bport1_ab_VLLSCTRL_maskWrite) {
    bport1_ab_data.VLLSCTRL.value = (bport1_ab_data.VLLSCTRL.value & 0xf8) | (data & 0x7);
}

//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
    handles.bport1 = ppmCreateSlaveBusPort("bport1", 4096);

}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {

    ppmCreateRegister("ab_PMPROT",
        "Power Mode Protection Register, offset: 0x0 ",
        handles.bport1,
        0,
        1,
        read_8,
        bport1_ab_PMPROT_maskWrite,
        view8,
        &(bport1_ab_data.PMPROT.value),
        True
    );
    ppmCreateRegister("ab_PMCTRL",
        "Power Mode Control Register, offset: 0x1 ",
        handles.bport1,
        1,
        1,
        read_8,
        bport1_ab_PMCTRL_maskWrite,
        view8,
        &(bport1_ab_data.PMCTRL.value),
        True
    );
    ppmCreateRegister("ab_VLLSCTRL",
        "VLLS Control Register, offset: 0x2 ",
        handles.bport1,
        2,
        1,
        read_8,
        bport1_ab_VLLSCTRL_maskWrite,
        view8,
        &(bport1_ab_data.VLLSCTRL.value),
        True
    );
    ppmCreateRegister("ab_PMSTAT",
        "Power Mode Status Register, offset: 0x3 ",
        handles.bport1,
        3,
        1,
        read_8,
        0,
        view8,
        &(bport1_ab_data.PMSTAT.value),
        True
    );

}

/////////////////////////////////// Net Ports //////////////////////////////////

static void installNetPorts(void) {
    handles.Reset = ppmOpenNetPort("Reset");
    if (handles.Reset) {
        ppmInstallNetCallback(handles.Reset, reset_Reset, (void*)0);
    }

}

PPM_NET_CB(reset_Reset) {
    if(value != 0 ) {
        bport1_ab_data.PMSTAT.value = 0x1;
        bport1_ab_data.VLLSCTRL.value = 0x3;
        bport1_ab_data.PMCTRL.value = 0x0;
        bport1_ab_data.PMPROT.value = 0x0;
    }
}

////////////////////////////////// Constructor /////////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor) {
    installSlavePorts();
    installRegisters();
    installNetPorts();
    reset_Reset(1,0);
}

///////////////////////////////////// Main /////////////////////////////////////

int main(int argc, char *argv[]) {

    ppmDocNodeP Root1_node = ppmDocAddSection(0, "Root");
    {
        ppmDocNodeP doc2_node = ppmDocAddSection(Root1_node, "Description");
        ppmDocAddText(doc2_node, 0);
        ppmDocNodeP doc_12_node = ppmDocAddSection(Root1_node, "Limitations");
        ppmDocAddText(doc_12_node, 0);
        ppmDocNodeP doc_22_node = ppmDocAddSection(Root1_node, "Licensing");
        ppmDocAddText(doc_22_node, "Open Source Apache 2.0");
    }

    diagnosticLevel = 0;
    bhmInstallDiagCB(setDiagLevel);
    constructor();

    bhmWaitEvent(bhmGetSystemEvent(BHM_SE_END_OF_SIMULATION));
    return 0;
}
