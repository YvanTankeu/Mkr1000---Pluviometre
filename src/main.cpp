/*
  Titre      : Pluviometrie
  Auteur     : Crepin Fouelefack AND Yvan Tankeu
  Date       : 06/04/2022
  Description: Envoie le nombre de bascule du à une quantité de pluie provenant du auget
  Version    : 0.0.1
*/

//librairie pour le framework
#include <Arduino.h>

//Librairie de branchement
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h"

// Definition des pin
const int PinReedSwitch = 1; // Reedswitch pour le decompteur
volatile int Compteur = 0; // Compteur de basculement de l'auget

// Variables de gestions du temps
unsigned int tempsActuel = 0;
unsigned int tempsPasse = 0; 
unsigned int tempsNecessaire = 60000; // 1 min

//Interrupt pour compter le nombre de bascule
void compteurBascule()
{
  Compteur++;
}

void setup()
{
  //Branchement au reseau
  wifiConnect(); // Branchement au réseau WIFI
  MQTTConnect(); // Branchement au broker MQTT à Thingsboard

  Serial.begin(9600);
  //Definition des pinMode
  pinMode(PinReedSwitch, INPUT);

  // Lien interrupt pour compter les bascules à chaque changement d'etat du haut vers le bas
  attachInterrupt(digitalPinToInterrupt(PinReedSwitch), compteurBascule, FALLING);
}

void loop()
{
  // Sauvegarde du temps en cours
  tempsActuel = millis();

  //Envoie du decompte sur thingsboard  après chaque 01 min
  if ((tempsActuel - tempsPasse) >  tempsNecessaire){

    appendPayload("Compteur", Compteur);
    sendPayload();

    Compteur = 0;
    tempsPasse = tempsActuel;
  } 

}