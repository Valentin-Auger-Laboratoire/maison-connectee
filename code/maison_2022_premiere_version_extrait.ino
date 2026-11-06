// Maison connectée : première version de loop() (seuils 27 / 30 °C), TRANSCRITE depuis une capture de ma présentation.
// Seul le début est visible sur la capture : la suite n'est pas reproduite ici.

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

    if (temperature > 27 && temperature <= 30) // T supérieur 25 ,T inférieur ou égal 27
    {
    digitalWrite(relayPin,LOW); // le ventilateur s'arrête
    digitalWrite(digPin, LOW);  //  le chauffage s'arrête
    delay(1000);
     }
     else
     if (temperature <= 26) // T Inferieur 25
     {
    // ... (suite non visible sur la capture)
