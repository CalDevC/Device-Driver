/**************************************************************
* Class:  CSC-415-02 Spring 2022
* Name: Chase Alexander
* Student ID: 921040156
* GitHub UserID: CalDevC
* Project: Assignment 6 - Device Driver
*
* File: alexander_chase_HW6_main.c
*
* Description: A simple sample application to test my device driver
*
**************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {
  int fd, info;
  long n1, n2, n3;
  char message[] = { "Sample Message" };
  char read_buf[512];

  fd = open("/dev/TextModder", O_RDWR);
  printf("returned from open file, %d\n", fd);
  if (fd < 0) {
    printf("Device Open Error\n");
    perror("Device File Open Error");
    return -1;
  } else {
    printf("Device open success.\n");
  }

  //Do Work

  close(fd);
}