#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char **argv)
{ 
  int fd;
  int led_val = 1;
  long led_frequency = 100000;

  if(argc == 2)
  {
    led_frequency = atol(argv[1]);
    printf("The frequency is : %d\n",led_frequency);
  } 
  else
  {
    printf("Usage:\n");
    printf("Please input led light frequency:(the unit is us)\n");
    printf("Example:\n");
    printf("./test 100000\n");
  }

  fd=open("/dev/led",O_RDWR);
  if(fd < 0)
  {
    printf("Cann't open /dev/led!\n");
  }
  while(1)
  {  
    led_val =~ led_val;
    write(fd,&led_val,4);
    usleep(led_frequency);
  }

return 0;
}
