// Maison connectée : version corrigée en 2026 (non testée sur le montage d'origine, qui n'existe plus).
// Même matériel : capteur Grove (thermistance B = 4275, R0 = 100 kΩ) sur A0, relais du chauffage sur D2,
// ventilateur sur D4, écran Grove LCD RGB. Corrections : hystérésis, défaut de sonde, boucle sans delay(), noms clairs.

#include <Wire.h>
#include "rgb_lcd.h"
#include <math.h>

rgb_lcd lcd;

const int PIN_CHAUFFAGE = 2;     // relais de la plaque chauffante (photos du montage)
const int PIN_VENTILATEUR = 4;
const int PIN_CAPTEUR = A0;
const float B = 4275.0;          // constante bêta de la thermistance
const float R0 = 100000.0;       // résistance à 25 °C

// hystérésis : le chauffage s'allume sous 25 °C et s'éteint au-dessus de 26 °C ; le ventilateur à 30 / 29 °C
const float CHAUFFE_ON = 25.0, CHAUFFE_OFF = 26.0, VENT_ON = 30.0, VENT_OFF = 29.0;
const unsigned long PERIODE_MS = 1000;

bool chauffage = false, ventilateur = false;
unsigned long dernier = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 255);
  pinMode(PIN_CHAUFFAGE, OUTPUT);
  pinMode(PIN_VENTILATEUR, OUTPUT);
  Serial.println(F("temperature;chauffage;ventilateur"));
}

void loop() {
  if (millis() - dernier < PERIODE_MS) return;   // pas de delay() : la boucle reste disponible
  dernier = millis();

  int a = analogRead(PIN_CAPTEUR);
  lcd.clear();                                    // évite les restes d'affichage
  if (a < 10 || a > 1013) {                       // sonde débranchée ou en court-circuit
    chauffage = false; ventilateur = false;
    digitalWrite(PIN_CHAUFFAGE, LOW); digitalWrite(PIN_VENTILATEUR, LOW);
    lcd.setRGB(255, 0, 0); lcd.print("DEFAUT SONDE");
    Serial.println(F("DEFAUT;0;0"));
    return;
  }
  lcd.setRGB(0, 0, 255);
  float R = R0 * (1023.0 / a - 1.0);
  float t = 1.0 / (log(R / R0) / B + 1.0 / 298.15) - 273.15;

  if (t < CHAUFFE_ON) chauffage = true; else if (t > CHAUFFE_OFF) chauffage = false;
  if (t > VENT_ON) ventilateur = true; else if (t < VENT_OFF) ventilateur = false;
  if (chauffage) ventilateur = false;             // jamais les deux à la fois

  digitalWrite(PIN_CHAUFFAGE, chauffage ? HIGH : LOW);
  digitalWrite(PIN_VENTILATEUR, ventilateur ? HIGH : LOW);

  lcd.print("Temp. "); lcd.print(t, 2); lcd.print(" C");
  lcd.setCursor(0, 1); lcd.print(chauffage ? "CHAUFFAGE" : ventilateur ? "VENTILATEUR" : "OK");
  Serial.print(t, 2); Serial.print(';'); Serial.print(chauffage); Serial.print(';'); Serial.println(ventilateur);
}
