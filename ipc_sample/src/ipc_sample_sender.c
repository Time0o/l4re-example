#include <stdio.h>
#include <stdlib.h>

#include <l4/sys/utcb.h>
#include <l4/sys/types.h>

#include "ipc_sample_common.h"


int main(int argc, char **argv)
{
  l4_cap_idx_t chan_cap = get_chan_cap(argc, argv);

  printf("Hello from sender\n");

  /* send IPC */
  l4_msg_regs_t *mr = l4_utcb_mr();
  for (unsigned i = 0; i < L4_UTCB_GENERIC_DATA_SIZE; ++i)
    mr->mr[i] = i;

  l4_msgtag_t send_tag = l4_msgtag(IPC_LABEL, L4_UTCB_GENERIC_DATA_SIZE, 0, 0);

  l4_msgtag_t result_tag = l4_ipc_call(
    chan_cap, l4_utcb(), send_tag, L4_IPC_NEVER);

  if (l4_msgtag_has_error(result_tag))
    {
      fprintf(stderr, "failed to send IPC: %s\n",
              ipc_strerror(l4_ipc_error_code(l4_utcb())));
    }
  else
    {
      printf("sent IPC\n");
    }

  exit(EXIT_SUCCESS);
}
