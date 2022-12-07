// C64 keyboard to Apple-1 TTL/ASCII adapter
// by Claudio Parmigiani - P-LAB
// 
// Modified for C16 keyboard layout
// by skate323k137
// 
// Based on the following projects:
//
// C64 USB Keyboard mod
// Original by Pyofer
// See original thread @
// http://www.lemon64.com/forum/viewtopic.php?t=55650
//
// Modified to Support restore key & US/EU keymaps by dabone.
// Wiring is as follows
// 64 Keyboard connector has 20 Pins with a key Pin @ Pin 2.
// Arduino Pro Micro Pinout
// http://i.stack.imgur.com/oOtkU.png
//
// USE THIS SKETCH ONLY IF YOU'RE USING A C16 KEYBOARD
// C16 KEYBOARD MUST BE CONNECTED TO THE DESIGNATED APPLEDORE CONNECTOR
//

int inChar = 0;
int keyPos = 0;
int digitalread = 0;
int keyDown[72];
long lastDebounceTime[72];
int debounceDelay = 50;
int shift = 0;
int outPin = 2;
int outPinSet = 0;
int i;
int windowsShift;
int DefaultKBMode = 1;                            // Select 0 For Windows Mode On startup or 1 for C64 Mode
int USKeyboard = 1;                               // Select 1 for US Keyboard or 0 For EU
int sendingCharOut = 0;

char keyMapUS[216] = {

  212, 13, 215, 200, 194, 196, 198, 217, 0,        //  Del Return LR F7 F1 F3 F5 UD Null
  51, 119, 97, 52, 122, 115, 101, 129, 0,           //  3 W A 4 Z S E LSHFT Null
  53, 114, 100, 54, 99, 102, 116, 120, 0,           //  5 R D 6 C F T X Null
  55, 121, 103, 56, 98, 104, 117, 118, 0,           //  7 Y G 8 B H U V Null
  57, 105, 106, 48, 109, 107, 111, 110, 0,          //  9 I J Zero M K O N Null
  94, 112, 108, 64, 46, 58, 45, 44, 0,              //  + P L - . : @ , Null     (US Keyboard)
  35, 42, 59, 210, 133, 61, 211, 47, 205,           //  Pound * ; Home RSHFT = Pi / Restore
  49, 178, 128, 50, 32, 135, 113, 177, 0,           //  1 BS CTRL 2 SPC C= Q RunStop Null

  209, 13, 216, 201, 195, 197, 199, 218, 0,        //  Del Return LR F8 F2 F4 F6 UD Null
  35, 87, 65, 36, 90, 83, 69, 129, 0,               //  # W A $ Z S E LSHFT Null
  37, 82, 68, 38, 67, 70, 84, 88, 0,                //  % R D & C F T X Null
  39, 89, 71, 40, 66, 72, 85, 86, 0,                //  ' Y G ( B H U V Null
  41, 73, 74, 48, 77, 75, 79, 78, 0,                //  ) I J Zero M K O N Null
  94, 80, 76, 95, 62, 91, 96, 60, 0,                //  + P L - > : @ < Null
  35, 42, 93, 210, 133, 61, 214, 63, 205,           //  Pound * ; Home RSHFT = Pi ? Restore
  33, 178, 128, 34, 32, 135, 81, 177, 0,            //  ! BS CTRL " SPC C= Q RS Null     (US Keyboard)

  178, 13, 215, 200, 194, 196, 198, 217, 0,        //  Del Return LR F7 F1 F3 F5 UD Null
  51, 119, 97, 52, 122, 115, 101, 129, 0,           //  3 W A 4 Z S E LSHFT Null
  53, 114, 100, 54, 99, 102, 116, 120, 0,           //  5 R D 6 C F T X Null
  55, 121, 103, 56, 98, 104, 117, 118, 0,           //  7 Y G 8 B H U V Null
  57, 105, 106, 48, 109, 107, 111, 110, 0,          //  9 I J Zero M K O N Null
  45, 112, 108, 61, 46, 59, 91, 44, 0,              //  + P L - . : @ , Null
  209, 93, 39, 210, 133, 92, 212, 47, 205,          //  Pound * ; Home RSHFT = Pi / Restore
  49, 223, 9, 50, 32, 128, 113, 177, 0,             //  1 BS CTRL 2 SPC C= Q RunStop Null

};

