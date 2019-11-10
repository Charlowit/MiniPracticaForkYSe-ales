#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <fcntl.h>

#define MAX_BUF 1024
using namespace std;


void manejadorTotal0( int segnal )
{
    printf(" Paquete mal estructurado... Nº senial: %d\n", segnal);
}

void manejadorTotal1( int segnal )
{
    printf(" Paquete bien estructurado...Nº senial: %d\n", segnal);
}

int main(){

	pid_t pid, pid2;

	int fd1[2];
	//int fd2[2];
	int contador = 0;
	int numero = 0;
	int suma = 0;
	int total = 0;
	bool check = false;
	//char buf[MAX_BUF];



	pipe(fd1);
	//pipe(fd2);
	
	pid = fork(); 


	switch(pid){
		case -1:
			cerr << "Failed to fork" << endl;
			exit(1);
		case 0:

		pid2 = fork();
            
		switch (pid2) {
			case -1:
				cerr << "Failed to fork" << endl;
				exit(1);
		    
			case 0:
		    
				signal(SIGUSR1, manejadorTotal0);
				signal(SIGUSR2, manejadorTotal1);
				break;
		    
			default:
			while(1){
				check = false;
				suma = 0;
				contador = 0;
				stringstream ss;
				string cadena = "";
				while(check == false){

	 
					cin >> numero;
					
					ss << numero;
					cadena = ss.str();
					//cout << " CADENA " << cadena << endl;
					if(numero >= 0 && contador == 0){

						suma += numero;

					}
					else if (numero < 0){

						if(numero == -1){

							check = true;
							sleep(1);
							close(fd1[0]);


						}
						else{
							suma = 0;
							contador++;
						
						}
					}
				}

				if (suma == 0){
					kill(pid2, SIGUSR1);
					cout << " Cadena Enviada: " << cadena << endl;
					write(fd1[1], &suma, sizeof(suma));
				}
				else{
					kill(pid2,SIGUSR2);
					cout << " Cadena Enviada: " << cadena << endl;
					write(fd1[1], &suma, sizeof(suma));
				}
			}
		}
			
                default:
			while(1){
				//close(fd2[1]);
				close(fd1[1]);
				//read(fd2[0], buf, MAX_BUF);
				read(fd1[0], &total, sizeof(total));
				cout  << " Total Suma cadena " << total << endl << "------------------------" << endl;
			}
	}


}
