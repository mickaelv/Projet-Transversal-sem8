#define SYSCLK 22118400 //approximate SYSCLK frequency in Hz
#define BAUDRATE  19200            // Baud rate of UART in bps
#define TICK_CLK  200  // (en hertz)
#define TAILLE_BUFFER 10

//Variables globales
extern char xdata message_recu_pc[TAILLE_BUFFER];
extern char xdata echo_actif;
extern char xdata robot_actif;
extern char *p_recu_pc;
extern char xdata message_a_traiter[TAILLE_BUFFER];
extern char xdata cycle_actif;
extern char xdata vitesse_avancer;
extern char xdata vitesse_reculer;
extern char xdata instructions[5][10];
extern double xdata  Tab_distances_devant[40];
extern double xdata  Tab_distances_derriere[40];

