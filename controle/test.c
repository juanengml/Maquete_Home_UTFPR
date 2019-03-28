#include <stdio.h>
#include <stdlib.h>


int leds[7] =   {12,11,10,9,8,7,2};
int status[7] = {0 ,0 ,0 ,0,0,0,0};

void controle(int pino,int opcao);

void main(){
   int i = 0;
   int pos = 0,cmd = 0;
   // status
   for(i = 0; i < 7; i++){
   	printf("%d %d\n",leds[i],status[i]);
   }
   
   // cmd
   printf("Digite Pino e Comando: ");
   scanf("%d %d",&pos,&cmd);
   
   // status
   controle(pos,cmd);
   
}

void controle(int pino,int opcao){
   int i = 0;
   for(i = 0; i < 7;i++){
   	 if (leds[i] == pino){
   	 	status[i] = opcao;
   	 }
   	 printf("%d %d\n",leds[i],status[i]);
   }
}
