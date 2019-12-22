/*
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>

const char* ssid = "Residencial ta cunha";
const char* password = "14105300";

const char* host = "192.168.0.22";

#define SS_PIN 21
#define RST_PIN 22
String mac;
String nfc;

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  mac = WiFi.macAddress();
  
  //Serial.println("Endereço MAC: ");
  //Serial.println(WiFi.macAddress());
  
  WiFi.begin(ssid,password);

  Serial.print("Iniciando a conexão com a rede ");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(". ");
  }
    delay(3000);
    Serial.println();
    Serial.println("Conexão estabelecida");
    Serial.println();
    Serial.println("Aguardando dados à serem lidos");

}
 
void loop() {
   
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Essa Tag não corresponde ao tipo 'MIFARE Classic' ."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
    rfid.uid.uidByte[1] != nuidPICC[1] ||
    rfid.uid.uidByte[2] != nuidPICC[2] ||
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println();
    Serial.print("Novo cartão detectado: ");
   

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    /*Serial.print(F("Tag em Hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("Em Dec"));*/
    
    printDec(rfid.uid.uidByte, rfid.uid.size);

    phpReturn();
   
  }
  else{
    Serial.println("Esse endereço foi lido recentemente, tente outro");
  } 
  

    // Halt PICC
    rfid.PICC_HaltA();
  
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();

}

void phpReturn(){
  WiFiClient client;

  const int httpPort = 80;
  if(!client.connect(host, httpPort)){
    Serial.println("estabelecendo conexão...");
    return;
  }
  
  //Criação da URL para solicitação
  String url = "/nodemcu/salvar.php?";
         url += "mac=";
         url += mac;
         url += "&nfc=";
         url += nfc;
         
  Serial.println();
  Serial.print("Requisitado para a URL:  ");
  Serial.println(url);
  Serial.println();

  //Envia a solicitação para o server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + 
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");

   //Confirmando conexão
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if(millis() - timeout > 5000){
        Serial.println(">>> Erro de conexão com o banco !");
        client.stop();
        return;  
      }  
    }
    
  //Confirma no terminal o resultado da inserção : 'Bem sucedido/mal sucedido'
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);  
    }

    Serial.println();
    Serial.println("Aguardando nova leitura");

    delay(1000);
}

void printDec(byte *buffer, byte bufferSize) {
 
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);

    //armazena na variável os dados do NFC contidos em 'buffer'
    nfc+=buffer[i];
  }
}





/*
void printHex(byte *buffer, byte bufferSize) {
 
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);

  }
}*/
