#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int i = 0;
	int a = 1; //me auth thn metavlhth elegxoume an h diergasia einai o pateras
	for(i = 0; i < 4; ++i)
	{
		if (a == 1)
		{
			if(fork() == 0) //h fork epistrfei 0 mono gia tis child diergasies
			{
				a = 0; //kanoume thn a=0 gia oles tis child diergasies gia na mhn ftiaxnoun nees diergasies ta paidia ths proths
			}	
		}
   	}
	
	printf("Process ID : %d Father process ID : %d \n", getpid(), getppid()) ;

	sleep(5);
	return (0);
}
