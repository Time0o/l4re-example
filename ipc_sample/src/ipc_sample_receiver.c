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
#include <string.h>

#include <l4/re/c/dataspace.h>
#include <l4/re/c/util/cap_alloc.h>
#include <l4/re/env.h>
#include <l4/sys/rcv_endpoint.h>
#include <l4/sys/utcb.h>
#include <l4/sys/types.h>

#include "ipc_sample_common.h"


int
main(void)
{
  printf("Hello from receiver\n");

  /* bind capability */
  l4_cap_idx_t server = l4re_env_get_cap(IPC_CAP);
  chkcap(server, "getting capability");

  l4re_env_t *env = l4re_env();
  chkipc(l4_rcv_ep_bind_thread(server, env->main_thread, IPC_THREAD_LABEL),
         "binding capability to main thread");

  /* receive IPC */
  l4_cap_idx_t rcv_cap = l4re_util_cap_alloc();

  l4_buf_regs_t *br = l4_utcb_br();
  br->bdr = 0;
  br->br[0] = rcv_cap | L4_RCV_ITEM_SINGLE_CAP | L4_RCV_ITEM_LOCAL_ID;

  l4_umword_t label;
  l4_msgtag_t tag = l4_ipc_wait(l4_utcb(), &label, L4_IPC_NEVER);
  chkipc(tag, "receiving IPC");

  if (label != IPC_THREAD_LABEL)
    fprintf(stderr, "IPC has incorrect thread label\n");

  if (l4_msgtag_label(tag) != IPC_LABEL)
    fprintf(stderr, "IPC has incorrect tag label\n");

  if (l4_msgtag_words(tag))
    {
      fprintf(stderr, "IPC message includes garbage MR data\n");
    }
  else
    {
      char const *text = lorem_ipsum();
      if (l4re_ds_size(rcv_cap) != strlen(text))
        fprintf(stderr, "received dataspace too small\n");

      // TODO: compare content
    }

  printf("Goodbye from receiver\n");

  exit(EXIT_SUCCESS);
}
