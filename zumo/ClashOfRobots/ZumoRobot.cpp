#include <Wire.h>
#include <Zumo32U4.h>
#include "ZumoRobot.h"



ZumoRobot::ZumoRobot() {
  // Initialise les capteurs de lumminosité au sol
  lineSensors.initThreeSensors();
  // Initialise les capteurs de proximité
  proxSensors.initThreeSensors();
  pressedButton = 'Z';
}

void ZumoRobot::avancer(int vitesse) {
  motors.setSpeeds(vitesse, vitesse);
}

void ZumoRobot::foncer() {
  motors.setSpeeds(200, 200);  // A mettre à 400 si zone de combat assez grande
}

void ZumoRobot::reculer(int vitesse) {
  motors.setSpeeds(-vitesse, -vitesse);
}

void ZumoRobot::arreter() {
  motors.setSpeeds(0, 0);
}

void ZumoRobot::tournerDroite(int vitesse) {
  motors.setSpeeds(vitesse, -vitesse);
}

void ZumoRobot::tournerGauche(int vitesse) {
  motors.setSpeeds(-vitesse, vitesse);
}
/**
* Lis et sauvegarde les valeurs des capteurs de reflectance au sol
**/
void ZumoRobot::lireCapteursSol() {
  uint16_t sensorValues[3];

  lineSensors.read(sensorValues);

  solValeurGauche = sensorValues[0];
  solValeurCentre = sensorValues[1];
  solValeurDroite = sensorValues[2];
}
/**
* Fonction qui retourne true si le robot roule sur une surface claire
**/
bool ZumoRobot::detecteZoneClaire() {
  lireCapteursSol();

  return solValeurGauche < seuilZoneClaire || solValeurDroite < seuilZoneClaire || solValeurCentre < seuilZoneClaire;
}

/**
* Fonction qui retourne true si le robot roule sur une surface sombre
**/
bool ZumoRobot::detecteZoneSombre() {
  lireCapteursSol();
  // Tous les 3 capteurs doivent être dans la zone sombre pour retourner true
  return solValeurGauche > seuilZoneSombre && solValeurDroite > seuilZoneSombre && solValeurCentre > seuilZoneSombre;
}
/**
* Lis et sauvegarde les valeurs des capteurs de proximité
**/
void ZumoRobot::lireCapteursProx() {
  proxSensors.read();

  proxValeurFrontGauche = proxSensors.countsFrontWithLeftLeds();
  proxValeurFrontDroit = proxSensors.countsFrontWithRightLeds();
  proxValeurGauche = proxSensors.countsLeftWithLeftLeds();
  proxValeurDroite = proxSensors.countsRightWithRightLeds();
}

/**
* Fonction qui retourne true si l'adversaire se trouve à doite du robot
**/
bool ZumoRobot::detecteAdversaireSurLaDroite() {
  lireCapteursProx();

  return (proxValeurDroite > proxValeurGauche || proxValeurFrontDroit > proxValeurFrontGauche) && (proxValeurDroite > seuilDetectionAdversaire || proxValeurFrontDroit > seuilDetectionAdversaire);
}
/**
* Fonction qui retourne true si l'adversaire se trouve à gauche du robot
**/
bool ZumoRobot::detecteAdversaireSurLaGauche() {
  lireCapteursProx();
  return proxValeurGauche > proxValeurDroite + 1 || proxValeurFrontGauche > proxValeurFrontDroit + 1 && (proxValeurGauche > seuilDetectionAdversaire || proxValeurFrontGauche > seuilDetectionAdversaire);
}
bool ZumoRobot::detecteAdversaireSurLaDroiteFrontale() {
  lireCapteursProx();
  // Si le capteur de proximité frontale de droite déctecte une plus grande valeur
  return (proxValeurFrontDroit > proxValeurFrontGauche && proxValeurFrontDroit > seuilDetectionAdversaire);
}
bool ZumoRobot::detecteAdversaireSurLaGaucheFrontale() {
  lireCapteursProx();
  // Si le capteur de proximité frontale de droite déctecte une plus grande valeur
  return (proxValeurFrontGauche > proxValeurFrontDroit && proxValeurFrontGauche > seuilDetectionAdversaire);
}

/**
* Fonction qui retourne true si l'adversaire se trouve en face du robot
**/
bool ZumoRobot::detecteAdversaireEnFace() {
  lireCapteursProx();
  return proxValeurFrontGauche == proxValeurFrontDroit && proxValeurFrontGauche > seuilDetectionAdversaire;
}

