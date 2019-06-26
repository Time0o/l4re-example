#include <stdio.h>
#include <stdlib.h>

#include <l4/sys/types.h>

#ifndef COMMON_H
#define COMMON_H

enum { IPC_LABEL = 42 };

char const *ipc_strerror(int err);

l4_cap_idx_t get_chan_cap(int argc, char **argv);

#endif // COMMON_H
