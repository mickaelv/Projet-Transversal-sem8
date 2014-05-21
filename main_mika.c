//Fichier MAIN.C pour les tests des UARTs


#include <intrins.h> 
#include "c8051F020.h"
#include "Ports_Declarations.h" 
#include "VarGlobales_Define.h"
#include  "LIB_UART0.h"
#include  "LIB_UART1.h"
#include "servo.h"
#include "mesure_distance.h"
#include "conversion_metre.h"
#include "ADC1_Init.h"


//Variables globales mika
 char xdata message_recu_pc[TAILLE_BUFFER];
 char xdata echo_actif;
 char xdata robot_actif;
 char *p_recu_pc;
 char xdata message_a_traiter[TAILLE_BUFFER];
 char xdata cycle_actif;
 char xdata vitesse_avancer;
 char xdata vitesse_reculer;
 char xdata instructions[5][10];
 double xdata  Tab_distances_devant[40];
 double xdata  Tab_distances_derriere[40];
 
 
void config_mika()
{
	
	CFG_UART0();
	CFG_UART1();
	cfg_serializer();
	p_recu_pc=message_recu_pc;	
	*p_recu_pc='\0';
	message_a_traiter[0]='\0';
	echo_actif=1;
	vitesse_avancer=100;
  vitesse_reculer=100;	
	instructions[0][0]='\0';
}

void config_louis()
{
	 initialisation_PCA ();
	 initialisation_Ports ();
	 ADC1_Init ();
}

void main()
{
	int xdata j=0;


	
//config pour les tests
	//clock
    int i = 0;
    OSCXCN    = 0x67;
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);
		OSCICN    = 0x08;
		EA=0;
	
	//WD
    WDTCN     = 0xDE;
    WDTCN     = 0xAD;
	
	//Config perso
	config_mika();
	
	//config perso
	config_louis();
	
	EA=1; 
	
 if (echo_actif) Send_String_pc("initOK");	
	
	while(1)
	{
		
		//testScrutation();
		//Traitement des messages
    traitement_message();
		//Ici on coderait l'algo de pilotage si mode 2 ou 3.


	}
	

}
	