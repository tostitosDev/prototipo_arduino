/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

/*
 * In Arduino Leonardo
 * 5V (RED wire)
 * GND (BLACK wire)
 * pin #8 is IN from sensor (GREEN wire)
 * pin #9 is OUT from arduino  (YELLOW wire)
 */
 

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9); // (RX, TX)

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;
uint8_t opcion;

void setup(){
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\n Sistema de control de asistencia");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("¡Sensor encontrado!");
  } else {
    Serial.println("No hemos localizado el sensor.");
    while (1) { delay(1); }
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop(){ // run over and over again

  Serial.println("¡Listo para trabajar");
  Serial.println("1. Enrolar");
  Serial.println("2. Marcar");
    
  opcion = readnumber();

  //Aqui hacer switch para saber que a que funcion ir
  /*
  switch (opcion) {
    case 1:
      // Opcion Enrolar
      break;
    case 2:
      // Opcion Marcacion
      break;
    default:
      //Oopcion inválida
  }
  */
  
  Serial.println("¡Listo para enrrolar!");
  
  //Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  //id = readnumber();
  id = 1;
  //if (id == 0) {// ID #0 not allowed, try again!
  //   return;
  //}
  Serial.print("1. Enrolling ID #1");
  //Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll(){

  int p = -1;
  Serial.print("1. Esperando para enrrolar como #1"); 
  //Serial.println(id);
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("1. Imagen tomada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("1. .");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("1. Error de comunicacion");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("1. Error de imagen");
      break;
    default:
      Serial.println("1. Error desconocido");
      break;
    }
  }

  /*
  * Luego de que la imagen es tomada por primera vez, esta se pasa a convertir 
  */
  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("2. Imagen convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("2. Imagen confusa");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("2. Error de comunicacion");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("2. No se encontraron caracteristicas de huella");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("2. No se encontraron caracteristicas en la imagen");
      return p;
    default:
      Serial.println("2. Error desconocido");
      return p;
  }
  
  Serial.println("3. Remover dedo");
  
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
  //Serial.print("ID "); 
  //Serial.println(id);
  
  p = -1;
  Serial.println("4. Pon la misma huella de nuevo");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("4. Imagen tomada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print("4. .");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("4. Error de comunicacion");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("4. Error de imagen");
      break;
    default:
      Serial.println("4. Error desconocido");
      break;
    }
  }

  // OK success!
  /*
  * Luego de que la imagen es tomada por segunda vez, esta se pasa a convertir 
  */

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("5. Imagen convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("5. Imagen confusa");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("5. Error de comunicacion");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("5. No se encontraron caracteristicas de huella");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("5. No se encontraron caracteristicas en la imagen");
      return p;
    default:
      Serial.println("5. Error desconocido");
      return p;
  }
  
  // OK converted!
  Serial.print("6. Creando modelo para #1");  
  //Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("6. Coincidencia de patrones");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("6. Error de comunicacion");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("6. No hay coincidencia de patrones");
    return p;
  } else {
    Serial.println("6. Error desconocido");
    return p;
  }   
  
  //Serial.print("ID "); 
  //Serial.println(id);
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("7. ¡Almacenada!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("7. Error de comunicacion");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("7. No se puede almacenar la huella");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("7. Error de escritura");
    return p;
  } else {
    Serial.println("7. Error desconocido");
    return p;
  }   
}

/*
* Lectura de templates
*/

uint8_t downloadFingerprintTemplate(uint16_t id)
{
  Serial.println("------------------------------------");
  Serial.print("Intentando cargar #1"); 
  //Serial.println(id);
  uint8_t p = finger.loadModel(id);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("Template #"); 
      Serial.print(id); 
      Serial.println(" cargado");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicacion");
      return p;
    default:
      Serial.print("Error desconocido"); 
      Serial.println(p);
      return p;
  }

  // OK success!

  Serial.print("Intentando obtener #1 #"); 
  //Serial.println(id);
  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("Template "); 
      Serial.print(id); 
      Serial.println(" transferencia:");
      break;
   default:
      Serial.print("Error desconocido"); 
      Serial.println(p);
      return p;
  }
  
  // one data packet is 267 bytes. in one data packet, 11 bytes are 'usesless' :D
  uint8_t bytesReceived[534]; // 2 data packets
  memset(bytesReceived, 0xff, 534);

  uint32_t starttime = millis();
  int i = 0;
  while (i < 534 && (millis() - starttime) < 20000) {
      if (mySerial.available()) {
          bytesReceived[i++] = mySerial.read();
      }
  }
  Serial.print(i); 
  Serial.println(" bytes lectura.");
  Serial.println("Decodificando paquete...");

  uint8_t fingerTemplate[512]; // the real template
  memset(fingerTemplate, 0xff, 512);

  // filtering only the data packets
  int uindx = 9, index = 0;
  while (index < 534) {
      while (index < uindx) ++index;
      uindx += 256;
      while (index < uindx) {
          fingerTemplate[index++] = bytesReceived[index];
      }
      uindx += 2;
      while (index < uindx) ++index;
      uindx = index + 9;
  }
  for (int i = 0; i < 512; ++i) {
      //Serial.print("0x");
      printHex(fingerTemplate[i], 2);
      //Serial.print(", ");
  }
  Serial.println("\nhecho.");
}


void printHex(int num, int precision) {
    char tmp[16];
    char format[128];
 
    sprintf(format, "%%.%dX", precision);
 
    sprintf(tmp, format, num);
    Serial.print(tmp);
}
