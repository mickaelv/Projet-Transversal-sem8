/**************
    servo.c    
 **************/



#include <c8051f020.h>                    
#include "servo.h"
#include "mesure_distance.h"
#include "conversion_metre.h"
#include  "LIB_UART0.h"
#include  "LIB_UART1.h"
#include "VarGlobales_Define.h"
#include <stdio.h>

//------------------------------------------Fonctions d'initialisation------------------------------------------------------------------------------------
//********************************
//Fonction d'initialisation du PCA
//********************************
void initialisation_PCA ()
{
   PCA0MD=0x0;//;        // L'horloge de fonctionnement :sysclk, Activation de l'interruption sur CF
   EIE1 |= 0x08;    			//On active l'interruption du PCA (xxxx 1xxx)
   
   

    // Utilisation 16-Bit Pulse Width Modulator
    // (cf p 252-258 datasheet)
   
   
    PCA0CPM0 = 0xC2; // configure PWM (cf p 261)

    // (cf p 259)
    PCA0CN=0x00;      //clear des flags et disable le timer
}


//***********************************
//Fonction d'initialisation des ports
//***********************************
void initialisation_Ports (void)
{
   // Enable interrupt

   //P0MDOUT   |= 0xFF;                   // sortie push pull
	 P1MDOUT   |= 0x02;
   //XBR0      |= 0x14;                   //Dans cette configuration, le PCA est sur P0.2
   //XBR2      |= 0x40;
	    XBR0      |= 0x0E;
    XBR2      |= 0x44;

}





//-----------------------------------------------Fonctions d'utilisation-------------------------------------------------------------------------

//********************************************************************************************************
// Fonction qui transforme la position angulaire en une duréee en usec puis demande le pulse correspondant
//********************************************************************************************************
void positionServo(int angle)  
{
		int dureeOut=0; 
		EA=0;
    if (angle>=0 && angle<=180)                            // angle compris entre 0 et 180°
    {
        dureeOut=    Temps_angle_0 + angle*10;                           // position finale = position 0° + angle
    }

    else
    {
         dureeOut=Temps_angle_0;                                 // sinon poisitionnement à 0°, donc temps_angle_0
    }
    GenerateurPulsation(dureeOut); 
			EA=1;//  appel a la fonction de création du pulse avec la durée calculée
}

//*********************************************************
//Fonction qui génère le pulse de rapport cyclique souhaité
//*********************************************************
void GenerateurPulsation (int tpsusec )     // tpsusec varie de 600 à 24000
{
    // Sachant que l'horloge tourne à 22.1184 MHz
    // Une période représente 65535 coups
	
    int tps=65535-tpsusec*221184/10000/12;                  
                                                        
   
    char CPL,CPH;
    CPL=(char)tps;                        //On prend les 8 premiers bits de temp pour la valeur low de la borne de PCA0CP0
    CPH=(char)(tps >> 8);                //On décale ensuite de 8 bits pour prendre les 8 bits de poids fort (valeur high)



    PCA0L=0x00;
    PCA0H=0x00;               //on initialise le timer à 0 : on commence le duty cycle à 0, notre pulse sera en fin de periode
    PCA0CPL0=CPL;
    PCA0CPH0=CPH;             //on definit la valeur a atteindre du compteur 16 bits (celle calculé dans tmp)
   
    PCA0CN = 0x40;            //on active le timer PCA (x1xx xxxx) p 259

}


//-----------------------------------------------------Fonctions de Test---------------------------------------------------------------------------

//****************************
//Fonction de Test, scrutation
//****************************
void testScrutation(int angle)
{
    long int xdata i, j;
		int xdata n=0;
	//	double xdata Tab_distances_devant[40];
		//double xdata Tab_distances_derriere[40];

	
	
        for(j=0; j<181; j+=angle)
        {
            for(i=0; i<Tic_Par_Sec/100; i++); // 1 s
					  
						positionServo(j);
    
						Tab_distances_devant[n]=mesure_distance();
						AMX1SL    = 0x02;
						Tab_distances_derriere[n]=mesure_distance();
						AMX1SL = 0x00;
	


						n++;	 
        }
				positionServo(0);

	
}
