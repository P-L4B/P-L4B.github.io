/*

APPLE-1 TERMINAL
by Claudio Parmigiani - P-LAB

*/

#define SERIAL_TX_BUFFER_SIZE 256
#define SERIAL_RX_BUFFER_SIZE 256
#include <EEPROM.h>
int incomingByte = 0;
int bitToTransmit = 0;
int incomingCase = bitRead(EEPROM.read(0), 0);
int outgoingCase = bitRead(EEPROM.read(1), 0);

void setup()
{
  // latch lines (7 bit)
  pinMode(3, INPUT);  //LSB
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);  //MSB

  // CLRSCR
  pinMode(10, INPUT);

  // keyboard lines (B8 must be connected to +5V)
  pinMode(A0, OUTPUT);  //LSB B1
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(11, OUTPUT);  //MSB B7


  //STROBE
  pinMode(13, OUTPUT);

  //RESET
  pinMode(12, INPUT);
  
  //Set your terminal speed here. 230400 bps suits "FAST TERMINAL" (FTBASIC) patched INTEGER BASIC's maximum speed.
  Serial.begin(230400);

  //Start menu
  Serial.println();
  Serial.println("Apple-1 serial terminal by P-LAB");
  Serial.println("--------------------------------");
  Serial.println();
  Serial.println("CTRL-L ==> Clear Screen");
  Serial.println("CTRL-R ==> Reset");
  Serial.println("CTRL-O ==> Toggle:");
  displayOutgoingCase();
  Serial.println("CTRL-I ==> Toggle:");
  displayIncomingCase();
  Serial.println();
  
  //attach interrupt coming from address decode logic
  attachInterrupt(digitalPinToInterrupt(2), irq, FALLING);
}

void loop()
{
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    //Serial.println(incomingByte);

    //CTRL-I toggles incoming UPPERCASE/lowercase
    if ( incomingByte == 9 ) {
      setIncomingCase();
    }

    //CTRL-O toggles outgoing UPPERCASE/lowercase
    if ( incomingByte == 15 ) {
      setOutgoingCase();
    }

    sendChar();

  }
}


void irq()
{
  // read bits from latch
  byte charIn =
    (digitalRead(3) << 0) |
    (digitalRead(4) << 1) |
    (digitalRead(5) << 2) |
    (digitalRead(6) << 3) |
    (digitalRead(7) << 4) |
    (digitalRead(8) << 5) |
    (digitalRead(9) << 6);

  // CR --> CR LF
  if (charIn == 13) {
    Serial.write(13);
    Serial.write(10);
  }

  // printable chars only or escape for ANSI/VT100 screen/cursor control
  if (charIn > 31 || charIn == 27) { 

    if (incomingCase == 1) {
      if (charIn >= 97 && charIn <= 122) {
        charIn = charIn - 32;
      }
    }
    Serial.write(charIn);
  }
}


void sendChar() {

  //RETURN must be remapped to 0x0D (13 DEC)
  if (incomingByte == 19) {
    incomingByte = 13;
  }

  //CONTROL-L --> CLEAR SCREEN and put cursor in HOME position
  if (incomingByte == 12) {
    //CLEAR SCREEN
    Serial.write(27); //Print "esc"
    Serial.print("[2J");
    //HOME
    Serial.write(27); //Print "esc"
    Serial.print("[H");
    clrscr();
  }

  //CONTROL-R --> RESET
  if (incomingByte == 18) {
    reset();
  }

  if (outgoingCase == 1) {
    if (incomingByte >= 97 && incomingByte <= 122) {
      incomingByte = incomingByte - 32;
    }
  }
  
  //Serial.print("232: ");
  //Serial.println(incomingByte);

  bitToTransmit = ( incomingByte >> 0 ) & 1;
  digitalWrite(A0, bitToTransmit);
  bitToTransmit = ( incomingByte >> 1 ) & 1;
  digitalWrite(A1, bitToTransmit);
  bitToTransmit = ( incomingByte >> 2 ) & 1;
  digitalWrite(A2, bitToTransmit);
  bitToTransmit = ( incomingByte >> 3 ) & 1;
  digitalWrite(A3, bitToTransmit);
  bitToTransmit = ( incomingByte >> 4 ) & 1;
  digitalWrite(A4, bitToTransmit);
  bitToTransmit = ( incomingByte >> 5 ) & 1;
  digitalWrite(A5, bitToTransmit);
  bitToTransmit = ( incomingByte >> 6 ) & 1;
  digitalWrite(11, bitToTransmit);

  strobe();
}



void strobe() {
  digitalWrite(13, HIGH);
  delay(1);
  digitalWrite(13, LOW);
}

void reset() {
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(500);
  pinMode(12, INPUT);
}

void clrscr() {
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  delay(200);
  pinMode(10, INPUT);
}

void setIncomingCase() {
  incomingCase = !incomingCase;
  EEPROM.write(0, incomingCase);
  displayIncomingCase();
}

void displayIncomingCase() {
  incomingCase = bitRead(EEPROM.read(0), 0);
  Serial.print("Uppercase for INCOMING characters: ");
  if (incomingCase == 0) {
    Serial.println("not active");
  }
  else {
    Serial.println("ACTIVE");
  }
}

void setOutgoingCase() {
  outgoingCase = !outgoingCase;
  EEPROM.write(1, outgoingCase);
  displayOutgoingCase();
}

void displayOutgoingCase() {
  outgoingCase = bitRead(EEPROM.read(1), 0);
  Serial.print("Uppercase for OUTGOING characters: ");
  if (outgoingCase == 0) {
    Serial.println("not active");
  }
  else {
    Serial.println("ACTIVE");
  }
}
