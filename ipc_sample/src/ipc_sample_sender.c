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


enum { OP_SUB = 0, OP_NEG = 1, PROTO_CALC = 0x42 };


static int
make_ipc_call(l4_cap_idx_t cap, unsigned words)
{
  l4_msgtag_t tag = l4_msgtag(PROTO_CALC, words, 0, 0);
  if (l4_msgtag_has_error(l4_ipc_call(cap, l4_utcb(), tag, L4_IPC_NEVER)))
    {
      fprintf(stderr, "failed to talk to server\n");
      return -L4_ENOSYS;
    }

  return L4_EOK;
}


static int
sub(l4_cap_idx_t cap, l4_mword_t val1, l4_mword_t val2, l4_mword_t *res)
{
  l4_umword_t *mr = l4_utcb_mr()->mr;

  mr[0] = OP_SUB;
  mr[1] = val1;
  mr[2] = val2;

  if (make_ipc_call(cap, 3) < 0)
    return -L4_ENOSYS;

  *res = (l4_mword_t)mr[0];
  return L4_EOK;
}


static int
neg(l4_cap_idx_t cap, l4_mword_t val1, l4_mword_t *res)
{
  l4_umword_t *mr = l4_utcb_mr()->mr;

  mr[0] = OP_NEG;
  mr[1] = val1;

  if (make_ipc_call(cap, 2) < 0)
    return -L4_ENOSYS;

  *res = (l4_mword_t)mr[0];
  return L4_EOK;
}


int
main(int argc, char **argv)
{
  l4_cap_idx_t server = get_chan_cap(argc, argv);

  printf("Hello from sender\n");

#if 0
  /* send IPC */
  l4_msg_regs_t *mr = l4_utcb_mr();
  for (unsigned i = 0; i < L4_UTCB_GENERIC_DATA_SIZE; ++i)
    mr->mr[i] = i;

  l4_msgtag_t send_tag = l4_msgtag(IPC_LABEL, L4_UTCB_GENERIC_DATA_SIZE, 0, 0);

  l4_msgtag_t result_tag = l4_ipc_call(
    server, l4_utcb(), send_tag, L4_IPC_NEVER);

  if (l4_msgtag_has_error(result_tag))
    {
      fprintf(stderr, "failed to send IPC: %s\n",
              ipc_strerror(l4_ipc_error_code(l4_utcb())));
    }
  else
    {
      printf("sent IPC\n");
    }
#endif

  /* make RPC */
  l4_mword_t res;

  // subtract two numbers
  if (sub(server, 8, 5, &res) < 0)
    exit(EXIT_FAILURE);

  printf("server returned: %ld\n", res);

  // negate number
  if (neg(server, 8, &res) < 0)
    exit(EXIT_FAILURE);

  printf("server returned: %ld\n", res);

  exit(EXIT_SUCCESS);
}
