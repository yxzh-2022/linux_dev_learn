#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>

#define CHRDEVBASE_MAJOR   200               /* 主设备号    */ 
#define CHRDEVBASE_NAME     "chrdevbase"      /* 设备名       */ 
 
static char readbuf[100];                /* 读缓冲区    */ 
static char writebuf[100];               /* 写缓冲区    */ 
static char kerneldata[] = {"kernel data!"}; 

/* 
* @description   : 打开设备 
* @param – inode : 传递给驱动的 inode 
* @param - filp  : 设备文件，file 结构体有个叫做 private_data 的成员变量 
*                    一般在 open 的时候将 private_data 指向设备结构体。 
* @return         : 0 成功;其他 失败 
*/ 

static int chrdevbase_open(struct inode *inode, struct file *filp)
{
    /*用户实现具体功能*/
    return 0;

}
/*向设备写数据*/
/* 
* @description   : 向设备写数据  
* @param - filp  : 设备文件，表示打开的文件描述符 
* @param - buf   : 要写给设备写入的数据 
* @param - cnt   : 要写入的数据长度 
* @param - offt  : 相对于文件首地址的偏移 
* @return         : 写入的字节数，如果为负值，表示写入失败 
*/ 
static ssize_t chrdevbase_write(struct  file *filp, const char __user *buf,
                                size_t cnt,loff_t *offt)
{
    /* 用户实现具体功能 */
    int retvalue = 0;
    /*接手用户空间传递给内核的数据并打印出来*/
    retvalue = copy_from_user(writebuf,buf,cnt);
    if(retvalue == 0)
    {
        printk("kernal recevdata:%s\r\n",writebuf);
    }else{
        printk("kernal recevdata failed!\r\n");   /*内核打印*/
    }
    return 0;
}

/* 
* @description   : 从设备读取数据  
* @param - filp  : 要打开的设备文件(文件描述符) 
* @param - buf   : 返回给用户空间的数据缓冲区 
* @param - cnt   : 要读取的数据长度 
* @param - offt  : 相对于文件首地址的偏移 
* @return         : 读取的字节数，如果为负值，表示读取失败 
*/ 
static ssize_t chrdevbase_read(struct file *filp,char __user *buf,
                               size_t cnt ,loff_t *offt)
{
    int retvalue = 0;
    /* 向用户空间发送数据 */
    memcpy(readbuf,kerneldata,sizeof(kerneldata));
    retvalue = copy_to_user(buf, readbuf,cnt);
    if(retvalue == 0){
        printk("kerneal senddata ok!\r\n");
    }else{
        printk("kernal senddata failed!\r\n");
    }

    return 0;

}
/* 
* @description   : 关闭/释放设备 
* @param - filp  : 要关闭的设备文件(文件描述符) 
* @return         : 0 成功;其他 失败 
*/ 
static int chrdevbase_release(struct inode *inode,  
                                struct file *filp) 
{ 
     printk("chrdevbase release！\r\n"); 
     return 0; 
} 
/*
*设备操作结构体
*/
static struct file_operations chrdevbase_fops = {
    .owner = THIS_MODULE,
    .open = chrdevbase_open,
    .read = chrdevbase_read,
    .write = chrdevbase_write,
    .release = chrdevbase_release,

};
 /* 
* @description   : 驱动入口函数  
* @param         : 无 
* @return        : 0 成功;其他 失败 
*/ 
static int __init chrdevbase_init(void)
{
    int retvalue = 0;

    /*注册字符设备驱动*/
    retvalue = register_chrdev(CHRDEVBASE_MAJOR,CHRDEVBASE_NAME,
                                    &chrdevbase_fops);
    if (retvalue < 0)
    {
        printk("chrdevbase drtiver register failed!\r\n");
    }
    printk("chrdevbase_init()\r\n");
    return 0;                                    
}
/* 
* @description   : 驱动出口函数 
* @param         : 无 
* @return        : 无 
*/ 


static void __exit chrdevbase_exit(void)
{
    /*注销字符设备驱动*/
    unregister_chrdev(CHRDEVBASE_MAJOR,
    CHRDEVBASE_MAJOR);
    printk("chrdevbase_exit()\r\n");
}

/*  
* 将上面两个函数指定为驱动的入口和出口函数  
*/ 
module_init(chrdevbase_init); //模块入口
module_exit(chrdevbase_exit); //模块出口
 
/*  
* LICENSE 和作者信息 
*/ 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("xzyao"); 