//Fichier LIB_UART0.h  pour les fonctions relatives à l'ADC0 donc la communication avec le pc

//*****************************************************************************	 
//CFG_UART0()
//
//	 Configuration de l'UART0
// 
//*****************************************************************************


void CFG_UART0();


//*****************************************************************************	 
//putchar_pc (char c)
//
//	 Envoyer un caractère vers le PC
// 
//*****************************************************************************


char putchar_pc (char c);


//*****************************************************************************	 
//getchar_pc()
//
//	 Fonction de reception d'un caractere 
// 
//*****************************************************************************

char getchar_pc();

//*****************************************************************************	 
//Send_String_pc
//
//	 envoi d'une chaine de caractère
// 
//*****************************************************************************

char Send_String_pc(char *char_ptr);


//*****************************************************************************	 
//reception_pc
//
//	 Interrupt a la reception d'un char
// 
//*****************************************************************************

void reception_pc();


//*****************************************************************************
//traitement_message
//
//	 traite le message de l'uart0
//
//*****************************************************************************


void traitement_message();

 //*****************************************************************************
//analyse_message
//
//	 recupère les chaines de caractères séparées par un separateur et les mets dans le tableau instructions
//	 ex: Petit papa noel =>  separateur=' ' => Petit
//											   papa
//											   noel
//
//*****************************************************************************

void analyse_message(char *chaine, char separateur);