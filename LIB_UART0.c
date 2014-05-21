//FIchier LIB_UART0  pour les fonctions relatives ‡ l'ADC0 donc la communication avec le pc
#include <intrins.h> 
#include "c8051F020.h"
#include "Ports_Declarations.h" 
#include "VarGlobales_Define.h"
#include "LIB_UART1.h"
#include "servo.h"
#include "mesure_distance.h"
#include "conversion_metre.h"
#include <stdlib.h>     /* pour la fonction atoi */
#include <string.h>
#include <stdio.h>




void CFG_UART0()
{
	//Configuration des ports en rapport avec l'UART0 :
	
	 XBR0      |= 0x04; //Activation de l'UART0 sur le crossbar
	 XBR2      |= 0x44;	//Activation du Crossbar
	 P0MDOUT   |= 0x41;	//Passage des sortie en PP
	

	//Configuration de l'UART :

	SCON0     |= 0x52;

	//Configuration du timer1 pour l'uart vitesse 19200
    CKCON     = 0x10;
    TCON      = 0x40;
    TMOD      = 0x20;
    TH1       = 0xDC;
	
	
	
}


char putchar_pc(char c) //Envoie d'un caractère
{
  if (c == '\n')  {		 //Si on doit faire un retour ‡ la ligne
    while (!TI0);
    TI0 = 0;
    SBUF0 = 0x0d;                         
  }
  while (!TI0);			 //Tant que le buffer n'est pas de nouveau disponible
  TI0 = 0;
	
  return (SBUF0 = c);	//On met le caractËre en buffer
}



char getchar_pc() //recoit un caratËre au PC
{
	char c;
	if(RI0==1)				//Si on a bien recu un caractËre
	{
		c=SBUF0;		   //On le rÈcupËre dans le buffer
		RI0=0;			   //On baisse le flag d'interuption
		return SBUF0;
		
	}
	else return 0;

}

char Send_String_pc(char *char_ptr) //Envoi une chaine de caratËres au PC
{
	char compteur=0;
	char retour=1;
	while(*char_ptr!='\0')		 //Tant que on est pas ‡ la fin de la chaine
	{
		compteur++;
		retour=putchar_pc(*char_ptr);	  //On envoie le caratËre
		char_ptr++;
	}
	return compteur;
}


void reception_pc() interrupt 4  //On met le caractËre dans un tableau de traitement de donnÈes
{	
	EA=0;
		if(RI0==1)//SI on a bien recu un caractËre
		{
			
			*p_recu_pc=getchar_pc(); //On le rÈcupËre
			if(*p_recu_pc=='Q')//arret d'urgence
			{
				avancer(0);
				RSTSRC|=0x10; //RESET

			}
			
			if(echo_actif==1) putchar_pc(*p_recu_pc);	//Ici on laisse l'Ècho actif pour voir ce que l'on tape

			if(*p_recu_pc=='\r') //Si c'est la fin de la commande
			{
				
				*p_recu_pc='\0'; //On cloture la chaine
				
								
				if(echo_actif==1)	//Debug 
				{
				Send_String_pc("\n--");
				Send_String_pc(" Commande recue :");
				Send_String_pc(message_recu_pc);
				Send_String_pc("--\n");
			 
				}
				strcpy(message_a_traiter,message_recu_pc); // On copie le message dans un autre tableau en attente de traitement
				p_recu_pc=message_recu_pc; //On rÈinit le pointeur	 
			
			}
			else 
			{
				p_recu_pc++;	   //Sinon on se place juste ‡ la case d'aprËs
				
			}
	 }

	EA=1;
}


void analyse_message(char *chaine, char separateur)
{
		//On aurait pu utiliser strtok mais la fonction n'est pas trËs sÈcurisÈ au niveau de la mÈmoire
		int i, j, k;
		i=0;
		j=0;
		k=0;
		while(chaine[i]!='\0') //On boucle jusqu'a la fin de la chaine 
		{
		 	if (chaine[i]==separateur) instructions[j][k]='\0', j++, k=-1; //Si on dÈtecte un sÈparateur, on cloture la chaine et on passe ‡ la case suivante.
			else instructions[j][k]= chaine[i];	 // SInon on copie simplement le caractËre.
			i++, k++;

		}
		instructions[j][k]='\0';   //On cloture la derniËre chaine.

}

