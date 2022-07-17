#include <stdio.h>          
#include <stdlib.h>         
#include <sys/types.h>      
#include <sys/shm.h>        
#include <errno.h>          
#include <semaphore.h>      
#include <fcntl.h>          
#include <unistd.h>
#include <sys/wait.h>

typedef sem_t Semaphore; 

void display(char *str);

Semaphore *synch1; 
Semaphore *synch2;

int main()
{
	int   i;                        /*      loop variables  */
	pid_t pid[5];                   /*      fork pid        */
	int child_status;
	

	synch1 = sem_open ("Sem1", O_CREAT | O_EXCL, 0644, 0); //arxikopoihsh shmaforon
        synch2 = sem_open ("Sem2", O_CREAT | O_EXCL, 0644, 0);
	
	for (i=0; i<5; i++) /*ston pinaka pid kathe child process exei 0 se mia mono 8esh kai 
				to xrhsimophoume gia na ksexorisoume ta kommatia kodika pou 8a treksei kaue diergasia*/
	{
        	pid[i] = fork ();

        	if (pid[i] == 0) 
		{
          		break;
        	}
	}	
	
	//kodikas gia kathe diergasia
	    /* 1st child */
    	if (pid[0] == 0)
    	{
		system("ls -l");
		sem_post (synch1);//auksanei ton shmaforo 1 kata 1
   	}
   	/* 2nd child */
   	else if (pid[0] != 0 && pid[1] == 0)
   	{
       		system("ls -l");
       		sem_post (synch1);//auksanei ton shmaforo 1 kata 1
   	}
   	/* 3rd child */
   	else if (pid[0] != 0 && pid[1] != 0 && pid[2] == 0)
   	{
		system("ls -l");
       		sem_post (synch2);//auksanei ton shmaforo 2 kata 1
   	}
   	/* 4th child */
   	else if (pid[0] != 0 && pid[1] != 0 && pid[2] != 0 && pid[3] == 0)
   	{
       		sem_wait (synch1);// meionei ton shmaforo 1 kata 1 kai perimenei na ksana aukshthei gia na ektelestei
		system("ls -l");
       		sem_post (synch2);//auksanei ton shmaforo 2 kata 1
   	}
	else if (pid[0] != 0 && pid[1] != 0 && pid[2] != 0 && pid[3] != 0 && pid[4] == 0)
        {
		sem_wait (synch2);//meionei ton shmaforo 2 kata 1 kai perimenei kapoion na ton aukhsei gia na ektelestei
                system("ls -l");
        }
   	else
   	{
		//ektelesh se periptosh error
		for (i = 0; i < 5; i++)
		{
	    		pid_t wpid = waitpid(pid[i], &child_status, 0);

	    		if (WIFEXITED(child_status))
            		{
               		printf("Child: %d terminated with exit status %d\n", wpid, WEXITSTATUS(child_status));
            		}
            		else
            		{	
               			printf("Child: %d terminate abnormally\n", wpid);
            		}
        	}	


        	printf(" Parent unlinks the semaphores\n Bye\n");

        	/* kanoume unlink tous shmaforous gia na mhn uparxoun gia panta */
        	/* an paei kati strava kata thn ektelesh         */
        	sem_unlink ("Sem1");   
        	sem_close(synch1);  
        	sem_unlink ("Sem2");   
        	sem_close(synch2);   
    	}
	exit (0);
}
