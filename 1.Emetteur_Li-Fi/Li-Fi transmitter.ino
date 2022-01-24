

#include "AESLib.h"
#include <Keypad.h>
const byte ROW = 4;
const byte COL = 4;
AESLib aesLib;
char keyscode[ROW][COL] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPin[ROW] = {A5, A4, A3, A2};
byte colPin[COL] = {A1, A0, 12, 11};
Keypad customKeypad = Keypad( makeKeymap(keyscode), rowPin, colPin, ROW, COL);
char keycount = 0;
char code[5];

String plaintext = " ";

char cleartext[256];
char ciphertext[512];

// AES Encryption Key
byte aes_key[] = { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 };

// General initialization vector (you must use your own IV's in production for full security!!!)
byte aes_iv[N_BLOCK] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Generate IV (once)
void aes_init() {
  Serial.println("gen_iv()");
  aesLib.gen_iv(aes_iv);
  Serial.println("encrypt()");
  Serial.println(encrypt(strdup(plaintext.c_str()), plaintext.length(), aes_iv));
}

String encrypt(char * msg, uint16_t msgLen, byte iv[]) {
  int cipherlength = aesLib.get_cipher64_length(msgLen);
  char encrypted[cipherlength]; // AHA! needs to be large, 2x is not enough
  aesLib.encrypt64(msg, msgLen, encrypted, aes_key, sizeof(aes_key), iv);
  Serial.print("encrypted = "); Serial.println(encrypted);
  return String(encrypted);
}

String decrypt(char * msg, uint16_t msgLen, byte iv[]) {
  char decrypted[msgLen];
  aesLib.decrypt64(msg, msgLen, decrypted, aes_key, sizeof(aes_key), iv);
  return String(decrypted);
}

void setup() {
  
  Serial.begin(9600);
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  
  while (!Serial); // wait for serial port
  delay(2000);
  aes_init();
  aesLib.set_paddingmode(paddingMode::Array);



  char b64in[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  char b64out[base64_enc_len(sizeof(aes_iv))];
  base64_encode(b64out, b64in, 16);

  char b64enc[base64_enc_len(10)];
  base64_encode(b64enc, (char*) "0123456789", 10);

  char b64dec[ base64_dec_len(b64enc, sizeof(b64enc))];
  base64_decode(b64dec, b64enc, sizeof(b64enc));

  Serial.println("Enter text to be encrypted into console (no feedback) and press ENTER (newline):");
}


void wait(unsigned long milliseconds) {
  unsigned long timeout = millis() + milliseconds;
  while (millis() < timeout) {
    yield();
  }
}

unsigned long loopcount = 0;
byte enc_iv[N_BLOCK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // iv_block gets written to, provide own fresh copy...
byte dec_iv[N_BLOCK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


void loop() {

   char customKey = customKeypad.getKey();
  if (customKey) {
    Serial.println(customKey);
   if (customKey == '1')
  {
    digitalWrite(8,HIGH);
    delay(10);
    digitalWrite(8,LOW);
  }
  else if (customKey == '2')
  {
    digitalWrite(8,HIGH);
    delay(20);
    digitalWrite(8,LOW);
  }
  else if (customKey == '3')
  {
    digitalWrite(8,HIGH);
    delay(30);
    digitalWrite(8,LOW);
  }
  else if (customKey == '4')
  {
    digitalWrite(8,HIGH);
    delay(40);
    digitalWrite(8,LOW);
  }
  else if (customKey == '5')
  {
    digitalWrite(8,HIGH);
    delay(50);
    digitalWrite(8,LOW);
  }
  else if (customKey == '6')
  {
    digitalWrite(8,HIGH);
    delay(60);
    digitalWrite(8,LOW);
  }
  else if (customKey == '7')
  {
    digitalWrite(8,HIGH);
    delay(70);
    digitalWrite(8,LOW);
  }
  else if (customKey == '8')
  {
    digitalWrite(8,HIGH);
    delay(80);
    digitalWrite(8,LOW);
  }
  else if (customKey == '9')
  {
    digitalWrite(8,HIGH);
    delay(90);
    digitalWrite(8,LOW);
  }
  else if (customKey == '*')
  {
    digitalWrite(8,HIGH);
    delay(100);
    digitalWrite(8,LOW);
  }
  else if (customKey == '0')
  {
    digitalWrite(8,HIGH);
    delay(110);
    digitalWrite(8,LOW);
  }
  else if (customKey == '#')
  {
    digitalWrite(8,HIGH);
    delay(120);
    digitalWrite(8,LOW);
  }
  else if (customKey == 'A')
  {
    digitalWrite(8,HIGH);
    delay(130);
    digitalWrite(8,LOW);
  }
  else if (customKey == 'B')
  {
    digitalWrite(8,HIGH);
    delay(140);
    digitalWrite(8,LOW);
  }
  else if (customKey == 'C')
  {
    digitalWrite(8,HIGH);
    delay(150);
    digitalWrite(8,LOW);
  }
  else if (customKey == 'D')
  {
    digitalWrite(8,HIGH);
    delay(160);
    digitalWrite(8,LOW);
  }
  else;
  } 

  if (Serial.available()) {
    loopcount++; Serial.println(loopcount); // entry counter

    String readBuffer = Serial.readStringUntil('\n');
    Serial.println("INPUT:" + readBuffer);

    sprintf(cleartext, "%s", readBuffer.c_str()); // must not exceed 255 bytes; may contain a newline

    // Encrypt
    uint16_t clen = String(cleartext).length();
    String encrypted = encrypt(cleartext, clen, enc_iv);
    sprintf(ciphertext, "%s", encrypted.c_str());
    Serial.print("Ciphertext: ");
    Serial.println(encrypted);
    delay(1000);
    // Decrypt
    delay(1000);
    uint16_t dlen = encrypted.length();
    String decrypted = decrypt( ciphertext, dlen, dec_iv);
    Serial.print("Cleartext: ");
    Serial.println(decrypted);
    if (decrypted.equals(cleartext)) {
      Serial.println("SUCCES");
    }
    else
    {
      Serial.println("FAILURE");

    }
    for (int i = 0; i < 16; i++) {
      enc_iv[i] = 0;
      dec_iv[i] = 0;
    }
  }
}
