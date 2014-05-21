#include <c8051f020.h> 
#include "ADC1_Init.h"

void ADC1_Init (void)
{
   ADC1CN  = 0xA0;                     // bit7: ADC1 Enabled. ADC1 is active and ready for data conversions.
                                       // bit5: ADC1 has completed a data conversion
                                      
  																			
   ADC1CF= 0xF9;                        // Gain=1, fr�quence de conversion 2.5Mhz
	
   REF0CN = 0x03;                      // initialise la tension de ref�rence de l'ADC1 sur VREF
                                       // et autorisation du g�n�ratuer de bias pour l'ADC1
                                       
   AMX1SL = 0x00;                      // on choisit l'entr�e AIN 1.0 du mux   
   P1MDIN = 0xFA; 		    					   // on configure le port P1.0 en analogique
	

   EIE2 |= 0x04;                       // autorise les interruptions de l'ADC1

} 