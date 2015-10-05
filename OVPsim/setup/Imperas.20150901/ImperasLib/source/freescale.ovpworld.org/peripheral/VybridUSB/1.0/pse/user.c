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


#include "pse.igen.h"
//////////////////////////////// Callback stubs ////////////////////////////////

PPM_REG_WRITE_CB(PHYWrite1c) {
    // YOUR CODE HERE (PHYWrite1c)
    *(Uns32*)user = (*(Uns32*)user & 0x7ffffff8) | (data & 0x80000007);
}

PPM_REG_READ_CB(readCapVersion) {
    // YOUR CODE HERE (readCapVersion)
    return *(Uns32*)user;
}

PPM_REG_READ_CB(readDCIVersion) {
    // YOUR CODE HERE (readDCIVersion)
    return *(Uns32*)user;
}

PPM_REG_READ_CB(readShare154h) {
    // YOUR CODE HERE (readShare154h)
    return *(Uns32*)user;
}

PPM_REG_READ_CB(readShare158h) {
    // YOUR CODE HERE (readShare158h)
    return *(Uns32*)user;
}

PPM_REG_WRITE_CB(writeShare154h) {
    // YOUR CODE HERE (writeShare154h)
    *(Uns32*)user = (*(Uns32*)user & 0x0) | (data & 0xffffffff);
}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)
    periphConstructor();
    // YOUR CODE HERE (post constructor)
}



// Save / restore callbacks

PPM_SAVE_STATE_FN(peripheralSaveState) {
}

PPM_RESTORE_STATE_FN(peripheralRestoreState) {
}
