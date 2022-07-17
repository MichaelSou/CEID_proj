#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>


int nothing() // h nothing kanei x+1
{
	int x=0;
	x=x+1;
	printf("%d", x);
}

int main()
{
	int status;
	int check = 1;
	double start=(double)time(NULL); //pairnoume ton xrono se deuteropleta
	printf("Αρχική τιμή δευτερολέπτων: %f sec.\n", start); //emfanizoume thn start
	int i = 0;
	int a = 1;//auth thn metavlhth thn exei 1 mono o pateras gia na ton ksexorizoume
	for(i = 0; i < 100; ++i)
	{
		if (a == 1) //mono to proto process ektelei auto
		{
			check = fork();//h check exei timh 0 gia to child process kai to pid tou child sthn parent process
			if (check < 0) // an h check einai arnhtikh epistrefei error
			{
				printf("error");
				exit(0);
			}
			if(check == 0) // auth h if ekteleite mono apo to child process kai kanei to a=0 8eloume mono to proto process na exei a=1
			{
				a = 0;
			}	
		}
   	}

	nothing();
	if (a == 1)//mono o pateras ektelei auton ton kodika
	{
		while ((check=waitpid(-1,&status,0))!=-1) //perimenoume na teliosoun ta child processes
		{

		}
		double end=(double)time(NULL);//pairnoume to time afotou exei teleiosei to programa
		printf("Τελική τιμή δευτερολέπτων: %f sec.\n", end);//emfanizoume to end
		double mesh = end-start;
		printf("End-Start: %f sec.\n", mesh);
		double mo = (mesh)/100;// vriskoume meso oro ekteleshs
		printf("μέσος χρόνος δημιουργίας, εκτέλεσης και τερματισμού των 100 διεργασιών. %f .sec\n", mo);//emfanizoume to meso oro

	}
	return 0;
}
