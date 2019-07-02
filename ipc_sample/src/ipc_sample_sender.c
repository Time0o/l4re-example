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
#include <l4/re/c/mem_alloc.h>
#include <l4/re/c/rm.h>
#include <l4/re/c/util/cap_alloc.h>
#include <l4/re/env.h>
#include <l4/sys/err.h>
#include <l4/sys/ipc.h>
#include <l4/sys/types.h>
#include <l4/sys/utcb.h>

#include "ipc_sample_common.h"


enum { Search_addr = 0x20 };

int
main(void)
{
  printf("Hello from sender\n");

  /* get capability */
  l4_cap_idx_t server = l4re_env_get_cap(IPC_CAP);
  chkcap(server, "getting capability");

  /* create and fill dataspace */
  // TODO: undo
  l4re_ds_t ds = l4re_util_cap_alloc();

  char const *text = lorem_ipsum();

  // TODO: undo
  chksys(l4re_ma_alloc(strlen(text), ds, 0),
         "allocating dataspace");

  // TODO: undo
  char *start;
  chksys(l4re_rm_attach(
           (void **)&start, strlen(text), Search_addr, ds, 0, L4_PAGESHIFT),
         "attaching dataspace");

  memcpy(start, text, strlen(text));

  /* send IPC */
  l4_umword_t *mr = l4_utcb_mr()->mr;

  mr[0] = l4_map_obj_control(0, 0);
  mr[1] = l4_obj_fpage(ds, 0, L4_FPAGE_RWX).raw;

  l4_msgtag_t send_tag = l4_msgtag(IPC_LABEL, 0, 1, 0);
  chkipc(l4_ipc_call(server, l4_utcb(), send_tag, L4_IPC_NEVER),
         "sending IPC\n");

  printf("Goodbye from sender\n");

  exit(EXIT_SUCCESS);
}