void setup() {
  Serial.begin(9600);

  for (i = 0; i < 64; i++) keyDown[i] = 0; // Set all keys as up

  pinMode(2, OUTPUT); // configure inputs and outputs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(10, INPUT_PULLUP); // use internal pullups to hold pins high
  pinMode(11, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);

  digitalWrite(2, LOW); // start with one active pin to detect '1'
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);

  if (DefaultKBMode == 1)
  {
    if (!digitalRead(10)) windowsShift = 1; else windowsShift = 0; // detect if '1' is held on power up to swap mode
  }
  if (DefaultKBMode == 0)
  {
    if (!digitalRead(10)) windowsShift = 0; else windowsShift = 1; // detect if '1' is held on power up to swap mod
  }
}

void loop() // main keyboard scanning loop
{

  //serial read
  if (Serial.available() > 0) {
    // read the incoming byte:
    int incomingByte = Serial.read();
    keyDown[keyPos] = (incomingByte);
    outChar();
  }

  //keyscan & stuff
  for (outPin = 2; outPin < 10; outPin++) // scan through all rows
  {
    pinMode(2, INPUT); // set unused (all) outputs to input to avoid ghosting
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    pinMode(8, INPUT);
    pinMode(9, INPUT);

    // pinMode(outPin,OUTPUT);  // select output to activate
    // digitalWrite(outPin,LOW); // set it as low, a pressed key will be pulled to ground
    // Changed order to match real c64 keyboard matrix layout.

    if (outPin == 2) {
      pinMode (9, OUTPUT);
      digitalWrite(9, LOW);
      outPinSet = 9;
    }
    if (outPin == 3) {
      pinMode (3, OUTPUT);
      digitalWrite(3, LOW);
      outPinSet = 3;
    }
    if (outPin == 4) {
      pinMode (4, OUTPUT);
      digitalWrite(4, LOW);
      outPinSet = 4;
    }
    if (outPin == 5) {
      pinMode (5, OUTPUT);
      digitalWrite(5, LOW);
      outPinSet = 5;
    }
    if (outPin == 6) {
      pinMode (6, OUTPUT);
      digitalWrite(6, LOW);
      outPinSet = 6;
    }
    if (outPin == 7) {
      pinMode (7, OUTPUT);
      digitalWrite(7, LOW);
      outPinSet = 7;
    }
    if (outPin == 8) {
      pinMode (8, OUTPUT);
      digitalWrite(8, LOW);
      outPinSet = 8;
    }
    if (outPin == 9) {
      pinMode (2, OUTPUT);
      digitalWrite(2, LOW);
      outPinSet = 2;
    }


    for (i = 0; i < 9; i++) // scan through columns
    {
      keyPos = i + ((outPin - 2) * 9); // calculate character map position
      if (USKeyboard == 1)
      {
        if (!windowsShift) inChar = keyMapUS[keyPos + shift]; // work out which key it is from the map and shift if needed
        else inChar = keyMapUS[keyPos + 144]; // use "windows" keymap where shift is passed through
      }

      if (i == 0) digitalread = 1 - digitalRead(10); // check the active input pin
      if (i == 1) digitalread = 1 - digitalRead(11);
      if (i == 2) digitalread = 1 - digitalRead(A0);
      if (i == 3) digitalread = 1 - digitalRead(A5);
      if (i == 4) digitalread = 1 - digitalRead(A2);
      if (i == 5) digitalread = 1 - digitalRead(A3);
      if (i == 6) digitalread = 1 - digitalRead(A4);
      if (i == 7) digitalread = 1 - digitalRead(A1);

      if ((millis() - lastDebounceTime[keyPos]) > debounceDelay) // debounce for each key individually
      {
        if (digitalread == 1 && keyDown[keyPos] == 0) // if a key is pressed and wasn't already down
        {
          keyDown[keyPos] = inChar;      // put the right character in the keydown array
          if ((keyPos != 16 && keyPos != 58) || windowsShift == 1 ) // is it not-shift or in windows mode?
          { // if so pass the key through
            lastDebounceTime[keyPos] = millis(); // reset the debounce delay
            //Serial.println(char(keyDown[keyPos]));    // out to serial port for debug
            outChar();
          }
          else {
            lastDebounceTime[keyPos] = millis();  // reset keybounce delay and mark as shift press
            shift = 72;
          }
        }
        if (digitalread == 0 && keyDown[keyPos] != 0) // key is up and a character is stored in the keydown position
        {
          if ((keyPos != 16 && keyPos != 58) || windowsShift == 1) // not-shift or windows mode
          {
            lastDebounceTime[keyPos] = millis();  // reset keybounce delay
            //Keyboard.release(keyDown[keyPos]);    // pass key release to windows
          }
          else {
            lastDebounceTime[keyPos] = millis();  // reset keybounce delay and mark as un-shifted
            shift = 0;
          }
          keyDown[keyPos] = 0; // set keydown array position as up
        }
      }
    }
    digitalWrite(outPinSet, HIGH); // set output back to high
  }

}

