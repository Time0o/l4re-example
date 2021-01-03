PKGDIR ?= ../..
L4DIR  ?= $(PKGDIR)/../..

TARGET                      = l4re_example_sender l4re_example_receiver
SRC_C_l4re_example_sender   = l4re_example_sender.c l4re_example_common.c
SRC_C_l4re_example_receiver = l4re_example_receiver.c l4re_example_common.c

REQUIRES_LIBS = l4re_c l4re_c-util

include $(L4DIR)/mk/prog.mk
