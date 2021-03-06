/*
*
*
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
//#include <asm/uaccess.h> from kernel to hardware (copy from user)
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/device.h>

#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>

#define MYMAJOR 200
#define MYNAME "testchar"


//用于存储主设备号
int mymajor;

//用于自动创建设备节点
static struct class *led_class;
static struct class_device *led_class_dev;

//GPIO寄存器物理地址
#define GPIO5_DR_PA 	0x020aC000
#define GPIO5_mux_PA  	0x020e01c4

//GPIO寄存器虚拟地址指针
volatile unsigned long *GPIO5_DR;
volatile unsigned long *GPIO5_GDIR;
volatile unsigned long *GPIO5_mux;


static int test_chrdev_open(struct inode *inode, struct file *file)
{
  printk(KERN_EMERG "test_chrdev_open\n");
  return 0;
}


static int test_chrdev_release(struct inode *inode, struct file *file)
{
  printk(KERN_EMERG "test_chrdev_release\n");
  return 0;
}

ssize_t test_chrdev_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
  printk(KERN_EMERG "test_chrdev_read\n");
  return 0;
}

static ssize_t test_chrdev_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
  int val;
  copy_from_user(&val,ubuf,count);
  if(val==1)/*light on*/
  {
    *GPIO5_GDIR |= (1<<15);   //gpio15
    *GPIO5_DR  |=  (1<<15);
  }
  else/*light off*/
  {
    *GPIO5_GDIR &= ~(1<<15);   //gpio15
    *GPIO5_DR  &=  ~(1<<15);
  }
  printk(KERN_EMERG "test_chrdev_write\n");
  return 0;
}

static const struct file_operations test_fops = 
 {
   .owner = THIS_MODULE,
   .open = test_chrdev_open,
   .release = test_chrdev_release,
   .write = test_chrdev_write,
   .read = test_chrdev_read,
 };

static int __init chrdev_init(void)
{
  //int ret = -1;
  //printk(KERN_EMERG "chrdev_init helloworld init\n");
  mymajor = register_chrdev(0,"led_driver", &test_fops);
  if(mymajor < 0)
  {
     printk(KERN_EMERG "register_chrdev fail\n");
     return -EINVAL;
  }
  printk(KERN_EMERG "register_chrdev success... mymajor = %d.\n",mymajor);

  led_class = class_create(THIS_MODULE,"led_driver");
  led_class_dev = device_create(led_class,NULL,MKDEV(mymajor,0),NULL,"led");

  GPIO5_mux = (volatile unsigned long *)ioremap(GPIO5_mux_PA, 4);
  GPIO5_DR = (volatile unsigned long *)ioremap(GPIO5_DR_PA, 8);
  GPIO5_GDIR = GPIO5_DR + 1; 
  *GPIO5_mux |= ((1) | (0<<1) | (1<<2)); // mux gpio5_15 ALT5   -> 101
  //*GPIO5_GDIR |= (1<<15);   //gpio15
  //*GPIO5_DR  |=  (1<<15);
  
  return 0;
}

static void __exit chrdev_exit(void)
{
  
  *GPIO5_DR  &=  (0<<15);
  *GPIO5_GDIR &= (0<<15);

  unregister_chrdev(mymajor,"led_driver");
  iounmap(GPIO5_DR);
  iounmap(GPIO5_mux);
  device_unregister(led_class_dev);
  class_destroy(led_class);
  printk(KERN_EMERG "chrdev_exit helloworld exit\n");  
}



module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("GPL");
