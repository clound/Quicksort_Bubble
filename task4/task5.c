#include <stdio.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/time.h>

int main ()  
{  
	int keys_fd;  
  	struct input_event t;  

 	struct timeval tv;
  	struct timezone tz;
  
  	keys_fd = open ("/dev/input/event4",O_RDONLY);  
  	if (keys_fd <= 0)  
    {  
      	printf ("open /dev/input/event2 device error!\n");  
      	return 0;  
    }  
  
  	while (1)  
    {  
      	if (read (keys_fd, &t, sizeof (t)) == sizeof (t))  
        {  
          	if (t.type == EV_KEY) 
			{
            	if (t.value == 0 || t.value == 1)  
        		{  
              		printf ("\n key %d %s\n",t.code,(t.value) ? "Pressed" : "Released"); 
					gettimeofday(&tv, &tz);
					printf("tv_sec:%d\n",(int)tv.tv_sec);
					printf("tv_usec:%d\n",(int)tv.tv_usec);
          			if(t.code==KEY_ESC)  
              				break;  
        		}  
        	}  
    	}  
    }  
	close (keys_fd);  
  	return 0;  
}
