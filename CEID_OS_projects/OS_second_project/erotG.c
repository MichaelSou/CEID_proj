#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>



int chain_process(int n) 
{
    if (n == 0) //an to n einai 0 shmainei oti ftiaksame oles tis nees diergasies
    { 
        return 0;
    }
    int pid = fork(); //h fork epistrefei pid=0 gia ta paidia kai to pid tou paidiou ston patera
    if (pid == -1) { //an to pid einai arnhtiko h fork ebgale error
        exit(0);
    }
    if (pid==0) { /*an to pid einai 0 tote h diergasia einai child emfanizei to pid ths kai to 
			pid tou patera ths meionei to n kai kalei pali thn synarthsh chain_process gia na ftiaksei 
			mono auth neo process kai na einai alusida ta processes*/
        printf("Process ID : %d Parent PID : %d \n", getpid(), getppid());
        n = n-1;
        chain_process(n);
        exit(0);
    }
    else {
       wait(NULL);
    } 
    return 0;   
}

int main()
{
	chain_process(5); // kaloume thn chain_process me n=5 gia na ftiaksoume 5 diergasies alusida
	return 0;
}
