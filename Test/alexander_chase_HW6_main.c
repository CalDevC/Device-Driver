/**************************************************************
* Class:  CSC-415-02 Spring 2022
* Name: Chase Alexander
* Student ID: 921040156
* GitHub UserID: CalDevC
* Project: Assignment 6 - Device Driver
*
* File: alexander_chase_HW6_main.c
*
* Description: A simple sample application to test each of my
* device driver commands.
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

  char message[] = { "Sample Message" };  //The data to be sent
  char read_buf[512];

  fd = open("/dev/textModder", O_RDWR);
  printf("returned from open file, %d\n", fd);
  if (fd < 0) {
    printf("Device Open Error\n");
    perror("Device File Open Error");
    return -1;
  } else {
    printf("Device open success.\n");
  }

  //Test calls to demonstrate the functionality of each of my 
  //4 ioctl commands

  printf("\nOriginal message: %s\n", message);

  //3 - Make the message all upper case
  write(fd, message, sizeof(message));
  ioctl(fd, 3, &info);
  read(fd, read_buf, sizeof(message));
  printf("Message after setting all upper case: %s\n", read_buf);

  //4 - Make the message all lower case
  write(fd, message, sizeof(message));
  ioctl(fd, 4, &info);
  read(fd, read_buf, sizeof(message));
  printf("Message after setting all lower case: %s\n", read_buf);

  //5 - Invert the case of each char in the message
  write(fd, message, sizeof(message));
  ioctl(fd, 5, &info);
  read(fd, read_buf, sizeof(message));
  printf("Message after inverting case: %s\n", read_buf);

  //6 - Reverse the message
  write(fd, message, sizeof(message));
  ioctl(fd, 6, &info);
  read(fd, read_buf, sizeof(message));
  printf("Message after reversal: %s\n", read_buf);

  close(fd);
}