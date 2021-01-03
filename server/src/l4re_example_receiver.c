#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <l4/re/c/dataspace.h>
#include <l4/re/c/util/cap_alloc.h>
#include <l4/re/c/rm.h>
#include <l4/re/env.h>

#include <l4/sys/rcv_endpoint.h>
#include <l4/sys/types.h>
#include <l4/sys/utcb.h>

#include "l4re_example_common.h"


int
main(int argc, char **argv)
{
  assert(argc == 2);

  printf("Hello from receiver\n");

  /* get IPC capability */
  l4_cap_idx_t chan =
    l4re_env_get_cap(argv[1]);

  /* bind capability to main thread */
  l4_rcv_ep_bind_thread(
    chan,
    l4re_env()->main_thread,
    IPC_THREAD_LABEL);

  /* receive IPC */
  l4re_ds_t ds = l4re_util_cap_alloc();

  l4_buf_regs_t *br = l4_utcb_br();
  br->bdr = 0;
  br->br[0] = ds
            | L4_RCV_ITEM_SINGLE_CAP
            | L4_RCV_ITEM_LOCAL_ID;

  l4_msgtag_t tag;
  l4_umword_t label;
  l4_msgtag_t tag = l4_ipc_wait(
    l4_utcb(),
    &label,
    L4_IPC_NEVER);

  if (label != IPC_THREAD_LABEL)
    printf("Incorrect thread label\n");

  if (l4_msgtag_label(tag) != IPC_LABEL)
    printf("Incorrect tag label\n");

  /* dump content of received dataspace */
  l4_addr_t *addr = 0;
  l4re_rm_attach((void **)&addr,
                 l4re_ds_size(ds),
                 L4RE_RM_SEARCH_ADDR,
                 ds,
                 0,
                 L4_PAGESHIFT);

  printf("Received: %s\n", (char *)addr);

  printf("Goodbye from receiver\n");

  exit(EXIT_SUCCESS);
}
