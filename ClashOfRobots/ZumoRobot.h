#ifndef ZUMOROBOT_H
#define ZUMOROBOT_H

#include <Zumo32U4.h>

class ZumoRobot {
private:
  // Déclaration des composants du robot
  Zumo32U4Motors motors;                 // Objet pour contrôler les moteurs
  Zumo32U4LCD ecran;                     // pour pouvoir utiliser les fonctions de l'écran LCD
  Zumo32U4ButtonA buttonA;               // pour pouvoir utiliser le button A
  Zumo32U4ButtonB buttonB;               // pour pouvoir utiliser le button B
  Zumo32U4ButtonC buttonC;               // pour pouvoir utiliser le button C
  Zumo32U4Buzzer buzzer;                 // pour pouvoir utiliser Buzzer
  Zumo32U4LineSensors lineSensors;       // pour pouvoir utiliser les fonctions de detection de ligne
  Zumo32U4ProximitySensors proxSensors;  // pour pouvoir utiliser les fonctions de detection d'objet à proximité

  // Valeurs des capteurs de proximité
  uint16_t proxValeurFrontGauche;
  uint16_t proxValeurFrontDroit;
  uint16_t proxValeurGauche;
  uint16_t proxValeurDroite;
  // Valeurs des capteur de "reflectance" au sol
  uint16_t solValeurGauche;
  uint16_t solValeurCentre;
  uint16_t solValeurDroite;

  // Constantes
  const uint16_t seuilZoneSombre = 1000;        // Seuil au delà duquel on considère que la zone sur laquellle le robot se trouve est sombre (Il faut que la valeur soit supérieur à ce nombre )
  const uint16_t seuilZoneClaire = 300;        // Seuil au delà duquel on considère que la zone sur laquellle le robot se trouve est claire (Il faut que la valeur soit inférieur à ce nombre)
  const uint16_t seuilDetectionAdversaire = 3;  // Seuil au dela duquel on considère que l'adversaire est détecté (La valeur doit être supérieur ou égale à ce nombre)

  // Variables
  char pressedButton; // dernier bouton appuyé

public:
  ZumoRobot();  // Constructeur

  void avancer(int vitesse);        // Méthode pour avancer tout droit
  void foncer();                    // Rouler à toute vitesse
  void reculer(int vitesse);        // Méthode pour reculer
  void arreter();                   // Méthode pour arrêter le robot
  void tournerDroite(int vitesse);  // Tourner à droite
  void tournerGauche(int vitesse);  // Tourner à gauche
  bool detecteZoneClaire();
  bool detecteZoneSombre();
  bool detecteAdversaireSurLaDroite();
  bool detecteAdversaireSurLaGauche();
  bool detecteAdversaireSurLaDroiteFrontale();
  bool detecteAdversaireSurLaGaucheFrontale();
  bool detecteAdversaireEnFace();
  bool trouverAdversaire();  // Cherche l'adversaire en tournant et avançant, si l'adversaire est en face il retourne true
  void afficherValeursCapteursAuSol();
  void afficherValeursCapteursProximite();
  void lireCapteursSol();
  void lireCapteursProx();
  void combattre();
  void suivreObjet();
  void evitementObstacles();
  void suivreLigne();
  void detecteBouttonappuye();
  char getPressedButton();
};

#endif
