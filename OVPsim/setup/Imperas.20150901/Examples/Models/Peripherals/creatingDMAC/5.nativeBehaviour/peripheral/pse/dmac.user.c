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
//                          Wed Aug 18 12:57:18 2010
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

#include "peripheral/impTypes.h"
#include "peripheral/bhm.h"
#include "peripheral/ppm.h"
#include "dmac.igen.h"

char title[BHM_MAX_PATH] = "";

#define THREAD_STACK	  (8*1024)
#define NUM_CHANNELS	  2
#define BYTES_PER_ACCESS  8

// Give a 'nice' name to the default generated
#define controlRegs DMACSP_ab8_dataT
#define channelRegs DMACSP_ab32ch0_dataT

controlRegs *control;
channelRegs *ch[2];

typedef struct {
	bhmThreadHandle       thread;
	bhmEventHandle        start;
	Bool                  busy;
	char                  stack[THREAD_STACK];
} channelState;

typedef struct {
    ppmAddressSpaceHandle readHandle;
    ppmAddressSpaceHandle writeHandle;
    ppmNetHandle          intTCHandle;
    Bool                  intTCAsserted;
    Bool                  inReset;
	channelState          ch[NUM_CHANNELS];
} dmaState, *dmaStateP;

dmaState DMAState;

Uns32 readNativeDataChannel = -1;

static void updateStatus(void);

static inline Uns32 byteSwap(Uns32 data){
#ifdef ENDIANBIG
    return
        ((data & 0xff000000) >> 24) |
        ((data & 0x00ff0000) >>  8) |
        ((data & 0x0000ff00) <<  8) |
        ((data & 0x000000ff) << 24);
#else
    return data;
#endif
}


static void writeAndStart(Uns8 channel, Uns32 data)
{
    ch[channel]->config.value = byteSwap(data);
    if (diagnosticLevel >= 3) {
        bhmMessage("I", "DMAC",
            "configCh%uWr 0x%08x enable %d interrupts %d halt %d",
            channel, ch[channel]->config.value,
            ch[channel]->config.bits.enable,
            ch[channel]->config.bits.inten,
            ch[channel]->config.bits.halt
        );
    }
    if(!ch[channel]->config.bits.halt && ch[channel]->config.bits.enable && !DMAState.ch[channel].busy) {
        bhmTriggerEvent(DMAState.ch[channel].start);
    }
    updateStatus();
}


//////////////////////////////// Callback stubs ////////////////////////////////


PPM_REG_WRITE_CB(configCh0Wr) {
    writeAndStart(0, data);
}

PPM_REG_WRITE_CB(configCh1Wr) {
    writeAndStart(1, data);
}


PPM_REG_WRITE_CB(TCclearWr) {
    control->intTCstatus.value = 0;
    if (diagnosticLevel >= 3) {
        bhmMessage("I", "DMAC",
            "TCclearWr"
        );
    }
}


PPM_REG_WRITE_CB(configWr) {
    control->config.value = data;
    if (diagnosticLevel >= 3) {
        bhmMessage("I", "DMAC",
            "configWr 0x%02x (%d) burst size %d",
            control->config.value,
            control->config.value,
            1 << control->config.bits.burstSize

        );
    }
}

PPM_REG_WRITE_CB(errClearWr) {
    control->intErrStatus.value = 0;
}

PPM_REG_READ_CB(regRd32) {
    Uns32 data = byteSwap(*(Uns32*)user);

    if (diagnosticLevel >= 3) {
        bhmMessage("I", "DMAC",
            "regRd32 0x%08x (%u)",
            data, data
        );
    }
    return data;
}

PPM_REG_READ_CB(regRd8) {
    if (diagnosticLevel >= 3) {
        bhmMessage("I", "DMAC",
            "regRd8 0x%02x (%u)",
            *(Uns8*)user,
            *(Uns8*)user
        );
    }
    return *(Uns8*)user;
}

PPM_REG_WRITE_CB(regWr32) {
    *(Uns32*)user =  byteSwap(data);
    if (diagnosticLevel >= 3) {
        bhmMessage("I", "DMAC",
            "regWr32 0x%08x (%u)",
            *(Uns32*)user,
            *(Uns32*)user
        );
    }

}


//define no inline attribute for intercepted functions
#define NOINLINE  __attribute__((noinline))

//
// Semihosted function: passes initialisation info
//
NOINLINE Uns32 initSemiHost(Uns32 addressRegisters, Uns32 diag)
{
    bhmMessage("F", "PSE", "Failed to intercept initSemiHost");

    return 0;
}

//
// Semihosted function: passes initialisation info
//
NOINLINE Uns32 transferDataNative(Uns32 src, Uns32 dest, Uns32 thisAccess)
{
    bhmMessage("F", "PSE", "Failed to intercept transferDataNative");

    return thisAccess;
}

#define CH0_INT (1<<0)
#define CH1_INT (1<<1)

