/*
 * --dice_driver by Buck Young, adapted from:
 *
 * "Hello, world!" minimal kernel module - /dev version
 * by Valerie Henson <val@nmt.edu>
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <asm/uaccess.h>

/*
 * dice_read is the function called when a process calls read() on
 * /dev/dice.  It writes a random die number to the buffer
 *
 */

static ssize_t dice_read(struct file * file, char * buf, 
			  size_t count, loff_t *ppos) 
{

	if(count<1){
		return -EINVAL;
	}
	
	if(*ppos != 0){
		return 0;
	}
	
	unsigned char data;
	get_random_bytes(&data, sizeof(char)); //get a byte of info
  

	data = (data %6) + 1; //scales the random data to 1-6

	if (copy_to_user(buf, &data, sizeof(ushort)/2)){
		return -EINVAL;
	}

	*ppos = 1;

	return 1;

}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations dice_fops = {
	.owner		= THIS_MODULE,
	.read		= dice_read,
};

static struct miscdevice dice_driver = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/dice.
	 */
	"dice",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&dice_fops
};

static int __init
dice_init(void)
{
	int ret;

	/*
	 * Create the "dice" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/dice device using
	 * the default rules.
	 */
	ret = misc_register(&dice_driver);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"dice_driver\" misc device\n");

	return ret;
}

module_init(dice_init);

static void __exit
dice_exit(void)
{
	misc_deregister(&dice_driver);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Buck Young bcy3@pitt.edu");
MODULE_DESCRIPTION("\"dice_driver\" minimal module");
MODULE_VERSION("dev");
