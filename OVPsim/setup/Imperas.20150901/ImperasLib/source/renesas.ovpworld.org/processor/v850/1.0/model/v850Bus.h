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

#ifndef V850_BUS_H
#define V850_BUS_H

// Model header files
#include "v850TypeRefs.h"

//
// Allocate bus port specifications
//
void v850NewBusPorts(v850P v850);

//
// Free bus port specifications
//
void v850FreeBusPorts(v850P v850);

#endif /* V850_BUS_H */