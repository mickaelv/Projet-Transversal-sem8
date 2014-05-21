//FIchier LIB_UART1  pour les fonctions relatives à l'ADC0 donc la communication avec le pc
#include <intrins.h> 
#include "c8051F020.h"
#include "Ports_Declarations.h" 
#include "VarGlobales_Define.h"
#include "LIB_UART0.h"
#include "LIB_UART1.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
		  
		  /*

		   RI1 (SCON1&0x01)
		   TI1 (SCON1&0x02)
		  */

void CFG_UART1()
{
	//Configuration des ports en rapport avec l'UART1 :
	
//	 XBR0      |= 0x04; //Activation de l'UART0 sur le crossbar
//	 XBR2      |= 0x44;	//Activation du Crossbar
//	 P0MDOUT   |= 0xC0;	//Passage des sortie en PP
	
	
//	
	  P0MDOUT   |= 0x41;
    XBR0      |= 0x06;
    XBR2      |= 0x44;


	
	//Configuration de l'UART 1:

		SCON1 = 0x52;
		SCON1|=0x02; //TI1 OK
	
		//Activation des interuptions
		//EIE2      = 0x40;

		IE        |= 0x94;
//Activation de l'interuption l'UART, timer et des interuptions Générales
  //Activation de l'interuption l'UART et des interuptions Générales
	EA=0;
}




char putchar_serializer (char c) //Envoi un caratère à la carte serializer
{
  if (c == '\n')  {
    while ((SCON1&0x02)==0x00);	  //Si buffer 1 dispo
    SCON1 &= 0xFC;				//On baisse le flag
    SBUF1 = 0x0d;               //On envoit un retour a la ligne          
  }
	
  while ((SCON1&0x02)==0x00);
  SCON1 &= 0xFC;
	
  return (SBUF1 = c);
}


void inter1() interrupt 20
{
	SCON1 &= 0xFC; //if((SCON1&0x01)==1) 


}


//char getchar_serializer() //recoit un caratère à la carte serializer  fonction inutile
//{
//	char c;
//	if((SCON1&0x01)==0x01)
//	{
//		c=SBUF1;
//		SCON1 &= 0xFE;
//		return SBUF1;
//		
//	}
//	else return 0;

//}

char Send_String_serializer(char *char_ptr) //Envoi une chaine de caratères à la carte serializer comme dans UART0
{
	char compteur=0;
	char retour=1;
	if(echo_actif==1) Send_String_pc("UART1:");
	while(*char_ptr!='\0')
	{
		compteur++;
		retour=putchar_serializer(*char_ptr);
		if(echo_actif==1) putchar_pc(*char_ptr);
		char_ptr++;
	}
	if(echo_actif==1) Send_String_pc("\n");

	return compteur;
}

void cfg_serializer()	//Configuration de la serializer
{
	Send_String_serializer("cfg baud 19200\r");
	Send_String_serializer("cfg units 0\r");
	Send_String_serializer("stop\r");

}

void avancer(int valeur)
{
	
  char xdata chaine[4];
	EA=0;
   sprintf(chaine, "%d", valeur);	//Conversion d'un int en chaine
   
   if(valeur==0) Send_String_serializer("stop\r");	 //Arret des moteurs
   else
   {
	   Send_String_serializer("pwm 1:");
	   Send_String_serializer(chaine);
	   Send_String_serializer(" 2:");
	   Send_String_serializer(chaine);
	   Send_String_serializer("\r");
		 
   }
	 EA=1;
}

void rotation(int valeur)
{	

	  char xdata chaine[4];
		int tick=floor(valeur*135.88*52*120/(360*194.78)/2); //CC Capucine 
		EA=0;

  if(valeur==0) Send_String_serializer("stop\r");
   else
   {
	   Send_String_serializer("digo 1:");
	   sprintf(chaine, "%d", tick);
	   Send_String_serializer(chaine);
	   Send_String_serializer(":100 2:");
		 sprintf(chaine, "%d", -tick);
	   Send_String_serializer(chaine);
	   Send_String_serializer(":100\r");	 
		 
   }
	 //wait(5);
	 		 EA=1;

}

void avancer_coord(int valeur)
{	
	  char xdata chaine[10];
		int xdata tick=floor(624/20.106*valeur); //CC Capucine 
		EA=0;

  if(valeur==0) Send_String_serializer("stop\r");
   else
   {
	   Send_String_serializer("digo 1:");
	   sprintf(chaine, "%d", tick);
	   Send_String_serializer(chaine);
	   Send_String_serializer(":100 2:");
	   Send_String_serializer(chaine);
	   Send_String_serializer(":100\r");	 
		 
   }
	//wait(3);
	EA=1;

}

void wait(int temps)
{
int i=0,j=0;

	for(i=0;i<temps*10000;i++) _nop_(); //durée étalon: on souhaiterait 1ms

}


