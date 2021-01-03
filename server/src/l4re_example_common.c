#include <stdio.h>
#include <stdlib.h>

#include <l4/sys/err.h>
#include <l4/sys/types.h>
#include <l4/sys/utcb.h>

#include "l4re_example_common.h"

struct sys_errcode_str {
  long code;
  char *str;
};

struct ipc_errcode_str {
  l4_umword_t code;
  char *str;
};

#define make_errcode_str(x) { x, #x }

static struct sys_errcode_str sys_errcodes[] = {
  make_errcode_str(L4_EOK),
  make_errcode_str(L4_EPERM),
  make_errcode_str(L4_ENOENT),
  make_errcode_str(L4_EIO),
  make_errcode_str(L4_ENXIO),
  make_errcode_str(L4_E2BIG),
  make_errcode_str(L4_EAGAIN),
  make_errcode_str(L4_ENOMEM),
  make_errcode_str(L4_EACCESS),
  make_errcode_str(L4_EFAULT),
  make_errcode_str(L4_EBUSY),
  make_errcode_str(L4_EEXIST),
  make_errcode_str(L4_ENODEV),
  make_errcode_str(L4_EINVAL),
  make_errcode_str(L4_ENOSPC),
  make_errcode_str(L4_ERANGE),
  make_errcode_str(L4_ENAMETOOLONG),
  make_errcode_str(L4_ENOSYS),
  make_errcode_str(L4_EBADPROTO),
  make_errcode_str(L4_EADDRNOTAVAIL),
  make_errcode_str(L4_ERRNOMAX),
  make_errcode_str(L4_ENOREPLY),
  make_errcode_str(L4_EMSGTOOSHORT),
  make_errcode_str(L4_EMSGTOOLONG),
  make_errcode_str(L4_EMSGMISSARG),
  make_errcode_str(L4_EIPC_LO),
  make_errcode_str(L4_EIPC_HI)
};

static struct ipc_errcode_str ipc_errcodes[] = {
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

static void
err(char const *msg, char const *err)
{
  if (err)
    fprintf(stderr, "error: %s (%s)\n", msg, err);
  else
    fprintf(stderr, "error: %s\n", msg);

  exit(EXIT_FAILURE);
}

static char const *
sys_strerror(long code)
{
  for (size_t i = 0; i < sizeof(sys_errcodes) / sizeof(sys_errcodes[0]); ++i)
    {
      if (sys_errcodes[i].code == -code)
        return sys_errcodes[i].str;
    }

  return "unknown error";
}

static char const *
ipc_strerror(l4_umword_t code)
{
  for (size_t i = 0; i < sizeof(ipc_errcodes) / sizeof(ipc_errcodes[0]); ++i)
    {
      if (ipc_errcodes[i].code == code)
        return ipc_errcodes[i].str;
    }

  return "unknown error";
}

void
chksys(long ret, char const *msg)
{
  if (ret < 0)
    err(msg, sys_strerror(ret));
}

void
chkcap(l4_cap_idx_t cap, char const *msg)
{
  if (l4_is_invalid_cap(cap))
    err(msg, NULL);
}

void
chkipc(l4_msgtag_t tag, char const *msg)
{
  if (l4_msgtag_has_error(tag))
    err(msg, ipc_strerror(l4_ipc_error(tag, l4_utcb())));
}