static void updateStatus(void)
{
    // Update intTCStatus from rawIntTCStatus
    Uns32 rawIntTCStatus = control->rawTCstatus.value;
    Uns32 intTCStatus = 0;

    if ((ch[0]->config.bits.inten) && (rawIntTCStatus & CH0_INT)) {
        intTCStatus |= CH0_INT;
    }

    if ((ch[1]->config.bits.inten) && (rawIntTCStatus & CH1_INT)) {
        intTCStatus |= CH1_INT;
    }
    control->intTCstatus.value = intTCStatus;

    // Update TC interrupt line
    if (DMAState.intTCHandle) {
        if (intTCStatus && !DMAState.intTCAsserted) {
            ppmWriteNet(DMAState.intTCHandle, 1);
            DMAState.intTCAsserted = True;
        } else if (!intTCStatus && DMAState.intTCAsserted) {
            ppmWriteNet(DMAState.intTCHandle, 0);
            DMAState.intTCAsserted = False;
        }
    }
}

static void dmaBurst(Uns32 channel)
{
    Uns32 bytes;
    Uns32 src;
    Uns32 dest;
    char buff[BYTES_PER_ACCESS];

    bytes = ch[channel]->control.bits.transferSize;
    src   = ch[channel]->srcAddr.value;
    dest  = ch[channel]->dstAddr.value;
    Uns32 burst = 1 << control->config.bits.burstSize;

    if (diagnosticLevel >= 2) {
        bhmMessage("I", "DMAC",
            "%s ch %u %u bytes burst size %u from %08x to %08x",
            title, channel, bytes, burst, src, dest
        );
    }
    while (bytes) {
        Uns32 thisAccess = (bytes > burst) ? burst : bytes;
        if(readNativeDataChannel == channel ) {
            thisAccess = transferDataNative(src, dest, thisAccess);
        } else {
            ppmReadAddressSpace (DMAState.readHandle,  src,  thisAccess, buff);
            ppmWriteAddressSpace(DMAState.writeHandle, dest, thisAccess, buff);
        }
        src   += thisAccess;
        dest  += thisAccess;
        bytes -= thisAccess;
        if (diagnosticLevel >= 3) {
            bhmMessage("I", "DMAC",
                "%s ch %u access %u bytes from %08x to %08x",
                title, channel, thisAccess, src, dest
            );
        }
        ch[channel]->srcAddr.value = src;
        ch[channel]->dstAddr.value = dest;
        bhmWaitDelay(1);
    }
}

// Thread for each channel
static void channelThread(void *user)
{
    Uns32 ch = (Uns32) user;
    for (;;) {
        if (diagnosticLevel >= 2) bhmMessage("I", "DMAC", "%s ch %u waiting", title, ch);
        DMAState.ch[ch].busy = False;

        bhmWaitEvent(DMAState.ch[ch].start);
        if (DMAState.inReset) {
            if (diagnosticLevel >= 2) bhmMessage("I", "DMAC", "%s In reset", title);
        } else {
            DMAState.ch[ch].busy = True;
            if (diagnosticLevel >= 2) bhmMessage("I", "DMAC", "%s ch %u running", title, ch);

            // Perform DMA burst
            dmaBurst(ch);

            control->rawTCstatus.value |= (1 << ch);
            if (diagnosticLevel >= 2)
                bhmMessage("I", "DMAC",
                    "ch %u status=0x%x",
                    ch,
                    control->rawTCstatus.value
                );
            updateStatus();
        }
    }
}

void userInit(void)
{
    Uns32 i;
    char threadName[32];

    if (diagnosticLevel >= 3)
        bhmMessage("I", "PP_STUBS","User initialization");

    control = (controlRegs *)&DMACSP_ab8_data;
    ch[0] = (channelRegs *)&DMACSP_ab32ch0_data;
    ch[1] = (channelRegs *)((void *)&DMACSP_ab32ch1_data);

    DMAState.readHandle = handles.MREAD;
    DMAState.writeHandle = handles.MWRITE;

    DMAState.intTCHandle = handles.INTTC;
    DMAState.intTCAsserted = False;

    // Create threads for the channels
    for (i=0; i<NUM_CHANNELS; i++) {

        // Event to start the thread
        DMAState.ch[i].start = bhmCreateEvent();
        DMAState.ch[i].busy = False;

        sprintf(threadName, "ch%u", i);
        DMAState.ch[i].thread = bhmCreateThread(
            channelThread,
            (void*) i,
            threadName,
            &DMAState.ch[i].stack[THREAD_STACK] // top of downward growing stack
        );
    }

    if(readNativeDataChannel != -1) {
        if(initSemiHost( (Uns32)ch[readNativeDataChannel], diagnosticLevel & 0xc)) {
            if (diagnosticLevel >= 1)
                bhmMessage("I", "INIT","Semihost Initialised");
        }
    }
}


PPM_CONSTRUCTOR_CB(constructor) {
    periphConstructor();
    bhmStringAttribute("title", title, sizeof(title));

    if (bhmUns32Attribute("enableNative", &readNativeDataChannel)) {
        if (diagnosticLevel >= 1)
            bhmMessage("I", "INIT","Use Native data on channel %d", readNativeDataChannel);
    }

    userInit();
}

PPM_DESTRUCTOR_CB(destructor) {
}

PPM_SAVE_STATE_FN(peripheralSaveState) {
}

PPM_RESTORE_STATE_FN(peripheralRestoreState) {
}
