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
