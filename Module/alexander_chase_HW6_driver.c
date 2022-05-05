/**************************************************************
* Class:  CSC-415-02 Spring 2022
* Name: Chase Alexander
* Student ID: 921040156
* GitHub UserID: CalDevC
* Project: Assignment 6 - Device Driver
*
* File: alexander_chase_HW6_driver.c
*
* Description:
**************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#define MY_MAJOR 415
#define MY_MINOR 0

static int device_open(struct inode* inode, struct file* fs) {
  return 0;
}

static int device_release(struct inode* inode, struct file* fs) {
  return 0;
}

static ssize_t device_read(struct file* fs, char __user* buf, size_t numBytes,
  loff_t* offset) {
  return 0;
}

static ssize_t device_write(struct file* fs, const char __user* buf,
  size_t numBytes, loff_t* offset) {
  return 0;
}

static long device_ioctl(struct file* fs, unsigned int command,
  unsigned long data) {
  return 0;
}

struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl
};

int init_module(void) {
  return 0;
}

void cleanup_module(void) {
  return;
}