/**
* Fonction qui cherche l'adversaire et retourne true si l'aversaire est détecté
**/
bool ZumoRobot::trouverAdversaire() {
  while (!detecteAdversaireSurLaDroite() && !detecteAdversaireSurLaGauche() && !detecteAdversaireEnFace()) {
    if (detecteZoneSombre()) {
      tournerDroite(200);
    }
    arreter();
  }
  return true;
}

/**
* Fonction qui affiche les valeurs des capteurs de reflectivité au sol
**/
void ZumoRobot::afficherValeursCapteursAuSol() {
  lireCapteursSol();

  Serial.print("Capteur Gauche: ");
  Serial.println(solValeurGauche);
  Serial.print("Capteurs centre: ");
  Serial.println(solValeurCentre);
  Serial.print("Capteur Droit: ");
  Serial.println(solValeurDroite);
  Serial.println("=======================================================");
}

/**
* Fonction qui affiche les valeurs des capteurs de proximité frontaux gauche et droite
**/
void ZumoRobot::afficherValeursCapteursProximite() {
  lireCapteursProx();

  Serial.print("Front Left sensor(proxValeurFrontGauche): ");
  Serial.println(proxValeurFrontGauche);
  Serial.print("Front Right sensor(proxValeurFrontDroit): ");
  Serial.println(proxValeurFrontDroit);
  Serial.print("Left sensor(proxValeurGauche): ");
  Serial.println(proxValeurGauche);
  Serial.print("Right sensor(proxValeurDroite): ");
  Serial.println(proxValeurDroite);
  Serial.println("=======================================================");
}

/**
*
**/
void ZumoRobot::combattre() {
  // Tant qu'on se trouve sur la zone sombre
  if (detecteZoneSombre()) {
    // chercher l'adversaire et le trouver
    if (trouverAdversaire()) {
      // Tant qu'il n'a pas l'adversaire en face
      while (!detecteAdversaireEnFace()) {
        // Si l'adversaire est à droite
        if (detecteAdversaireSurLaDroite()) {
          tournerDroite(200);
        }
        // Si l'adversaire est à gauche
        else if (detecteAdversaireSurLaGauche()) {
          tournerGauche(200);
        }
      }
      // Si il est en face
      foncer();
    }
  }
  else{
    if()
  }
  arreter();
}

/**
* Detection et suivis d'une ligne
**/
void ZumoRobot::suivreLigne() {
}

/**
* Stratégie de suivis d'objet
**/
void ZumoRobot::suivreObjet() {
  // Lire les capteurs de proximité
  lireCapteursProx();

  // Si l'objet est à droite, tourner à droite
  if (detecteAdversaireSurLaDroiteFrontale()) {
    tournerDroite(200);
    // Si il est à gauche, tourner à gauche
  } else if (detecteAdversaireSurLaGaucheFrontale()) {
    tournerGauche(200);
  }
  // Si il est en face, s'arrêter
  else if(detecteAdversaireEnFace()){
    arreter();
  }
  // Sinon tourner en rond pour détecter un objet
  else{
    tournerDroite(200);
  }
}

/**
* Stratégie évitement d'obstacles
**/
void ZumoRobot::evitementObstacles() {
  // Si l'adversaire est en face, tourner à droite
  if(detecteAdversaireEnFace()){
    if(proxValeurFrontGauche == proxValeurFrontDroit && proxValeurFrontDroit == 6){
      reculer(200);
      delay(1000);
    }
    tournerDroite(200);
  }
  // Si l'adversaire est à gauche, tourner à droite
  else if(detecteAdversaireSurLaGaucheFrontale()){
    tournerDroite(200);
  }
  // Si l'adversaire est à droite, tourner à gauche
  else if(detecteAdversaireSurLaDroiteFrontale()){
    tournerGauche(200);
  }
  // sinon avancer
  else{
    avancer(200);
  }

}

char ZumoRobot::getPressedButton() {
  return pressedButton;
}

void ZumoRobot::detecteBouttonappuye() {
  if (buttonA.getSingleDebouncedPress()) {
    pressedButton = 'A';
  } else if (buttonB.getSingleDebouncedPress()) {
    pressedButton = 'B';
  } else if (buttonC.getSingleDebouncedPress()) {
    pressedButton = 'C';
  }
}
