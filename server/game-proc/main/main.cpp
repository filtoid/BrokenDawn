#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include "./bdgame.h"

/*
This is the main game loop - it is designed to run as daemon.
The basic skeleton code has been taken from this excellent tutorial:
http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
by Devin Watson
*/

int main(void) {
        
        /* Our process ID and Session ID */
        pid_t pid, sid;
	
        openlog ("brokendawn", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        syslog(LOG_INFO, "Broken Dawn Deamon - Starting\n");

        /* Fork off the parent process */
        pid = fork();
        if (pid < 0) {
		syslog(LOG_ERR, "Broken Dawn - Failed to fork, exiting\n");
		closelog();
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
		syslog(LOG_ERR,"Failed to create a new sys-id for the child process\n");
		closelog();
                exit(EXIT_FAILURE);
        }
        
        /* Change the current working directory */
        if ((chdir("/")) < 0) {
                /* Log the failure */
		syslog(LOG_ERR,"Broken Dawn Daemon - Failed to change dir to root\n");
		closelog();
                exit(EXIT_FAILURE);
        }
        
        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        
        /* Daemon-specific initialization goes here */
 	syslog(LOG_INFO,"Broken Dawn Daemon- Finished Initialization\n");       
        /* The Big Loop */
	bool bQuit = false; // This bool is part of a mammoth hack
			    // where we can enter a db setting to
			    // to the make the process exit - this
			    // is mostly to allow for automation of
			    // exiting rather than having to kill
			    // the process. 
	while (bQuit==false) {
		
           /* Do the game processing */
	     
          
        }
	
	syslog(LOG_INFO, "Broken Dawn Daemon - Exiting Correctly\n");	
	closelog();
	exit(EXIT_SUCCESS);
}
