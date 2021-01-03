#include <l4/sys/types.h>

#ifndef COMMON_H
#define COMMON_H

enum { IPC_LABEL = 1, IPC_THREAD_LABEL = 2 };

void chksys(long ret, char const *msg);
void chkcap(l4_cap_idx_t cap, char const *msg);
void chkipc(l4_msgtag_t tag, char const *msg);

#endif // COMMON_H
