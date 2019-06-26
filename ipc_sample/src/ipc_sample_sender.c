#include <stdio.h>
#include <stdlib.h>

#include <l4/re/env.h>
#include <l4/sys/types.h>

#include "ipc_sample_common.h"


int main(int argc, char **argv)
{
  l4_cap_idx_t chan_cap = get_chan_cap(argc, argv);

  printf("Hello from sender\n");

  /* send a trivial IPC with a labelled tag */
  l4_msgtag_t tag = l4_msgtag(IPC_LABEL, 0, 0, 0);
  l4_ipc_send(chan_cap, l4_utcb(), tag, L4_IPC_SEND_TIMEOUT_0);

  printf("sent trivial IPC\n");

  exit(EXIT_SUCCESS);
}
