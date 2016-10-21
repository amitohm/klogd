#include <stdlib.h>
#include <stdio.h>

//#include <syslog.h>
#include <sys/klog.h>

#define	KMSG_BUF_SIZE	1024

int main(int argc, char *argv[])
{
    static char kmsg_buf[KMSG_BUF_SIZE];

    while(0<klogctl(2,kmsg_buf,KMSG_BUF_SIZE))
	printf("%s",kmsg_buf);
	
    return 0;
}

