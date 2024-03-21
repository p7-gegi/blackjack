/* 
 * Auteurs: Jean-Samuel Lauzon    
 * Date: Fevrier 2022
*/

/*------------------------------ Librairies ---------------------------------*/
#include <Arduino.h>
#include <ArduinoJson.h>

/*------------------------------ Constantes ---------------------------------*/

#define BAUD 9600        // Frequence de transmission serielle

/*---------------------------- Variables globales ---------------------------*/

volatile bool shouldSend_ = false;  // Drapeau prêt à envoyer un message
volatile bool shouldRead_ = false;  // Drapeau prêt à lire un message

int ledState = 0;
int potValue = 0;
int buttonpin1 = 12; //bouton stand
int buttonpin = 13; //bouton hit
int buttonpins[2] = {6,8};
int potentiometre=A0;

int buttonAnalogue = A1;
int led_rouge = 43;
int pinLED = 7;
int pinPOT = A5;
int previousButtonState = 1;
int previousButtonState1 = 1;
int previouspotstate;
int buttonState;
int buttonState1;
int potstate=0;


/*------------------------- Prototypes de fonctions -------------------------*/
void sendMsg(int hit, int stand,int pot); 
void readMsg();
void serialEvent();
/*---------------------------- Fonctions "Main" -----------------------------*/

void setup() {
  Serial.begin(BAUD);               // Initialisation de la communication serielle
  pinMode(pinLED, OUTPUT);
  /*for (int i=0;i<2;i++)
    pinMode(buttonpins[i],INPUT);*/
  pinMode(potentiometre,INPUT);
  pinMode(buttonpin,INPUT);
  pinMode(buttonpin1,INPUT);
  pinMode(led_rouge, OUTPUT);
  digitalWrite(pinLED, ledState);
  previouspotstate=analogRead(potentiometre);
}

/* Boucle principale (infinie) */
void loop() {
  // Lecture de l'état actuel du bouton
  /*int buttonState = digitalRead(buttonpin);

  // Si l'état du bouton a changé et est à 0 (bouton pressé), mettre shouldSend_ à true
  if (buttonState != previousButtonState && buttonState == 0) {
    shouldSend_ = true;
  }

  // Si shouldRead_ est vrai, lire le message
  if (shouldRead_) {
    readMsg();
    shouldRead_ = false;
  }

  // Si shouldSend_ est vrai, envoyer le message
  if (shouldSend_) {
    sendMsg();
    shouldSend_ = false;
  }*/
  /*if(shouldRead_)
  {
    readMsg();
    sendMsg();
  }
  buttonState = digitalRead(buttonpin);*/
  // Lecture de l'état actuel du bouton
  if (analogRead(buttonAnalogue) > 340 && analogRead(buttonAnalogue) < 450)
  {
    buttonState = 1;
  }
  else
  {
    buttonState = 0;
  }

  int buttonState1 = digitalRead(buttonpin1);
  int potstate = analogRead(potentiometre);
  
  // Si l'état du bouton a changé
  if (buttonState != previousButtonState) {
    // Mettre à jour l'état précédent du bouton
    previousButtonState = buttonState;
    // Si l'état actuel du bouton est à 0 (bouton pressé), envoyer le message
    if (buttonState == 0) {
      buttonState1=1;
      sendMsg(0,1,0);
    }
  }
  if (buttonState1 != previousButtonState1) {
    // Mettre à jour l'état précédent du bouton
    previousButtonState1 = buttonState1;
    // Si l'état actuel du bouton est à 0 (bouton pressé), envoyer le message
    if (buttonState1 == 0) {
      buttonState=1;
      sendMsg(1,0,0);
    }
  }
  if(potstate<=previouspotstate-5 || potstate>=previouspotstate+5)
  {
    sendMsg(1,1,potstate);
    //Serial.print(potstate);
    previouspotstate=potstate;
  }

  // Lire le message si nécessaire
  if (shouldRead_) {
    readMsg();
    shouldRead_ = false;
  }
  // Autres actions dans la boucle loop()
  delay(400);//ne pas mettre à 50 ni 1000 car erreur de parsing
  // Autres actions dans la boucle loop()
}


/*---------------------------Definition de fonctions ------------------------*/

void serialEvent() { shouldRead_ = true; }


/*---------------------------Definition de fonctions ------------------------
Fonction d'envoi
Entrée : Aucun
Sortie : Aucun
Traitement : Envoi du message
-----------------------------------------------------------------------------*/
void sendMsg(int hit, int stand,int pot) { 
    // Création de l'objet JSON
    StaticJsonDocument<500> doc;
    
    // Ajout de l'état du bouton à l'objet JSON
    doc["hit"] = hit;
    doc["pot"] = pot;
    doc["stand"] = stand;
    //Serial.print("La valeur du bouton est : ");
    //Serial.println(buttonState);
    // Sérialisation de l'objet JSON
    serializeJson(doc, Serial);

    // Envoi du message
    Serial.println();

    // Mise à jour de l'état précédent du bouton
    previousButtonState = buttonState;
    previousButtonState1 = buttonState1;
  // Mise à jour de shouldSend_ à false pour ne pas envoyer de messages supplémentaires
  shouldSend_ = false;
}

/*---------------------------Definition de fonctions ------------------------
Fonction de reception
Entrée : Aucun
Sortie : Aucun
Traitement : Réception du message
-----------------------------------------------------------------------------*/
void readMsg(){
  // Lecture du message Json
  StaticJsonDocument<500> doc;
  JsonVariant parse_msg;

  // Lecture sur le port Seriel
  DeserializationError error = deserializeJson(doc, Serial);
  shouldRead_ = false;

  // Si erreur dans le message
  if (error) {
    Serial.print("deserialize() failed: ");
    Serial.println(error.c_str());
    return;
  }
  // Analyse des éléments du message message
  parse_msg = doc["lcd"];//message du LCD
  if (!parse_msg.isNull()) {
    // mettre la led a la valeur doc["led"]
    //digitalWrite(pinLED,doc["led"].as<bool>());//allumer une led selon le message JSON
  }
}