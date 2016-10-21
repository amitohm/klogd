#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/klog.h>
#include <utils/Log.h>

#undef LOG_TAG
#define LOG_TAG "klogd"

#define KLOG_BUF_SHIFT	17	/* CONFIG_LOG_BUF_SHIFT from our kernel */
#define KLOG_BUF_LEN	(1 << KLOG_BUF_SHIFT)

int klogd_main(int argc, char **argv) {

    char buffer[KLOG_BUF_LEN + 1];
	char *p;
	int n, ret;
	
	/* Our process ID and Session ID */
	pid_t pid, sid;
	
	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
			exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then
	   we can exit the parent process. */
	if (pid > 0) {
			exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);
			
	/* Open any logs here */        
			
	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
			/* Log the failure */
			exit(EXIT_FAILURE);
	}
	
	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	/* Daemon-specific initialization goes here */
	p = buffer;
	n = 8;//Default log level
	if((argc == 3) && (!strcmp(argv[1],"-n"))) {
		n = atoi(argv[2]);
		if((n<0) && (n>8))
			n = 8;
	}
	klogctl(KLOG_SETLEVEL, NULL, n);
	
	/* The Big Loop */
	while (1) {
	   /* Do some task here ... */
		n = klogctl(KLOG_READ, buffer, KLOG_BUF_LEN);
		if (n < 0) {
			perror("klogctl");
			goto klog_exit;//exit daemon
		}
		buffer[n] = '\0';
		
		ALOGE("%s", buffer);
	}
	
klog_exit:
   exit(EXIT_SUCCESS);
}
