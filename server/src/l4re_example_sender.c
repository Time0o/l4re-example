#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <l4/re/c/dataspace.h>
#include <l4/re/c/mem_alloc.h>
#include <l4/re/c/util/cap_alloc.h>
#include <l4/re/c/rm.h>
#include <l4/re/env.h>

#include <l4/sys/err.h>
#include <l4/sys/ipc.h>
#include <l4/sys/types.h>
#include <l4/sys/utcb.h>

#include <l4/util/util.h>

#include "l4re_example_common.h"

/* text to be sent to receiving server */
static char str[] = "Hello world";

int
main(int argc, char **argv)
{
  assert(argc == 2);

  printf("Hello from sender\n");

  /* get IPC capability */
  l4_cap_idx_t chan =
    l4re_env_get_cap(argv[1]);

  /* create dataspace */
  l4re_ds_t ds = l4re_util_cap_alloc();

  l4re_ma_alloc(sizeof(str), ds, 0);

  /* copy text into dataspace */
  l4_addr_t *addr = 0;
  l4re_rm_attach((void **)&addr,
                 sizeof(str),
                 L4RE_RM_SEARCH_ADDR,
                 ds,
                 0,
                 L4_PAGESHIFT);

  memcpy(addr, str, sizeof(str));

  /* send IPC message */
  l4_fpage_t fpage =
    l4_obj_fpage(ds, 0, L4_FPAGE_RWX).raw;

  l4_msg_regs_t *mr = l4_utcb_mr();
  mr->mr[0] = l4_map_obj_control(0, 0);
  mr->mr[1] = fpage;

  l4_msgtag_t send_tag =
    l4_msgtag(IPC_LABEL, 0, 1, 0);

  l4_ipc_send(chan,
              l4_utcb(),
              send_tag,
              L4_IPC_NEVER),

  l4_sleep(100);

  /* free allocated ressources */

  /* ... */

  printf("Goodbye from sender\n");

  exit(EXIT_SUCCESS);
}
