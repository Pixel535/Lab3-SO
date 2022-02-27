#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>


int main()
{
	int pipe1[2];
	int pipe2[2];
	int pipe3[2];
	int pipe4[2];
	char string[100];
	char string2[100];
	char string3[100];
	
	if (pipe(pipe1)==-1)
    	{
        	fprintf(stderr, "Pipe ERROR" );
        	return 1;
    	}
    	if (pipe(pipe2)==-1)
    	{
        	fprintf(stderr, "Pipe ERROR" );
        	return 1;
    	}
    	if (pipe(pipe3)==-1)
    	{
        	fprintf(stderr, "Pipe ERROR" );
        	return 1;
    	}
    	if (pipe(pipe4)==-1)
    	{
        	fprintf(stderr, "Pipe ERROR" );
        	return 1;
    	}
    	
    	
    	switch( fork() )
    	{
    		case 0:
    		close(pipe1[0]);
    		
    		printf("PP1 [pid: %d] - Podaj tekst do wpisania: ", getpid());
    		scanf("%s", string);
    		printf("PP1 [pid: %d, id: %d] - Wyslano do PM: %s\n", getpid(), pipe1[1], string);
    		write(pipe1[1], string, sizeof(string));
    		
    		close(pipe1[1]);
    		close(pipe2[1]);
    		close(pipe3[1]);
    		close(pipe4[1]);
    		
    		close(pipe2[0]);
    		close(pipe3[0]);
    		
    		while(read(pipe4[0], string3, sizeof(string3)))
    		{
    			printf("PP1 [pid: %d, id: %d] - Odczytano od PM: %s\n", getpid(), pipe4[0], string3);
    		}
    		close(pipe4[0]);
    		wait(NULL);
		exit(0);
    		
    		case -1:
		printf("Error");
    		exit (1);
    		
    		default:
    		close(pipe1[1]);
    		while(read(pipe1[0], string, sizeof(string3)))
    		{
    			printf("PM [pid: %d, id: %d] - Odczytano od PP1: %s\n", getpid(), pipe1[0], string);
    		}
    		close(pipe1[0]);
    		
    		switch( fork() )
    		{
    			case 0:
    			close(pipe2[1]);
    			close(pipe4[1]);
    			close(pipe4[0]);
    			while(read(pipe2[0], string2, sizeof(string2)))
    			{
    				printf("PP2 [pid: %d, id: %d] - Odczytano od PM: %s\n", getpid(), pipe2[0], string2);
    			}
    			close(pipe2[0]);
    			for(int i=0;i<50;i++)
			{
				if (string2[i] >= 'A' && string2[i] <= 'Z')
				{
					string2[i] += 32;
				}
				else if(string2[i] >= 'a' && string2[i]<= 'z')
				{
					string2[i] -= 32;
				}
			}
			close(pipe3[0]);
			printf("PP2 [pid: %d, id: %d] - Wyslano do PM: %s\n", getpid(), pipe3[1], string2);
			write(pipe3[1], string2, sizeof(string2));
			close(pipe3[1]);
    			wait(NULL);
			exit(0);
    			
    			case -1:
    			printf("Error");
    			exit (1);
    			
    			default:
    			close(pipe2[0]);
    			printf("PM [pid: %d, id: %d] - Wyslano do PP2: %s\n", getpid(), pipe2[1], string);
    			write(pipe2[1], string, sizeof(string));
    			close(pipe2[1]);
    			close(pipe3[1]);
    			while(read(pipe3[0], string2, sizeof(string2)))
    			{
    				printf("PM [pid: %d, id: %d] - Odczytano od PP2: %s\n", getpid(), pipe3[0], string2);
    			}
			close(pipe3[0]);
    			close(pipe4[0]);
    			printf("PM [pid: %d, id: %d] - Wyslano do PP1: %s\n", getpid(), pipe4[1], string2);
    			write(pipe4[1], string2, sizeof(string2));
    			close(pipe4[1]);
    			wait(NULL);
			exit(0);
    		}
    		exit (0);
    		
    		

    	}
	
	return 0;
}
