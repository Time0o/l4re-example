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

#include <l4/re/env.h>
#include <l4/sys/rcv_endpoint.h>
#include <l4/sys/utcb.h>
#include <l4/sys/types.h>

#include "ipc_sample_common.h"


int main(void)
{
  printf("Hello from receiver\n");

  /* bind capability */
  l4_cap_idx_t server = l4re_env_get_cap(IPC_CAP);
  ipc_chkcap(server, "getting capability");

  l4re_env_t *env = l4re_env();
  ipc_chksys(l4_rcv_ep_bind_thread(server, env->main_thread, IPC_THREAD_LABEL),
             "binding capability to main thread");

  /* receive IPC */
  l4_umword_t label;
  l4_msgtag_t tag = l4_ipc_wait(l4_utcb(), &label, L4_IPC_NEVER);
  ipc_chksys(tag, "receiving IPC\n");

  if (label != IPC_THREAD_LABEL)
    fprintf(stderr, "IPC has incorrect thread label\n");

  if (l4_msgtag_label(tag) != IPC_LABEL)
    fprintf(stderr, "IPC has incorrect tag label\n");

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

  printf("Goodbye from receiver\n");

  exit(EXIT_SUCCESS);
}
