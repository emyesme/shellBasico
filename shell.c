#include <stdio.h>

#include "execute.h"

int main(int argc, char*argv[]) {
	
	while(1){
		char *texto = malloc(80*sizeof (char));
		
		printf("$>");
		
		int vacio = scanf("%[^\n]%*c", texto);
		
		if(vacio==0 || !strcmp(texto, "exit")){
			break;
		}
		
		char* comando = strtok (texto,">");//toma la informacion escrita antes de simbolo ">"
		char* archivo = strtok (NULL," ");//toma lo que queda de texto despues del ">"
		
		if(execute(comando, archivo)){
			break;
		}
		
		free(texto);
	}
	return 0;
}
