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

#define RESTORE "\033[1;0m"
#define GREY "\033[1;30m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"
#define BLACK "\033[1;38m"
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
	int contador = 0;
	int numero = 0;
	int suma = 0;
	int total = 0;
	bool check = false;

	cout << GREEN << endl << "------------------------------------------------------------------------------" << RESTORE << endl;
	cout << BLUE << endl << "Bienvenido a la torre de control del Satelite a continuación deberá enviar los paquetes separados por espacios terminando cada paquete en -1" << RESTORE << endl;
	cout << GREEN << endl << "------------------------------------------------------------------------------" << RESTORE << endl;
	cout << PURPLE << "Ejemplo de introducción de paquetes bien estructurado: 1 2 3 4 -1 , recuerda todos numeros introducidos deben ser positivos, en caso de no serlo nuestro sistema lo detectara y el paquete quedará anulado" << RESTORE << endl;
	cout << GREEN << endl << "------------------------------------------------------------------------------" << RESTORE << endl;
	cout << RED << "Ejemplo paquete mal estructurado: 1 2 3 -5 -1" << RESTORE << endl;

	cout << endl << "Puede empezar: " << endl;

	pipe(fd1);

	
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
					cout << " Cadena Enviada: " << cadena << endl;
					sleep(3);
					kill(pid2,SIGUSR1);
					sleep(3);
					write(fd1[1], &suma, sizeof(suma));
				}
				else{
					cout << " Cadena Enviada: " << cadena << endl;
					sleep(3);
					kill(pid2,SIGUSR2);
					sleep(3);
					write(fd1[1], &suma, sizeof(suma));
				}
			}
		}
			
                default:
			while(1){
				close(fd1[1]);
				read(fd1[0], &total, sizeof(total));
				cout  << " Total Suma cadena: " << total << "\n" <<" ---------------------" << endl;
			}
	}


}