void outChar() {

  //CLRSCR
  if ((keyDown[keyPos]) == -78)
  {
    pinMode (12, OUTPUT);
    digitalWrite(12, HIGH);
    delay(200);
    pinMode(12, INPUT);
  }

  //CRTL mapped as ESC
  if ((keyDown[keyPos]) == -128)
  {
    keyDown[keyPos] = 27;
  }

  //INST/DEL remapped as "_"
  if ((keyDown[keyPos]) == -44)
  {
    keyDown[keyPos] = 95;
  }

  //C16 + key fix (Dirty?)
  if ((keyDown[keyPos]) == -45)
  {
    keyDown[keyPos] = 43;
  }
  
  //ARROW UP remapped as "^" // THIS IS ACTUALLY MINUS ON A C16?
  if ((keyDown[keyPos]) == 64)
  {
    keyDown[keyPos] = 94;
  }

  //GBP remapped as "#"
  if ((keyDown[keyPos]) == -41)
  {
    keyDown[keyPos] = 35;
  }
  
  //@ key C16
  if ((keyDown[keyPos]) == -39)
  {
    keyDown[keyPos] = 64;
  }
  
  if (keyDown[keyPos] > 0 && keyDown[keyPos] <= 127) {
    //if (keyDown[keyPos] != 0 ) {


    //CR
    if ((keyDown[keyPos]) == 19) {
      keyDown[keyPos] = 13;
    }

    //to uppercase
    if (keyDown[keyPos] >= 97 && keyDown[keyPos] <= 122) {
      keyDown[keyPos] = keyDown[keyPos] - 32;
    }

    Serial.print(char(keyDown[keyPos]));
    //Serial.println(keyDown[keyPos]);

    /*pinMode(9, OUTPUT);
      pinMode(8, OUTPUT);
      pinMode(7, OUTPUT);
      pinMode(6, OUTPUT);
      pinMode(5, OUTPUT);
      pinMode(4, OUTPUT);
      pinMode(3, OUTPUT);
      pinMode(2, OUTPUT);*/

    for (int i = 6; i >= 0; i--) {
      int bitToCheck = 1 << i;
      boolean bitToTransmit = keyDown[keyPos] & bitToCheck;
      pinMode(i + 2, OUTPUT);
      digitalWrite((i + 2), bitToTransmit);
    }

    //forcing highest bit to 1 as required by Apple-1
    digitalWrite(9, HIGH);
    delayMicroseconds(100);

    //STROBE OUT
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    delayMicroseconds(100);
    digitalWrite(13, LOW);

    //resetting lines
    pinMode(9, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
  }
}
