KERNEL_DIR :=/usr/src/linux-source-$(version)	

#if build for running kernel
KERNEL_DIR := /lib/modules/`uname -r`/build  
  
#depends on your platform  
ARCH=x86_64  
  
PWD := $(shell pwd)  
  
#for module specific flags, such -I/include/dir...  
EXTRA_CFLAGS +=  
EXTRA_LDFLAGS +=  
  
#  
#modules and objects definition  
#  
obj-m := page.o vmem_disk.o page_cache.o rmmodule.o
#page-y := uaccess.o init.o # xxx.o ...  
#for subdirectories  
#page-y += ./subdir/xxx.o  
#  
  
all:  
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) ARCH=$(ARCH) modules  
  
help:  
	@echo "My module description here"  
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) ARCH=$(ARCH) help  
  
#customize the install directory with INSTALL_MOD_PATH and INSTALL_MOD_DIR  
#or the default /lib/modules/$(KERNELRELEASE)/extra  
#install:  
#   $(MAKE) -C $(KERNEL_DIR) M=$(PWD) ARCH=$(ARCH) modules_install  
  
clean:  
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) ARCH=$(ARCH) clean  
