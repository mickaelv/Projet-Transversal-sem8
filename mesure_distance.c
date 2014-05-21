#include <c8051f020.h> 
#include "conversion_metre.h"
#include "mesure_distance.h"
#include "VarGlobales_Define.h"

double mesure_distance(void)
{
	double distance,distance_reelle;
	int valeur_ADC;
	
	int a = ADC1CN & 0x10; //pour "selectionner" le bit AD1BUSY et le tester ensuite
	int b;
	
	
	
	if( a == 0x00) //on verifie qu'une conversion n'est pas en cours (AD1BUSY diff�rent de 1)
	{
		ADC1CN&=0xDF; //on met AD1INT � 0
		ADC1CN|=0x80;  // mise sous tension
		ADC1CN|=0x10; // AD1BUSY � 1: on d�clenche la conversion
							 
		//ADC1CN|=0x20;  		//Pour tester en "simulation" on met le bit AD1INT � 1
	 
		b = ADC1CN | 0xDF; // on "r�cup�re" le bit AD1INT 
			
	
		
		while(b==0x00); // on attend tant que ADINT=0
		
		valeur_ADC=ADC1; //on lit la valeur � la sortie de l'ADC1
			
		distance=conversion_metre(valeur_ADC); //on effectue la conversion
			
		ADC1CN |=0x20; //on met AD1BUSY � 0 pour indiquer que la conversion est termin�e
		
		distance_reelle=2*distance;  // �talonnage du t�l�m�tre
		
		return distance_reelle; //on retourne la distance convertie en metre
	}
	else return 0;
	
}
