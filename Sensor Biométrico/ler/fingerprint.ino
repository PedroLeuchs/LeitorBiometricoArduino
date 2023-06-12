#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

int getFingerprintIDez();

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Mapeamento de IDs para nomes
const char* nomes[] = {
  "Nome Desconhecido",
  "Nome Desconhecido",
  "Renan",
  "Pedro",
  "victor H. N.",
  "Luiz G",
  "Octavio",
  "Luan",
  "João",
  "Arthur",
  "Daniel",
  "Rafael",
  "Hiury",
  "Jefferson",
  "Davi",
  "Luiz",
  "Lucas",
  "Vitor",
  "Alan",
  "Gustavo Jesus ",
  "Gabriel",
  "Eduardo",
  "Alan Valentina",
  "Samuel",
  "Furini",
  "Jorge",
  "Djonathan",
  "Ismael",
  "Mayumi",
  "Edvaldo"
};

void setup()  {  
  Serial.begin(9600);
  Serial.println("Iniciando Leitor Biometrico");
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Leitor Biometrico Encontrado");
  } else {
    Serial.println("Leitor Biometrico nao encontrada");
    while (1);
  }
  Serial.println("Esperando Dedo para Verificar");
}

void loop()                   
{
  int id = getFingerprintIDez();
  if (id != -1) {
    const char* nome = getNomeByID(id);
    Serial.print("Nome: ");
    Serial.println(nome);
  }
  digitalWrite(12, HIGH);
  delay(50);          
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem Capturada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Dedo nao Localizado");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro ao se comunicar");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Erro ao Capturar");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem Convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem muito confusa");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro ao se comunicar");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Impossivel localizar Digital");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Impossivel Localizar Digital");
      return p;
    default:
      Serial.println("Erro Desconhecido");
      return p;
  }
  
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Digital Encontrada");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro ao se comunicar");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Digital Desconhecida");
    return p;
  } else {
    Serial.println("Erro Desconhecido");
    return p;
  }   
  
  Serial.print("ID # Encontrado"); 
  Serial.print(finger.fingerID); 
  Serial.print(" com precisao de "); 
  Serial.println(finger.confidence); 
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  delay(1000);
  digitalWrite(11, LOW);
  delay(1000);
  digitalWrite(12, HIGH);
  Serial.print("ID # Encontrado "); 
  Serial.print(finger.fingerID); 
  Serial.print(" com precisao de "); 
  Serial.println(finger.confidence);
  return finger.fingerID; 
}
const char* getNomeByID(int id) {
  if (id >= 2 && id <= 29) {
    return nomes[id];
  }
  return "Nome Desconhecido";
}
