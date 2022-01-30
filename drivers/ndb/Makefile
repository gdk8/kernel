WFLAGS := -Wall -Wstrict-prototypes -Wno-trigraphs

LDFLAGS = -Map /var/tmp/ndbmap.txt

EXTRA_CFLAGS := $(WFLAGS)

# EXTRA_CFLAGS += -g -Wa,-adhln=$(<:.c=.lst)

EXTRA_CFLAGS += -D_DEBUG -g3  -fno-stack-protector

MODULE = ndb
KDIR = /lib/modules/`uname -r`/build
M := make -C ${KDIR} M=`pwd`

obj-m := $(MODULE).o 

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

install:
	/bin/bash ./install.sh

modules_install:
	${M} modules_install

$(MODULE)-objs := main.o kgb.o mailbox.o

clean:
	rm -rf *.o *~ .*.cmd *.ko *.mod.c *.order *.symvers .tmp_versions built-in.o
