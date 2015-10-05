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

#ifndef PSE_MACROS_IGEN_H
#define PSE_MACROS_IGEN_H
// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'BPORT1' to the base of port 'bport1'
#ifndef BPORT1
    #error BPORT1 is undefined.It needs to be set to the port base address
#endif
#define BPORT1_AB_C1    (BPORT1 + 0x0)

#define BPORT1_AB_C1_CLKS   (0x3 << 6)
#define BPORT1_AB_C1_FRDIV   (0x7 << 3)
#define BPORT1_AB_C1_IRCLKEN   (0x1 << 1)
#define BPORT1_AB_C1_IREFS   (0x1 << 2)
#define BPORT1_AB_C1_IREFSTEN   0x1
#define BPORT1_AB_C2    (BPORT1 + 0x1)

#define BPORT1_AB_C2_EREFS0   (0x1 << 2)
#define BPORT1_AB_C2_HGO0   (0x1 << 3)
#define BPORT1_AB_C2_IRCS   0x1
#define BPORT1_AB_C2_LOCRE0   (0x1 << 7)
#define BPORT1_AB_C2_LP   (0x1 << 1)
#define BPORT1_AB_C2_RANGE0   (0x3 << 4)
#define BPORT1_AB_C3    (BPORT1 + 0x2)

#define BPORT1_AB_C3_SCTRIM   0xff
#define BPORT1_AB_C4    (BPORT1 + 0x3)

#define BPORT1_AB_C4_DMX32   (0x1 << 7)
#define BPORT1_AB_C4_DRST_DRS   (0x3 << 5)
#define BPORT1_AB_C4_FCTRIM   (0xf << 1)
#define BPORT1_AB_C4_SCFTRIM   0x1
#define BPORT1_AB_C5    (BPORT1 + 0x4)

#define BPORT1_AB_C5_PLLCLKEN0   (0x1 << 6)
#define BPORT1_AB_C5_PLLREFSEL0   (0x1 << 7)
#define BPORT1_AB_C5_PLLSTEN0   (0x1 << 5)
#define BPORT1_AB_C5_PRDIV0   0x7
#define BPORT1_AB_C6    (BPORT1 + 0x5)

#define BPORT1_AB_C6_CME0   (0x1 << 5)
#define BPORT1_AB_C6_LOLIE0   (0x1 << 7)
#define BPORT1_AB_C6_PLLS   (0x1 << 6)
#define BPORT1_AB_C6_VDIV0   0x1f
#define BPORT1_AB_S    (BPORT1 + 0x6)

#define BPORT1_AB_S_CLKST   (0x3 << 2)
#define BPORT1_AB_S_IRCST   0x1
#define BPORT1_AB_S_IREFST   (0x1 << 4)
#define BPORT1_AB_S_LOCK0   (0x1 << 6)
#define BPORT1_AB_S_LOLS0   (0x1 << 7)
#define BPORT1_AB_S_OSCINIT0   (0x1 << 1)
#define BPORT1_AB_S_PLLST   (0x1 << 5)
#define BPORT1_AB_SC    (BPORT1 + 0x8)

#define BPORT1_AB_SC_ATME   (0x1 << 7)
#define BPORT1_AB_SC_ATMF   (0x1 << 5)
#define BPORT1_AB_SC_ATMS   (0x1 << 6)
#define BPORT1_AB_SC_FCIRDIV   (0x7 << 1)
#define BPORT1_AB_SC_FLTPRSRV   (0x1 << 4)
#define BPORT1_AB_SC_LOCS0   0x1
#define BPORT1_AB_ATCVH    (BPORT1 + 0xa)

#define BPORT1_AB_ATCVH_ATCVH   0xff
#define BPORT1_AB_ATCVL    (BPORT1 + 0xb)

#define BPORT1_AB_ATCVL_ATCVL   0xff
#define BPORT1_AB_C7    (BPORT1 + 0xc)

#define BPORT1_AB_C7_OSCSEL   0x1
#define BPORT1_AB_C8    (BPORT1 + 0xd)

#define BPORT1_AB_C8_CME1   (0x1 << 5)
#define BPORT1_AB_C8_LOCRE1   (0x1 << 7)
#define BPORT1_AB_C8_LOCS1   0x1
#define BPORT1_AB_C10    (BPORT1 + 0xf)

#define BPORT1_AB_C10_EREFS1   (0x1 << 2)
#define BPORT1_AB_C10_HGO1   (0x1 << 3)
#define BPORT1_AB_C10_LOCRE2   (0x1 << 7)
#define BPORT1_AB_C10_RANGE1   (0x3 << 4)
#define BPORT1_AB_C11    (BPORT1 + 0x10)

#define BPORT1_AB_C11_PLLCLKEN1   (0x1 << 6)
#define BPORT1_AB_C11_PLLCS   (0x1 << 4)
#define BPORT1_AB_C11_PLLREFSEL1   (0x1 << 7)
#define BPORT1_AB_C11_PLLSTEN1   (0x1 << 5)
#define BPORT1_AB_C11_PRDIV1   0x7
#define BPORT1_AB_C12    (BPORT1 + 0x11)

#define BPORT1_AB_C12_CME2   (0x1 << 5)
#define BPORT1_AB_C12_LOLIE1   (0x1 << 7)
#define BPORT1_AB_C12_VDIV1   0x1f
#define BPORT1_AB_S2    (BPORT1 + 0x12)

#define BPORT1_AB_S2_LOCK2   (0x1 << 6)
#define BPORT1_AB_S2_LOCS2   0x1
#define BPORT1_AB_S2_LOLS2   (0x1 << 7)
#define BPORT1_AB_S2_OSCINIT2   (0x1 << 1)
#define BPORT1_AB_S2_PLLCST   (0x1 << 4)


#endif