#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <stdlib.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <dirent.h> 
#include<stdlib.h>
#include<sys/wait.h>


int main(int argc, char* argv[])
{
	int liczba;
	char dane_wyslane[50];
	char dane_odebrane[50];
	int klient_int, serv_int;
	liczba = atoi(argv[1]);
	int i, pusta = 0;
	
	FILE *in;
	char plik[50];
	
	char serwer[] = "/home/ubuntu/Desktop/Lab3/Zad2/serwer";
	mkfifo(serwer, 0666);
	
	for(int i = 0; i<liczba; i++)
	{
		switch(fork())
		{
			case 0:
			sprintf(dane_wyslane, "/home/ubuntu/Desktop/Lab3/Zad2/fifo_%d", getpid());
			klient_int = open(serwer, O_WRONLY);
			write(klient_int, dane_wyslane, sizeof(dane_wyslane));
			close(klient_int);
			
			while(open(dane_wyslane, O_RDONLY) < 0)
			{
				pusta++;
			}
			
			serv_int = open(dane_wyslane, O_RDONLY);
			read(serv_int, dane_odebrane, 50);
			printf("Klient [%d(%d)]: %s", i, getpid(), dane_odebrane);
			
			while(read(serv_int, dane_odebrane, 50))
			{
				printf("%s", dane_odebrane);
			}
			printf("\n");
			close(serv_int);
			wait(NULL);
			exit(0);
			
			case -1:
    			printf("Error");
    			exit (1);
		}
	}
	
	klient_int = open(serwer, O_RDONLY);
	while(read(klient_int, dane_odebrane, 50))
	{
		mkfifo(dane_odebrane, 0666);
		in = popen("ls","r");
		serv_int = open(dane_odebrane, O_WRONLY);
		
		while(fgets(plik, 50, in) != NULL)
		{
			write(serv_int, plik, 50);
		}
		pclose(in);
		close(serv_int);
		i++;
		
	}
	close(klient_int);
	return 0;
}
