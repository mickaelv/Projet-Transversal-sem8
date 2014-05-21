#include <c8051f020.h> 
#include "conversion_metre.h"


double conversion_metre(int ADC1)
{
	double distance;
	double x;
	x=2.4*ADC1/256; //de la formule: ADC1=Vin*(Gain/Vref)*256
        
	// CONVERSION grâce à la doc technique			
        if(0.4<=x && x<= 0.61)
	{
		distance=-143*x+137;
		return distance;
	}	           
        if(0.61<x && x<=0.87)
	{
		distance=-58*x+85;            
		return distance;
	}
        if(0.87<x &&   x<= 1.15)
	{
		distance=-36*x+66;
		return distance;
	}
        if(1.15<x &&  x<= 1.67)
	{
		distance=-17*x+44;
		return distance;
	}
        if(1.67<x &&  x<=2.45)
	{
		distance=-8*x+29;
		return distance;
	}
        else return 0;
}
