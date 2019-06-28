#include <stdio.h>
#include <stdlib.h>

#include <l4/re/env.h>
#include <l4/sys/rcv_endpoint.h>
#include <l4/sys/utcb.h>
#include <l4/sys/types.h>

#include "ipc_sample_common.h"


int main(int argc, char **argv)
{
  l4_cap_idx_t chan_cap = get_chan_cap(argc, argv);

  printf("Hello from receiver\n");

  l4re_env_t *env = l4re_env();
  if (l4_msgtag_has_error(l4_rcv_ep_bind_thread(chan_cap, env->main_thread, 0)))
    fprintf(stderr, "failed to bind capability to main thread\n");

  /* receive IPC */
  l4_msgtag_t tag = l4_ipc_receive(chan_cap, l4_utcb(), L4_IPC_NEVER);
  if (l4_msgtag_has_error(tag))
    {
      fprintf(stderr, "failed to receive IPC: %s\n",
              ipc_strerror(l4_ipc_error_code(l4_utcb())));
    }
  else
    {
      printf("successfully received IPC\n");

      if (l4_msgtag_label(tag) != IPC_LABEL)
        fprintf(stderr, "IPC message has incorrectly labelled tag\n");

      if (l4_msgtag_words(tag) != L4_UTCB_GENERIC_DATA_SIZE)
        {
          fprintf(stderr, "IPC message does not include full MR data\n");
        }
      else
        {
          l4_msg_regs_t *mr = l4_utcb_mr();
          for (unsigned i = 0; i < l4_msgtag_words(tag); ++i)
            {
              if (mr->mr[i] != i)
                {
                  fprintf(stderr, "IPC message includes incorrect MR data\n");
                  break;
                }
            }
        }
    }

  exit(EXIT_SUCCESS);
}
