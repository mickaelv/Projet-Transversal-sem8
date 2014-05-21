/**************
   servo.h    
 **************/

#ifndef SERVO_H
#define SERVO_H

#define Temps_angle_0 600
#define Tic_Par_Sec 256000
   
void initialisation_PCA ();
void GenerateurPulsation (int _usec );
void positionServo(int _angle);
void testScrutation(int angle);
void initialisation_Ports (void);

#endif
