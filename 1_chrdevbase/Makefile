#内核源码路径
KERNEDIR := /home/yxz/linuxwork/IMX6ULL/linux-imx-rel_imx_4.1.15_2.1.0_ga_alientek
CURRENT_PATH := $(shell pwd)  #当前路径
obj-m := chrdevbase.o  #将chrdevbase.c编译为chrdevbase.ko
build: kernel_modules
kernel_modules:
	$(MAKE) -C $(KERNEDIR) M=$(CURRENT_PATH) modules
clean:
	$(MAKE) -C $(KERNEDIR) M=$(CURRENT_PATH) clean
