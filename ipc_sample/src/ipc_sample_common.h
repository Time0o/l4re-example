/*
 *  Copyright 2019 Timo Nicolai
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License 2.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <stdio.h>
#include <stdlib.h>

#include <l4/sys/types.h>

#ifndef COMMON_H
#define COMMON_H

#define IPC_CAP "cap"

enum { IPC_LABEL = 1, IPC_THREAD_LABEL = 2 };

void ipc_chkcap(l4_cap_idx_t cap, char const *msg);
void ipc_chksys(l4_msgtag_t tag, char const *msg);
char const *ipc_strerror(int err);

#endif // COMMON_H
