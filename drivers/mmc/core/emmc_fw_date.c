/*
 * Author: andip71, 15.01.2013
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#include "emmc_fw_date.h"


/*****************************************/
// Global variables
/*****************************************/


/*****************************************/
// sysfs interface functions
/*****************************************/

static ssize_t emmc_fw_date_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	// return value of EMMC firmware date
	return sprintf(buf, "EMMC firmware date: %08x \n", emmc_firmware_date);
}



/*****************************************/
// Initialize emmc firmware date sysfs
/*****************************************/

// define objects
static DEVICE_ATTR(emmc_fw_date, S_IRUGO | S_IWUGO, emmc_fw_date_show, NULL);

// define attributes
static struct attribute *emmc_fw_date_attributes[] = {
	&dev_attr_emmc_fw_date.attr,
	NULL
};

// define attribute group
static struct attribute_group emmc_fw_date_control_group = {
	.attrs = emmc_fw_date_attributes,
};

// define control device
static struct miscdevice emmc_fw_date_control_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "emmc_fw_date",
};


/*****************************************/
// Driver init and exit functions
/*****************************************/

static int emmc_fw_date_init(void)
{
	// register emmc firmware date diagnostic device
	misc_register(&emmc_fw_date_control_device);
	if (sysfs_create_group(&emmc_fw_date_control_device.this_device->kobj,
				&emmc_fw_date_control_group) < 0) {
		printk("Boeffla-kernel: failed to create emmc firmware date diagnostic sys fs object.\n");
		return 0;
	}

	// Print debug info
	printk("Boeffla-kernel: emmc firmware date diagnostic device initialized\n");

	return 0;
}


static void emmc_fw_date_exit(void)
{
	// remove emmc firmware date diagnostic device
	sysfs_remove_group(&emmc_fw_date_control_device.this_device->kobj,
                           &emmc_fw_date_control_group);

	// Print debug info
	printk("Boeffla-kernel: emmc firmware date diagnostic device stopped\n");
}


/* define driver entry points */

module_init(emmc_fw_date_init);
module_exit(emmc_fw_date_exit);

