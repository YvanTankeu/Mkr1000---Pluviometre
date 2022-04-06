/*
  Titre      : Pluviometrie
  Auteur     : Crepin Fouelefack AND Yvan Tankeu
  Date       : 06/04/2022
  Description: Envoie le nombre de bascule du à une quantité de pluie provenant du auget
  Version    : 0.0.1
*/

#include <Arduino.h>
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h"

const int PinReedSwitch = 1;
volatile int Compteur = 0;

int PreviousCounter = 0;

void compteurBascule()
{
  Compteur++;
}

void setup()
{

  wifiConnect(); // Branchement au réseau WIFI
  MQTTConnect(); // Branchement au broker MQTT

  Serial.begin(9600);
  pinMode(PinReedSwitch, INPUT);
  attachInterrupt(digitalPinToInterrupt(PinReedSwitch), compteurBascule, FALLING);
}

void loop()
{
  if (Compteur != PreviousCounter)
  {
    Serial.println(Compteur);
    PreviousCounter = Compteur;

    // envoie le décompte
    appendPayload("Compteur", Compteur);
    sendPayload();

    Compteur = 0;
  }

}