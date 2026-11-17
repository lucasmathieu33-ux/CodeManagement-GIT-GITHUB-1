// Step 5 - Capteur de force FSR

const uint8_t PIN_FSR = A0; // broche analogique du FSR

bool startLoop = false; // Il sert de "verrou" : tant qu'il est false, on considère que la boucle principale de doit pas démarrer
unsigned long time, startTime; // instant de référience en (en µs) au début de la boucle
char serialMessage[64]; // zone de texte de 64 caractères qui contient le message envoyé au PC

// Step 5 - convertir valeur brute en masse (g) par interpolation linéaire entre les points de calibrations
// (pas utilisé : pas de calibration)
// float brutVersMasse(int brut) {      
//   if (brut <= brutCalib[0]) return masseCalib[0];   // Step 5 - en dessous du 1er point
//   for (int i = 1; i < NB_POINTS; i++) {           // step 5 - on cherche l'intervalle   
//     if (brut <= brutCalib[i]) {
//       float t = (float)(brut - brutCalib[i-1]) / (brutCalib[i] - brutCalib[i-1]);
//       return masseCalib[i-1] + t * (masseCalib[i] - masseCalib[i-1]);     // Step 5 = interpolation
//     }
//   }
//   return masseCalib[NB_POINTS - 1];            // Step 5 - au dessus du dernier point
// }

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT); // -    Mettre le port LED_Builtin en sortie (OUTPUT) utiliser « Pin mode » 
  digitalWrite(LED_BUILTIN, LOW); // -    Mettre ce port à Low (Eteindre la led)
  Serial.begin (115200, SERIAL_8N1); // -    Démarrer / configurer le port série : Serial  
  
  while (startLoop == false) { // -    Atteindre réception caractère spécial du port série (boucle while) 
    if(Serial.available() > 0) { // Vérigier si un octet à été reçu sur le port série 
      int value = Serial.read();  // Lire l'octet reçu et le stocker dans "value"
      if(value == 114) startLoop = true; // Si le caractère reçu est 'r', sortir de la boucle d'attente
    }
  }
   startTime = micros() ; // Etape 3 - mémoriser l'isntant de départ du comptage temporel
}
void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(LED_BUILTIN, HIGH); // -    Allumer la Led_BUILTIN
  

  // Etape 3 - 
  time = micros() - startTime; // Etape 3 - calculter le temps écoulé depuis le édbut de la boucle 
  //Serial.println(time); // Etape 3 - afficher le temps écoulé sur le moniteur série 
  // delay(500); // Etape 3 - ralentir l'affichage
  
  // Etape 2, Nouveau : on surveille en permanance si un ordre de reset arrive 
  if (Serial.available() > 0) { // Vérifier en continu si un octet arrive sur le port série
    int value = Serial.read(); // Lire l'octet reçu et le sotcker dans 'value'
    if (value == 115) {               // par exemple 's' = 115, arrêt/reset
      NVIC_SystemReset();             // redémarrage logiciel de la carte
    }
  }

  // Step 5 - lecture du FSR
  int brut = analogRead(PIN_FSR);       // step 5 - valeur brute (0 à 1023)
  // long masse = (long)brutVersMasse(brut);    // valeur calibrée en g (pas utilisé)

  // sprintf(serialMessage, "%lu", time); // Etape 4 - écrire le temps sous forme de texte dans serialMessage
  sprintf(serialMessage, "%lu;%d", time, brut); // Step 5 - message : temps(µs);brut
  Serial.println(serialMessage) ; // Etape 4 - envoyer la valeur (sous forme de texte) au PC
  Serial.flush(); // etape 4 - attendre que la transmission soit bien terimné avant de continuer
}

