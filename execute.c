#include "execute.h"

/**execute recibe los comandos y si hay el nombre del archivo para el flujo sino NULL*/
int execute (char * comando, char * archivo){
	
		int pipesCont = countDiv(strdup(comando),"|");
		
		char *pipesdiv[pipesCont];
		divStr(pipesdiv,strdup(comando),"|");


		char **arreglo[pipesCont];  // En la primera dimension el texo esta separado por pipes, en la segunda por espacios
		int sizes[pipesCont];
		int validation = 0;

		for(int i=0;  i<pipesCont; i++){
			fillMatrix(arreglo, pipesdiv[i], sizes, i);
			
			validation += validateEntry(arreglo[i],sizes[i]);
		}
		
		
		if(validation == pipesCont){
			//hijo principal
			int pid;

			if(!(pid = fork())){
				if(archivo!=NULL){//si se guardo algo en archivo
					archivo= strtok(archivo," ");
						
					close(STDOUT_FILENO);
					open(archivo, O_EXCL|O_CREAT|O_WRONLY, S_IRWXU);
				}
				
				if(pipesCont > 1){
					conPipes(arreglo);	
				}else{	
					sinPipes(arreglo);
				}
				return 1;
			}else{
				wait(NULL);
				return 0;
			}
		}
	
	return 0;
}


/**conPipes recibe la matriz de comandos dividido en filas por la parte derecha e izquierda de pipe y los comandos en cada elemento de la matriz**/
void conPipes (char **arreglo[]){
	int pipefd[2];//un canal de escritura y otro de lectura
	pipe(pipefd);//crea los nuevos canales de entrada y salida
	int pidPipe1;
	int pidPipe2;

	if(!(pidPipe1 = fork())){
		//hijo encargado de ejecutar parte izquierda
		dup2(pipefd[1], STDOUT_FILENO);//recibe un canal y cierra el canal dado en el segundo parametro
		close(pipefd[0]);
		//cuando se ejecute el execvp va a enviar su resultado a traves del canal pipefd[1]
		execvp(arreglo[0][0], arreglo[0]);
	}
	
	wait(NULL);
	
	if(!(pidPipe2 = fork())){
		//hijo encargado de ejecutar parte derecha
		dup2(pipefd[0], STDIN_FILENO);//como se escribio en pipefd[1] se puede leer esto en el pipefd[0]
		close(pipefd[1]);
		//como en el canal de lectura pipedf[0] hay informacion el execvp lo toma como entrada para su ejecución
		execvp(arreglo[1][0], arreglo[1]);
	}else{
		close(pipefd[0]);
		close(pipefd[1]);

		wait(NULL);
	}
}
/**sinPipes recibe la matriz de comandos divido en filas por la parte derecha e izquierda de pipe y los comandos en cada elemento de la matriz**/
void sinPipes (char **arreglo[]){
	execvp(arreglo[0][0], arreglo[0]);
}


//countDiv recibe una cadena y un caracter y cuenta la cantidad de divisiones en las que se
// puede separar la cadena usando el caracter dado
int countDiv(char *text, char *sep){
	int cont = 0;
	char *div = strtok(text,sep);

	while(div != NULL){ 
		cont++;
		div = strtok(NULL,sep);
	}

	return cont;
}


//divStr asigna a array cada una de las divisiones dadas por el separador
void divStr(char *array[],char *text, char *sep){
	char *div = strtok(text,sep);

	for(int i=0; div!=NULL; i++){
		array[i] = div;
		div = strtok(NULL,sep);
	}
}

// validateEntry valida los comandos
// retorna 0 si encuentra algun error
int validateEntry(char *wDiv[],int size){
	int validation = 1;
	for(int i=0; i<size; i++){
		if(!strcmp(wDiv[i],"top")){
			if(size>1){
				printf("Muchos argumentos para el comando top.\n");
				validation = 0;
			}else{
				printf("Comando %s encontrado.\n",wDiv[i]);
			}
		}else if(!strcmp(wDiv[i],"ps")){
			if(size>1){
				printf("Muchos argumentos para el comando ps.\n");
				validation = 0;
			}else{
				printf("Comando %s encontrado.\n",wDiv[i]);
			}
		}else if(!strcmp(wDiv[i],"ls")){
			if(size>2){
				printf("Muchos argumentos para el comando ls.\n");
				validation = 0;
			}else{
				printf("Comando %s encontrado.\n",wDiv[i]);
			}
		}else if(!strcmp(wDiv[i],"-la")){
			printf("Comando %s encontrado.\n",wDiv[i]);
		}else if(!strcmp(wDiv[i],"cat")){
			printf("Comando %s encontrado.\n",wDiv[i]);
			break;
		}else if(!strcmp(wDiv[i],"grep")){
			printf("Comando %s encontrado.\n",wDiv[i]);
			break;
		}else if(!strcmp(wDiv[i],"sort")){
			printf("Comando %s encontrado.\n",wDiv[i]);
			break;
		}else if(!strcmp(wDiv[i],"wc")){
			printf("Comando %s encontrado.\n",wDiv[i]);
		}else if(!strcmp(wDiv[i],"-l")){
			printf("Comando %s encontrado.\n",wDiv[i]);
		}else{
			printf("Comando %s no encontrado.\n",wDiv[i]);
			validation = 0;
		}
	}
	return validation;
}

//fillMatrix recibe una matriz de punteros, un texto, un arreglo con los tamanos de cada una de las filas 
//y un entero de posicion este le asigna a array los arreglos que contienen cada una
// de las palabras en las divisiones del texto separado por pipes
void fillMatrix(char **array[], char *text, int *sizes, int  index){
	int cont = countDiv(strdup(text)," ");
	char **wDiv = malloc((cont+1)*sizeof(char*));
	array[index] = wDiv;
	sizes[index] = cont;

	divStr(wDiv,strdup(text)," ");

	wDiv[cont] = NULL;
}


