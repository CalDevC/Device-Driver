/**************************************************************
* Class:  CSC-415-02 Spring 2022
* Name: Chase Alexander
* Student ID: 921040156
* GitHub UserID: CalDevC
* Project: Assignment 6 - Device Driver
*
* File: alexander_chase_HW6_driver.c
*
* Description: This file defines and implements a character
* device driver that will modify the user's input text to be
* either all upper case, all lower case, inverse case or reversed.
*
**************************************************************/

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#define MY_MAJOR 415
#define MY_MINOR 0
#define DEVICE_NAME "TextModder"

int major, minor;
char* kernel_buffer;

struct cdev my_cdev;
int actual_rx_size = 0;

MODULE_AUTHOR("Chase Alexander");
MODULE_DESCRIPTION(
  "A simple program to modify input text to be either all upper case, all lower case, inverse case or reversed"
);
MODULE_LICENSE("GPL");

//Data structure to keep track of how many times data is written
struct driverDS {
  int bufferPos;  //Buffer position
  char buffer[512];
  int operation;
} driverDS;

static int device_open(struct inode* inode, struct file* fs) {
  //Allocate memory for driver data and save it to private_data
  struct driverDS* ds = vmalloc(sizeof(struct driverDS));

  if (ds == 0) {
    printk(KERN_ERR "vmalloc could not be completed: file not opened\n");
  }

  ds->bufferPos = 0;
  fs->private_data = ds;
  return 0;
}

static int device_release(struct inode* inode, struct file* fs) {
  //Release allocated memory for driver data
  struct driverDS* ds = (struct driverDS*)fs->private_data;
  vfree(ds);
  return 0;
}

static ssize_t device_read(struct file* fs, char __user* buf, size_t numBytes,
  loff_t* offset) {
  struct driverDS* ds = (struct driverDS*)fs->private_data;
  int i, len;
  char temp;

  switch (ds->operation) {
    case 3:  //Set to all upper case
      printk(KERN_INFO "Setting to all upper case\n");
      for (i = 0; i < ds->bufferPos; i++) {
        if (ds->buffer[i] >= 97 && ds->buffer[i] <= 122) {
          ds->buffer[i] = ds->buffer[i] - 32;
        }
      }
      break;
    case 4:  //Set to all lower case
      printk(KERN_INFO "Setting to all lower case\n");
      for (i = 0; i < ds->bufferPos; i++) {
        if (ds->buffer[i] >= 65 && ds->buffer[i] <= 90) {
          ds->buffer[i] = ds->buffer[i] + 32;
        }
      }
      break;
    case 5:  //Invert the case of each letter
      printk(KERN_INFO "Inverting letter case\n");
      for (i = 0; i < ds->bufferPos; i++) {
        if (ds->buffer[i] >= 65 && ds->buffer[i] <= 90) {
          ds->buffer[i] = ds->buffer[i] + 32;
        } else if (ds->buffer[i] >= 97 && ds->buffer[i] <= 122) {
          ds->buffer[i] = ds->buffer[i] - 32;
        }
      }
      break;
    case 6:  //Reverse the text
      printk(KERN_INFO "Reversing text %d\n", ds->bufferPos);
      len = ds->bufferPos - 2;
      for (i = 0; i < ds->bufferPos / 2; i++) {
        printk(KERN_INFO "POSITION %d: %c\n", i, ds->buffer[i]);
        temp = ds->buffer[i];
        ds->buffer[i] = ds->buffer[len];
        ds->buffer[len--] = temp;
        printk(KERN_INFO "POSITION %d AFTER: %c\n", i, ds->buffer[i]);
      }
      break;
    default:
      copy_to_user(buf, ds->buffer, numBytes);
      return -1;
  }

  copy_to_user(buf, ds->buffer, numBytes);
  return ds->bufferPos;
}

static ssize_t device_write(struct file* fs, const char __user* buf,
  size_t numBytes, loff_t* offset) {
  struct driverDS* ds = (struct driverDS*)fs->private_data;

  //Copy data to buffer
  copy_from_user(ds->buffer, buf, numBytes);
  ds->bufferPos = numBytes;
  return numBytes;
}

static long device_ioctl(struct file* fs, unsigned int command,
  unsigned long data) {

  struct driverDS* ds = (struct driverDS*)fs->private_data;

  if (command >= 3 && command <= 6) {
    ds->operation = (int)command;
  } else {
    printk(KERN_ERR "Failed in ioctl\n");  //Error
    return -1;
  }

  return 0;
}

struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = device_open,
  .release = device_release,
  .read = device_read,
  .write = device_write,
  .unlocked_ioctl = device_ioctl
};

int init_module(void) {
  int result, registers;
  dev_t devno;

  devno = MKDEV(MY_MAJOR, MY_MINOR);

  //register device driver
  registers = register_chrdev_region(devno, 1, DEVICE_NAME);
  printk(KERN_INFO "Register chrdev succeeded 1: %d\n", registers);
  cdev_init(&my_cdev, &fops);
  my_cdev.owner = THIS_MODULE;

  result = cdev_add(&my_cdev, devno, 1);

  printk(KERN_INFO "Dev Added chrdev succeeded 2: %d\n", result);
  printk(KERN_INFO "Device driver loaded\n");

  if (result < 0) {
    printk(KERN_ERR "Register chrdev failed: %d\n", result);
  }

  return result;
}

void cleanup_module(void) {
  dev_t devno;
  devno = MKDEV(MY_MAJOR, MY_MINOR);
  unregister_chrdev_region(devno, 1);
  cdev_del(&my_cdev);
}