#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define LED_MAJOR  201
#define LED_NAME   "led"

#define ledoff    0
#define ledon     1
#define LedFlicker 2   

/*寄存器的物理地址*/
#define CCM_CCGR1_BASE                  (0X020C406C)
#define SW_MUX_GPIO1_IO03_BASE          (0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE          (0X020E02F4)
#define GPIO1_DR_BASE                   (0X0209C000)
#define GPIO1_GDIR_BASE                 (0X0209C004)


/*地址映射的虚拟地址*/
static void __iomem *CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;




static void led_on( void )
{
    u32 val = 0;
    val = readl(GPIO1_DR);
    val &= ~(1 << 3);  /*先清除以前的配置bit3*//*打开LED */
    writel(val, GPIO1_DR);
}

static void  led_off(void)
{
    u32 val = 0;
    val = readl(GPIO1_DR);
    val |= (1 << 3);  /*置1 bit3 关闭LED */
    writel(val, GPIO1_DR);
}

static void  led_flicker(void)
{
    u32 val = 0;
    for(;;)
    {
        val = readl(GPIO1_DR);
        val |= (1 << 3);  /*置1 bit3 关闭LED */
        writel(val, GPIO1_DR);
        //delayed_work();
        val &= ~(1 << 3);  /*置1 bit3 关闭LED */
        writel(val, GPIO1_DR);
        //延时
    }
}

static int led_open(struct inode *inode, struct file *filp)
{
    return 0;
}



static ssize_t led_read(struct file *filp , const char __user *buf ,
                    size_t count ,loff_t *ppos)
{
    return 0;
}

static ssize_t led_write(struct file *filp , const char __user *buf ,
                    size_t count ,loff_t *ppos)
{
    int ret = 0;
    unsigned char databuf[1];

    ret = copy_from_user(databuf,buf,count);
    if (ret < 0)
    {
        printk("kernel read data failed !\r\n");        /* code */
    }
    switch (databuf[0])
    {
        case ledon/* constant-expression */:
            /* code */
            led_on();
            break;
        case ledoff:
            led_off();
            break;
        case LedFlicker:
            led_flicker();
            break;
        default:
            break;
    }

    
    return 0;
}

static int led_release(struct inode *inode, struct file *filp)
{
    return 0;
}
/*字符设备操作集*/
static const struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,
    .release =  led_release,
};

/*入口函数*/
static int __init led_init(void)
{
    int ret = 0;
    unsigned int val = 0;
    /*初始化 led  地址映射*/
    CCM_CCGR1           = ioremap(CCM_CCGR1_BASE ,4);
    SW_MUX_GPIO1_IO03   = ioremap(SW_MUX_GPIO1_IO03_BASE, 4);
    SW_PAD_GPIO1_IO03   = ioremap(SW_PAD_GPIO1_IO03_BASE, 4);
    GPIO1_DR            = ioremap(GPIO1_DR_BASE, 4);
    GPIO1_GDIR          = ioremap(GPIO1_GDIR_BASE, 4);

    /*初始化时钟*/
    val = readl(CCM_CCGR1);
    val &= ~(3 <<26);  /*先清除以前的配置bit26、27*/
    val |= 3 << 26;     /*26 27 置1*/
    writel(val, CCM_CCGR1);
    writel(0x05,SW_MUX_GPIO1_IO03);     /*设置复用*/
    writel(0x10b0, SW_PAD_GPIO1_IO03);  /*设置电气属性*/

    val = readl(GPIO1_GDIR);
    val |= 1 << 3;                       /*bit3 置1 ,设置为输出*/
    writel(val, GPIO1_GDIR);

    val = readl(GPIO1_DR);
    val &= ~(1 << 3);  /*先清除以前的配置bit3*//*打开LED */
    writel(val, GPIO1_DR);

    /*注册字符设备*/
    ret = register_chrdev(LED_MAJOR, LED_NAME, &led_fops);
    if(ret < 0)
    {
        printk("led register failed!\r\n");
        return -EIO;
    }
    printk("led_init\r\n");
    return 0;
}

/*出口函数*/
static void __exit led_exit(void)
{
    unsigned int val = 0;
    /*关闭led*/
    val = readl(GPIO1_DR);
    val |= (1 << 3);  /*bit3置1 关闭LED */
    writel(val, GPIO1_DR);

    /*取消地址映射*/
    iounmap(CCM_CCGR1);
    iounmap(SW_MUX_GPIO1_IO03);
    iounmap(SW_PAD_GPIO1_IO03);
    iounmap(GPIO1_DR);
    iounmap(GPIO1_GDIR);

    /*注销字符设备驱动*/
    unregister_chrdev(LED_MAJOR,LED_NAME);
    printk("led_exit\r\n");
}
/*注册驱动加载与卸载*/
module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("yxzh");

