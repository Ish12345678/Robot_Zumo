#include <Wire.h>
#include <Zumo32U4.h>
#include "ZumoRobot.h"

// Créer une instance du robot Zumo
ZumoRobot Tom;

void setup() {
  Serial.begin(9600);
  delay(3000);
}

void loop() {
  /*
  //Calibrage sol
  Tom.afficherValeursCapteursAuSol();
  delay(1000);
  */
  /*
  // Calibrage Proximite
  Tom.afficherValeursCapteursProximite();
  delay(1000);
  */

  
  Tom.detecteBouttonappuye();
  
  if(Tom.getPressedButton() == 'A'){
    Tom.combattre();
  }
  else if(Tom.getPressedButton() == 'B'){
    Tom.suivreObjet();
  }
  else if(Tom.getPressedButton() == 'C'){
    Tom.evitementObstacles();
  }
}
