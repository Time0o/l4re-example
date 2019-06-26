#include <stdio.h>
#include <stdlib.h>

#include <l4/re/env.h>
#include <l4/sys/types.h>

#include "ipc_sample_common.h"


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

char const *ipc_strerror(int code)
{
  for (size_t i = 0; i < sizeof(errcodes) / sizeof(errcodes[0]); ++i)
    {
      if (errcodes[i].code == code)
        return errcodes[i].str;
    }

  return "unknown error";
}

l4_cap_idx_t get_chan_cap(int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "usage: %s CHAN_CAPNAME\n", argv[0]);
      exit(EXIT_FAILURE);
    }

  char *chan_capname = argv[1];

  l4_cap_idx_t chan_cap = l4re_env_get_cap(chan_capname);
  if (l4_is_invalid_cap(chan_cap))
    {
      fprintf(stderr, "failed to obtain '%s' capability\n", chan_capname);
      exit(EXIT_FAILURE);
    }

  printf("'%s' capability has read rights: %s\n",
         chan_capname,
         chan_cap & L4_CAP_FPAGE_R ? "yes" : "no");

  printf("'%s' capability has special rights: %s\n",
         chan_capname,
         chan_cap & L4_CAP_FPAGE_S ? "yes" : "no");

  return chan_cap;
}
