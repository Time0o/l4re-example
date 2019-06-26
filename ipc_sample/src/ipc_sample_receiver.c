#include <stdio.h>
#include <stdlib.h>

#include <l4/sys/types.h>

#include "ipc_sample_common.h"


int main(int argc, char **argv)
{
  l4_cap_idx_t chan_cap = get_chan_cap(argc, argv);

  printf("Hello from receiver\n");

  /* receive a trivial IPC with a labelled tag */
  l4_msgtag_t tag = l4_ipc_receive(chan_cap, l4_utcb(), L4_IPC_NEVER);
  if (l4_msgtag_has_error(tag))
    {
      fprintf(stderr, "failed to receive trivial IPC: %s\n",
              ipc_strerror(l4_ipc_error_code(l4_utcb())));
    }
  else
    {
      printf("successfully received trivial IPC\n");

      if (l4_msgtag_label(tag) != IPC_LABEL)
        fprintf(stderr, "trivial IPC message has incorrectly labelled tag\n");
    }

  exit(EXIT_SUCCESS);
}