void traitement_message()	  
{
		int xdata angle, j, n ;
		char xdata chaine[20];


	if(message_a_traiter[0]!='\0')	 //Si le tableau des ordres n'est pas vide
	{
		analyse_message(message_a_traiter, ' '); //On dÈcoupe le message en partie suivant les espaces.
		//dÈbug
		if(echo_actif==1) Send_String_pc("\n-- Message a traiter :"),Send_String_pc(instructions[0]),Send_String_pc("\n")	;
					


					if (!strcmp(instructions[0],"D"))  //DÈbut de l'Èpreuve
					{ 
						if(instructions[1]!=NULL && strcmp(instructions[1],"") && atoi(instructions[1])<4 && atoi(instructions[1])>0) //Si le premier arguement est un chiffre entre 0 et 3
						{
							cycle_actif=atoi(instructions[1]);
						}
						else cycle_actif=1; //Sinon programme de base 
	
						//On active le robot ici 
						robot_actif=1;
						Send_String_pc("I 'm Ready ! Cycle :");//on rend la main
						putchar_pc((char)cycle_actif+48);
						Send_String_pc("\n>");
					}


				   else if (!strcmp(instructions[0],"E"))  
				   { 
				   		//Fin de l'Èpreuve
				   		//On termine le travail et on renvoi les infos rÈcuperer par les fonctions des autres

				   }

				   else if (!strcmp(instructions[0],"TV")) //vitesse avancer
				   { 
					   //reglage vitesse
					   if(atoi(instructions[1])>=0 && atoi(instructions[1])<=100)
					   {
						   vitesse_avancer=atoi(instructions[1]);
						   Send_String_pc("\n>");
					   }
					   else Send_String_pc("##");
				   }

				   else if (!strcmp(instructions[0],"RV")) //vitesse avancer
				   { 
					   //reglage vitesse
					   if(atoi(instructions[1])>=0 && atoi(instructions[1])<=100)
					   {
						   vitesse_reculer=atoi(instructions[1]);	
						   Send_String_pc("\n>");
					   }
					   else Send_String_pc("##");
				   }

				   else if (!strcmp(instructions[0],"A"))  
				   { 
				   		//On avance a une vitesse passÈ par TV ou sinon par l'argument
				   		//

						if(atoi(instructions[1])>=10 && atoi(instructions[1])<=100)
					   {
						   avancer(atoi(instructions[1]));
					   }
					   else avancer(vitesse_avancer);
					   Send_String_pc("\n>");

				   }

				   else if (!strcmp(instructions[0],"B"))  
				   { 
				   		//On recule a une vitesse passÈ par RV ou sinon par l'argument
				   		//

						if(atoi(instructions[1])>=10 && atoi(instructions[1])<=100)
					   {
						   avancer(-1*atoi(instructions[1]));
					   }
					   else avancer(-1*vitesse_reculer);
					   Send_String_pc("\n>");

				   }
				   else if (!strcmp(instructions[0],"S"))  
				   { 
				   		// on arrete le dÈplacement
				   		//
						avancer(0);
						Send_String_pc("\n>");

				   }
				   else if (!strcmp(instructions[0],"RD"))  
				   { 
				   		// rotation droite 90∞
				   		//
						rotation(90);
						Send_String_pc("\n>");

				   }
				   else if (!strcmp(instructions[0],"RG"))  
				   { 
				   		// rotation droite -90∞	ou gauche
				   		//
						rotation(-90);
						Send_String_pc("\n>");

				   }

				   else if (!strcmp(instructions[0],"RC"))  
				   { 
				   		// rotation 180 droite par dÈfaut
				   		//
						if (!strcmp(instructions[1],"G"))rotation(-180);
						else rotation(180); 
						Send_String_pc("\n>");

				   }
					 else if (!strcmp(instructions[0],"RA"))  
				   { 
				   	
						 if(instructions[1][0]=='G')
						 rotation(atoi(instructions[1]+2));
						 else	
						 rotation(-atoi(instructions[1]+2));

						 Send_String_pc("\n>");

				   }
				   else if (!strcmp(instructions[0],"G"))  
				   { 
				   	// deplacement par coordonnÈe
						//
						//On rÈcupËre les coordonnÈes puis on les stoquerais dans un tableau en brute, une autres fonction les annalyserait
						//Format : G X:valeur Y:valeur A:angle
						 avancer_coord(atoi(instructions[2]+2));
						 wait(atoi(instructions[2]+2)/10);
						 rotation(90);
						 	wait(1);
						 avancer_coord(atoi(instructions[1]+2));
						 wait(atoi(instructions[1]+2)/10);
						 rotation(atoi(instructions[3]+2));
						 	wait(1);
						 Send_String_pc("\nB\n>");

				   }

				   else if (!strcmp(instructions[0],"F"))  
				   { 
				   		//	  Point fixe
						//

						avancer(0);
						 
						 						 if(instructions[1][0]=='A') 
						 {
							 analyse_message(instructions[1],':');
							 if(atoi(instructions[1])<180 && atoi(instructions[1])>4)
							 {
									angle=atoi(instructions[1]);
							 }
							 	else
								 {
									  angle=30;		
								 }
						 }
							 else if(instructions[2][0]=='A') 
						   {
							 analyse_message(instructions[2],':');
								 
								 if(atoi(instructions[1])<180 && atoi(instructions[1])>4)
								 {
									angle=atoi(instructions[1]);
								 }
								 else
								 {
									  angle=30;
								 }
							 
							 
						 }
						 else angle=30;
						 
						 testScrutation(angle);
						 
						 //AFIICHAGE
						Send_String_pc("KOI "); 
						if(strcmp(instructions[1],"D"))
					   {
								n=0;
								for(j=0; j<181; j+=angle)
								{
									sprintf(chaine,"%d:%f ",j,Tab_distances_devant[n]);
									n++;
									Send_String_pc(chaine);
								}
						   
					   }
						 else 
						 {//360 deg
								n=0;
								for(j=0; j<181; j+=angle)
								{
									sprintf(chaine,"%d:%f ",j,Tab_distances_devant[n]);
									n++;
									Send_String_pc(chaine);
								}
								n=0;
								j=0;
								for(j=180; j<361; j+=angle)
								{
									sprintf(chaine,"%d:%f ",j,Tab_distances_derriere[n]);
									n++;
									Send_String_pc(chaine);
								}
						 }
							 
						//On rÈcupere les infos grace aux fonctions des autres

						Send_String_pc("\n>");


				   }

				   else if (!strcmp(instructions[0],"Y"))  
				   { 
				   		//  SON 
						//
						//test dÈcoupage
						if(echo_actif==1)
						{
							analyse_message(instructions[1],':');
						    Send_String_pc(instructions[0]);
							Send_String_pc(instructions[1]);
							Send_String_pc(instructions[2]);
						}
						//On appelerait la fonction de gestion du son avec les arguments du tableau
							Send_String_pc("\n>");

				   }
					////////////////////////////////////MESURES///////////////////////
					//On appelerait les fonctions quand il le faudrait ...
				   else if (!strcmp(instructions[0],"MI"))  
				   { 
				   		// mesure courant
						//
					
						Send_String_pc("KI xxxxx\n>");
				   }

				   else if (!strcmp(instructions[0],"MS"))  
				   { 
				   		// mesure son
						//
					   Send_String_pc("KA xx\n>");
				   }

				   else if (!strcmp(instructions[0],"MOI"))  
				   { 						 				 
						 if(instructions[1][0]=='A') 
						 {
							 analyse_message(instructions[1],':');
							 if(atoi(instructions[1])<180 && atoi(instructions[1])>4)
							 {
									angle=atoi(instructions[1]);
							 }
							 	else
								 {
									  angle=30;		
								 }
						 }
							 else if(instructions[2][0]=='A') 
						   {
							 analyse_message(instructions[2],':');
								 
								 if(atoi(instructions[1])<180 && atoi(instructions[1])>4)
								 {
									angle=atoi(instructions[1]);
								 }
								 else
								 {
									  angle=30;
								 }
							 
							 
						 }
						 else angle=30;
						 
						 testScrutation(angle);
						 
						 //AFIICHAGE
						Send_String_pc("KOI "); 
						if(strcmp(instructions[1],"D"))
					   {
								n=0;
								for(j=0; j<181; j+=angle)
								{
									sprintf(chaine,"%d:%f ",j,Tab_distances_devant[n]);
									n++;
									Send_String_pc(chaine);
								}
						   
					   }
						 else 
						 {//360 deg
								n=0;
								for(j=0; j<181; j+=angle)
								{
									sprintf(chaine,"%d:%f ",j,Tab_distances_devant[n]);
									n++;
									Send_String_pc(chaine);
								}
								n=0;
								j=0;
								for(j=180; j<361; j+=angle)
								{
									sprintf(chaine,"%d:%f ",j,Tab_distances_derriere[n]);
									n++;
									Send_String_pc(chaine);
								}
						 }
							 
			   		
						
						Send_String_pc("\n>");
				   }

				   else if (!strcmp(instructions[0],"MOU"))  
				   { 
				   		// ultrason
						//
						Send_String_pc("\n>");
				   }

				   else if (!strcmp(instructions[0],"ML"))  
				   { 
				   		// LumiËre
						//
						 Send_String_pc("\n>");
				   }


					else Send_String_pc("##"); 
				 //Reset du buffet
				strcpy(message_a_traiter,"");
				instructions[1][0]='\0';
				instructions[2][0]='\0';
				instructions[3][0]='\0';
				if(echo_actif==1) Send_String_pc("\n-- Buffer vide --\n");
				}

	
}