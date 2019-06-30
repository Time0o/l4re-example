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
#include <l4/sys/err.h>
#include <l4/sys/ipc.h>
#include <l4/sys/types.h>
#include <l4/sys/utcb.h>

#include "ipc_sample_common.h"


int
main(void)
{
  printf("Hello from sender\n");

  /* get capability */
  l4_cap_idx_t server = l4re_env_get_cap(IPC_CAP);
  ipc_chkcap(server, "getting capability");

  /* send IPC */
  l4_msg_regs_t *mr = l4_utcb_mr();
  for (unsigned i = 0; i < L4_UTCB_GENERIC_DATA_SIZE; ++i)
    mr->mr[i] = i;

  l4_msgtag_t send_tag = l4_msgtag(IPC_LABEL, L4_UTCB_GENERIC_DATA_SIZE, 0, 0);
  ipc_chksys(l4_ipc_send(server, l4_utcb(), send_tag, L4_IPC_NEVER),
             "sending IPC\n");

  printf("Goodbye from sender\n");

  exit(EXIT_SUCCESS);
}
