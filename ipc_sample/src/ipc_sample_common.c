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

#include <l4/sys/types.h>

#include "ipc_sample_common.h"


void
ipc_chkcap(l4_cap_idx_t cap, char const *msg)
{
  if (l4_is_invalid_cap(cap))
    {
      if (msg)
        fprintf(stderr, "error: %s\n", msg);

      exit(EXIT_FAILURE);
    }
}

void
ipc_chksys(l4_msgtag_t tag, char const *msg)
{
  if (l4_msgtag_has_error(tag))
    {
      if (msg)
        fprintf(stderr, "error: %s\n", msg);

      exit(EXIT_FAILURE);
    }
}

struct errcode_str {
  int code;
  char *str;
};

#define make_errcode_str(x) { x, #x }

static struct errcode_str errcodes[] = {
  make_errcode_str(L4_IPC_ERROR_MASK),
  make_errcode_str(L4_IPC_ERROR_MASK),
  make_errcode_str(L4_IPC_SND_ERR_MASK),
  make_errcode_str(L4_IPC_ENOT_EXISTENT),
  make_errcode_str(L4_IPC_RETIMEOUT),
  make_errcode_str(L4_IPC_SETIMEOUT),
  make_errcode_str(L4_IPC_RECANCELED),
  make_errcode_str(L4_IPC_SECANCELED),
  make_errcode_str(L4_IPC_REMAPFAILED),
  make_errcode_str(L4_IPC_SEMAPFAILED),
  make_errcode_str(L4_IPC_RESNDPFTO),
  make_errcode_str(L4_IPC_SESNDPFTO),
  make_errcode_str(L4_IPC_RERCVPFTO),
  make_errcode_str(L4_IPC_SERCVPFTO),
  make_errcode_str(L4_IPC_REABORTED),
  make_errcode_str(L4_IPC_SEABORTED),
  make_errcode_str(L4_IPC_REMSGCUT),
  make_errcode_str(L4_IPC_SEMSGCUT)
};

char const *
ipc_strerror(int code)
{
  for (size_t i = 0; i < sizeof(errcodes) / sizeof(errcodes[0]); ++i)
    {
      if (errcodes[i].code == code)
        return errcodes[i].str;
    }

  return "unknown error";
}
