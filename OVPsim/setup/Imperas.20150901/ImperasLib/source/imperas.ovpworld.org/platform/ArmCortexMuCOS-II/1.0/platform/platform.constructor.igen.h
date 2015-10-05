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


// This file constructs the platform according to TCL script.
// This file should NOT need to be edited.

////////////////////////////////////////////////////////////////////////////////

void platformConstructor(void) {

    icmInitAttrs ArmCortexMuCOS_II_attrs = ICM_INIT_DEFAULT;

    icmInitPlatform(ICM_VERSION, ArmCortexMuCOS_II_attrs, 0, 0, "ArmCortexMuCOS-II");


    icmDocNodeP Root1_node = icmDocSectionAdd(0, "Root");
    {
        icmDocNodeP doc2_node = icmDocSectionAdd(Root1_node, "Description");
        icmDocTextAdd(doc2_node, "Platform for Micrium uc/OS-II bring up and simple LED access");
        icmDocNodeP doc_12_node = icmDocSectionAdd(Root1_node, "Licensing");
        icmDocTextAdd(doc_12_node, "Open Source Apache 2.0");
        icmDocNodeP doc_22_node = icmDocSectionAdd(Root1_node, "Limitations");
        icmDocTextAdd(doc_22_node, "BareMetal platform for bring up of uc/OS-II on ARM Cortex-M3 processor");
        icmDocNodeP doc_32_node = icmDocSectionAdd(Root1_node, "Reference");
        icmDocTextAdd(doc_32_node, "www.micrium.com/page/products/rtos/os-ii");
    }
    icmSetPlatformStatus(ICM_OVP,ICM_BAREMETAL,ICM_VISIBLE);

////////////////////////////////////////////////////////////////////////////////
//                                  Bus bus1
////////////////////////////////////////////////////////////////////////////////


    handles.bus1_b = icmNewBus( "bus1", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Processor cpu1
////////////////////////////////////////////////////////////////////////////////


    const char *cpu1_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "arm.ovpworld.org"  ,    // vendor
        0                   ,    // library
        "armm"              ,    // name
        0                   ,    // version
        "model"                  // model
    );

    icmAttrListP cpu1_attrList = icmNewAttrList();
    icmAddStringAttr(cpu1_attrList, "variant", "Cortex-M3");
    icmAddStringAttr(cpu1_attrList, "compatibility", "gdb");
    icmAddDoubleAttr(cpu1_attrList, "mips", 100.000000);
    icmAddStringAttr(cpu1_attrList, "endian", "little");

    const char *armNewlib_0_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "arm.ovpworld.org"  ,    // vendor
        0                   ,    // library
        "armNewlib"         ,    // name
        0                   ,    // version
        "model"                  // model
    );
    icmNewProcAttrs cpu1_attrs = ICM_ATTR_SIMEX;


    handles.cpu1_c = icmNewProcessor(
        "cpu1"              ,   // name
        0,
        0                   ,   // cpuId
        0x0000              ,  // flags
        32                  ,   // address bits
        cpu1_path           ,   // model
        0, 
        cpu1_attrs          ,   // procAttrs
        cpu1_attrList       ,   // attrlist
        armNewlib_0_path    ,   // semihost file
        0
    );


////////////////////////////////////////////////////////////////////////////////
//                                   PSE led
////////////////////////////////////////////////////////////////////////////////


    const char *led_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "ovpworld.org"      ,    // vendor
        0                   ,    // library
        "ledRegister"       ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP led_attrList = icmNewAttrList();

    handles.led_p = icmNewPSE(
        "led"               ,   // name
        led_path            ,   // model
        led_attrList        ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.led_p, handles.bus1_b, "busPort", 0, 0x10000000, 0x10000007);
    icmSetPSEdiagnosticLevel(handles.led_p, 1);


    icmConnectProcessorBusByName( handles.cpu1_c, "INSTRUCTION", handles.bus1_b );

    icmConnectProcessorBusByName( handles.cpu1_c, "DATA", handles.bus1_b );

////////////////////////////////////////////////////////////////////////////////
//                               Memory memory1
////////////////////////////////////////////////////////////////////////////////

    handles.memory1_m = icmNewMemory("memory1", 0x7, 0xfffffff);


    icmConnectMemoryToBus( handles.bus1_b, "sp1", handles.memory1_m, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                               Memory memory2
////////////////////////////////////////////////////////////////////////////////

    handles.memory2_m = icmNewMemory("memory2", 0x7, 0xdfffffff);


    icmConnectMemoryToBus( handles.bus1_b, "sp1", handles.memory2_m, 0x20000000);

    if(!icmLoadProcessorMemory(handles.cpu1_c, "ucosiiDemoApplication.ARM_CORTEX_M3.elf", 0, 0, 0)){
        icmPrintf("%s", icmLastMessage());
        icmTerminate();
        icmExitSimulation(1);
    }
}