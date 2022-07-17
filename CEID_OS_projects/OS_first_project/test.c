#include <stdio.h>
#include <stdlib.h>

int main()
{
	fork();
	fork();
 	printf("Process ID : %d\n", getpid());
   
    return (0);
}
