//Fichier LIB_UART1.h  pour les fonctions relatives � l'ADC0 donc la communication avec le pc

//*****************************************************************************	 
//CFG_UART1()
//
//	 Configuration de l'UART0
// 
//*****************************************************************************


void CFG_UART1();


//*****************************************************************************	 
//putchar_serializer (char c)
//
//	 Envoyer un caract�re vers la carte serializer
// 
//*****************************************************************************


char putchar_serializer (char c);


//*****************************************************************************	 
//getchar_serializer()
//
//	 Fonction de reception d'un caractere de la carte serializer
// 
//*****************************************************************************

//char getchar_serializer();

//*****************************************************************************	 
//Send_String_serializer
//
//	 envoi d'une chaine de caract�re vers la carte serializer
// 
//*****************************************************************************

char Send_String_serializer(char *char_ptr);


//*****************************************************************************	 
//cfg_serializer
//
//	Configure la carte serializer avec des unit�s en metres et une vitesse de 19200
// 
//*****************************************************************************

void cfg_serializer();


//*****************************************************************************	 
//avancer
//
//	 Fait avancer le robot � une vitesse pass�e en parametre
// 
//*****************************************************************************

void avancer(int valeur);


 //*****************************************************************************	 
//rotation
//
//	 fait tourner le robot d'un angle pass� en parametre
// 
//*****************************************************************************

void rotation(int valeur);




void inter1();

void avancer_coord(int valeur);

void wait(int temps);