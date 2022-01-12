#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
        int     fd[2], nbytes;
        pid_t   childpid;
        char    string[] = "Hello, world!\n";
        char *p;
        long nconv = strtol(argv[1], &p, 10);
        int n = nconv;
        long dconv = strtol(argv[2], &p, 10);
        int d = dconv;
 
        char    readbuffer[80];

        pipe(fd);
        
        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(childpid == 0)
        {
                /* Child process closes up input side of pipe */
            close(fd[0]);

                /* Send "string" through the output side of pipe */
            int next_produced;
            for (int i = 0; i < n; i++){
                next_produced = d*i;
                sprintf(string,"%d",next_produced);
                write(fd[1], string, (strlen(string)+1));
                sleep(2);
            }
            close(fd[1]);

            exit(0);
        }
        else
        {
                /* Parent process closes up output side of pipe */
                //close(fd[1]);

                /* Read in a string from the pipe */
                while(1){
                    while ((nbytes = read(fd[0], readbuffer, sizeof(readbuffer))) > 0){
                        printf("%s\n", readbuffer);
                        fflush(stdout);
                    }
                }

        }
        
        return(0);
}
