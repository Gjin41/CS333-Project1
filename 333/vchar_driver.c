#include <linux/module.h> //Includes module functions
#include <linux/fs.h> //define functions to reg/unreg the device number
#include <linux/device.h> //contains functions to create device file
#include <linux/slab.h> //to use functions kmalloc and kfree
#include <linux/cdev.h> //contains funtions to work with cdev
#include <linux/uaccess.h>//contains functions to help user communicates with kernel
#include <linux/random.h>//includes function to generate random number

#define DRIVER_AUTHOR "Nguyen Truong Giang <ntgiang@apcs.vn>"
#define DRIVER_DESC   "CS333 project"
#define DRIVER_VERSION "0.1"

typedef struct vchar_dev{
	unsigned char *control_regs;
	unsigned char *status_regs;
	unsigned char *data_regs;
} vchar_dev_t;

struct _vchar_drv{
	dev_t dev_num;
	struct class *dev_class;
	struct device *dev;
	vchar_dev_t *vchar_hw;
	struct cdev *vcdev;
	unsigned int open_cnt;
} vchar_drv;

/****************************** device specific - START *****************************/

//Function to release the device
void vchar_hw_exit(vchar_dev_t *hw){
	kfree(hw->control_regs);
}


/******************************* device specific - END *****************************/

/******************************** OS specific - START *******************************/
//Entry point functions
static int vchar_driver_open(struct inode *inode, struct file *filp){
	vchar_drv.open_cnt++;
	printk("Handle opened event (%d)\n", vchar_drv.open_cnt);
	return 0;
}

static int vchar_driver_release(struct inode *inode, struct file *filp){
	printk("Handle closed event\n");
	return 0;
}


static ssize_t vchar_driver_read(struct file *filp, char __user *user_buf, size_t len, loff_t *off){
	//Generate a random number
	int rand;
	printk("Generating a random number\n");
	get_random_bytes(&rand, 4);
	if(*off > 0)//EOF
		return 0;
	copy_to_user(user_buf, &rand, 4);
	*off += 4;
	return 4;
}

static ssize_t vchar_driver_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off){
	printk("Writing.....\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = vchar_driver_open,
	.release = vchar_driver_release,
	.read = vchar_driver_read,
	.write = vchar_driver_write,
};

//Init driver funtion
static int __init vchar_driver_init(void)
{
	int ret = 0;

	//Get device number
	vchar_drv.dev_num = 0;
	ret = alloc_chrdev_region(&vchar_drv.dev_num, 0, 1, "vchar_device");
	if (ret < 0){
		printk("Failed to register device number");
		goto failed_register_devnum;
	}
	printk("Allocated device number (%d,%d)\n", MAJOR(vchar_drv.dev_num), MINOR(vchar_drv.dev_num));

	//Create device file
	vchar_drv.dev_class = class_create(THIS_MODULE, "class_vchar_dev");
	if(vchar_drv.dev_class == NULL){
		printk("Failed to create a device class\n");
	goto failed_create_class;
	}
	vchar_drv.dev = device_create(vchar_drv.dev_class, NULL, vchar_drv.dev_num, NULL, "vchar_dev");
	if(IS_ERR(vchar_drv.dev)){
		printk("Failed to create a device\n");
	goto failed_create_device;
	}

	//Register entry points with kernel
	vchar_drv.vcdev = cdev_alloc();
	if(vchar_drv.vcdev == NULL){
		printk("Failed to allocate cdev structure\n");
		goto failed_alloc_cdev;
	}
	cdev_init(vchar_drv.vcdev, &fops);
	ret = cdev_add(vchar_drv.vcdev, vchar_drv.dev_num, 1);
	if(ret<0){
		printk("Failed to add a character device to the system\n");
		goto failed_alloc_cdev;
	}			

	printk("Initialize vchar driver successfully");
	return 0;

failed_alloc_cdev:
	vchar_hw_exit(vchar_drv.vchar_hw);
failed_create_device:
	class_destroy(vchar_drv.dev_class);
failed_create_class:
	unregister_chrdev_region(vchar_drv.dev_num, 1);
failed_register_devnum:
	return ret;
}

//End driver function
static void __exit vchar_driver_exit(void)
{

	//Unregister entry point with kernel
	cdev_del(vchar_drv.vcdev);

	//Release device
	vchar_hw_exit(vchar_drv.vchar_hw);

	//Delete device file
	device_destroy(vchar_drv.dev_class, vchar_drv.dev_num);
	class_destroy(vchar_drv.dev_class);

	//Release device number
	unregister_chrdev_region(vchar_drv.dev_num, 1);
	printk("Exit vchar driver\n");
}
/********************************* OS specific - END ********************************/

module_init(vchar_driver_init);
module_exit(vchar_driver_exit);

MODULE_LICENSE("GPL"); //License of module
MODULE_AUTHOR(DRIVER_AUTHOR); //Module's author
MODULE_DESCRIPTION(DRIVER_DESC); //Module description
MODULE_VERSION(DRIVER_VERSION); //Module version
MODULE_SUPPORTED_DEVICE("testdevice"); //Module supported device
