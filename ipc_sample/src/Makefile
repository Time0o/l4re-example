PKGDIR ?= ../..
L4DIR  ?= $(PKGDIR)/../..

TARGET                    = ipc_sample_sender ipc_sample_receiver
SRC_C_ipc_sample_sender   = ipc_sample_sender.c ipc_sample_common.c
SRC_C_ipc_sample_receiver = ipc_sample_receiver.c ipc_sample_common.c

REQUIRES_LIBS = l4re_c l4re_c-util

include $(L4DIR)/mk/prog.mk
