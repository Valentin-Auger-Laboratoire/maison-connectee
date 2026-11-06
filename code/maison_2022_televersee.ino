// Maison connectée, tâche 1 : régulation de la température (Tle STI2D SIN, novembre 2022)
// Programme TRANSCRIT depuis les captures de ma présentation (sketch_nov08a_trav, Arduino 1.8.12),
// version téléversée (seuils 26,1 / 29,9 °C). Recopié tel quel, commentaires compris (voir la vérification 2026 du README).

#include <Wire.h>
#include "rgb_lcd.h"
#include <math.h>

rgb_lcd lcd;

const int colorR = 0;             // constante couleur rouge =0 sur le lcd
const int colorG = 0;             // constante couleur gris =0 sur le lcd
const int colorB = 255;           // constante couleur bleu =255 sur le lcd
const int digPin = 2;             // Valeur "b" d'une thermistance(bêta) relation entre la résistance et la température d'une thermistance NTC.
const int B = 4275;               // B Valeur
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0

const int relayPin = 4 ;

void setup(){
    Serial.begin(9600);
    lcd.begin(16, 2);                              //Configurez le nombre de colonnes et de lignes de l'écran LCD
    lcd.setRGB(colorR, colorG, colorB);            //Afficher un message sur l'écran LCD.
    lcd.print("Temperature : ");
    delay(1000);
    Serial.begin(9600);                            // le débit de communication en nombre de caractères par seconde
    Serial.println(F("Initialize System"));        //Initialiser USB série
    pinMode(digPin, OUTPUT); // Init sortie
    pinMode(relayPin,OUTPUT);

}

void loop()
{
    // placer le curseur sur la colonne 0, ligne 1
    // (note : la ligne 1 est la deuxième ligne, puisque le comptage commence par 0) :
    lcd.setCursor(0, 1);
    // Lecture de la valeur sur la broche analogique A0
    int valeur = analogRead(A0);
    int a = analogRead(pinTempSensor);

    float R = 1023.0/a-1.0;
    R = R0*R;
    float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convertir en température via la fiche technique
    Serial.print("temperature = ");
    Serial.println(temperature); // créer la valeur température
    delay(100);
    lcd.print(temperature); // afficher la valeur températuresur le lcd
    delay(1000);

    if (temperature > 26.1 && temperature <= 29.9) // T supérieur 25 ,T inférieur ou égal 27
    {
    digitalWrite(relayPin,LOW); // le ventilateur s'arrête
    digitalWrite(digPin, LOW);  // le chauffage s'arrête
    delay(1000);
     }
     else
     if (temperature <= 26) // T Inferieur 25
     {
    digitalWrite(relayPin,LOW); // le ventilateur s'arrête
    digitalWrite(digPin, HIGH); // le ventilateur s'allume
    delay(1000);
     }
    else

    if (temperature > 30) // T superieur 27
    {
    digitalWrite(digPin, LOW); // le chauffage s'arrête
    digitalWrite(relayPin,HIGH); // le ventilateur s'allume
    delay(1000);
    }


  }
