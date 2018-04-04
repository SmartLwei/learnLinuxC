obj-m += its_netlink.o
#注：**.o必须与c文件同名，否则编译将会失败
#generate the path
CURRENT_PATH:=$(shell pwd)
#the current kernel version number
LINUX_KERNEL:=$(shell uname -r)
#the absolute path
LINUX_KERNEL_PATH:=/usr/src/linux-headers-$(LINUX_KERNEL)
#complie object
all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
#make –C 表示（change）改变当前makefile的路径，切换到其他makefile中去执行，后面跟的就是想要切换到的路径
#clean
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
