/////////////////////////////////////////////////////////

//  Apple-1 6502/65C02 Single Stepper -- v1.2

//  by Claudio Parmigiani - P-LAB 2021
//
//  project homepage: https://p-l4b.github.io/A1-STEPPER/
//

//  inspired by the works of Ben Eater:
//  https://eater.net/
//  and Erturk Kocalar, 8Bitforce.com
//  http://www.8bitforce.com/

/////////////////////////////////////////////////////////

//  SELECT BUTTON: FREE or SINGLE STEP / AUTO selection
//  LEFT BUTTON: SINGLE STEP operation
//  RIGHT BUTTON: toggle between SINGLE STEP and AUTO mode
//  UP/DOWN BUTTONS: increase/decrease speed



const char ADDR[] = {52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};
const char DATA[] = {53, 51, 49, 47, 45, 43, 41, 39};
const int STEP_S[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
const char opcodeMatrix[256][5] = {\
                                   "BRK", "ORA", ""   , "", "TSB", "ORA", "ASL", "RMB0", "PHP", "ORA", "ASL", ""   , "TSB", "ORA", "ASL", "BBR0", \
                                   "BPL", "ORA", "ORA", "", "TRB", "ORA", "ASL", "RMB1", "CLC", "ORA", "INC", ""   , "TRB", "ORA", "ASL", "BBR1", \
                                   "JSR", "AND", ""   , "", "BIT", "AND", "ROL", "RMB2", "PLP", "AND", "ROL", ""   , "BIT", "AND", "ROL", "BBR2", \
                                   "BMI", "AND", "AND", "", "BIT", "AND", "ROL", "RMB3", "SEC", "AND", "DEC", ""   , "BIT", "AND", "ROL", "BBR3", \
                                   "RTI", "EOR", ""   , "", ""   , "EOR", "LSR", "RMB4", "PHA", "EOR", "LSR", ""   , "JMP", "EOR", "LSR", "BBR4", \
                                   "BVC", "EOR", "EOR", "", ""   , "EOR", "LSR", "RMB5", "CLI", "EOR", "PHY", ""   , ""   , "EOR", "LSR", "BBR5", \
                                   "RTS", "ADC", ""   , "", "STZ", "ADC", "ROR", "RMB6", "PLA", "ADC", "ROR", ""   , "JMP", "ADC", "ROR", "BBR6", \
                                   "BVS", "ADC", "ADC", "", "STZ", "ADC", "ROR", "RMB7", "SEI", "ADC", "PLY", ""   , "JMP", "ADC", "ROR", "BBR7", \
                                   "BRA", "STA", ""   , "", "STY", "STA", "STX", "SMB0", "DEY", "BIT", "TXA", ""   , "STY", "STA", "STX", "BBS0", \
                                   "BCC", "STA", "STA", "", "STY", "STA", "STX", "SMB1", "TYA", "STA", "TXS", ""   , "STZ", "STA", "STZ", "BBS1", \
                                   "LDY", "LDA", "LDX", "", "LDY", "LDA", "LDX", "SMB2", "TAY", "LDA", "TAX", ""   , "LDY", "LDA", "LDX", "BBS2", \
                                   "BCS", "LDA", "LDA", "", "LDY", "LDA", "LDX", "SMB3", "CLV", "LDA", "TSX", ""   , "LDY", "LDA", "LDX", "BBS3", \
                                   "CPY", "CMP", ""   , "", "CPY", "CMP", "DEC", "SMB4", "INY", "CMP", "DEX", "WAI", "CPY", "CMP", "DEC", "BBS4", \
                                   "BNE", "CMP", "CMP", "", ""   , "CMP", "DEC", "SMB5", "CLD", "CMP", "PHX", "STP", ""   , "CMP", "DEC", "BBS5", \
                                   "CPX", "SBC", ""   , "", "CPX", "SBC", "INC", "SMB6", "INX", "SBC", "NOP", ""   , "CPX", "SBC", "INC", "BBS6", \
                                   "BEQ", "SBC", "SBC", "", ""   , "SBC", "INC", "SMB7", "SED", "SBC", "PLX", ""   , ""   , "SBC", "INC", "BBS7"
                                  };


#define CLOCK 2
#define READ_WRITE 3
#define FREE_SINGLE 23
#define DIV_NC 25
#define DIV_NO 27
#define IRQ 29
#define NMI 31
#define SYN 35
#define S_I 37
#define RST A5
#define B1 A8
#define B2 A9
#define B3 A10
#define B4 A11
#define B5 A12
#define B6 A13
#define B7 A14
#define CLRSCR A15
#define STROBE A4

#include <LiquidCrystal.h>

#define LCD_RS  8
#define LCD_EN  9
#define LCD_D4  4
#define LCD_D5  5
#define LCD_D6  6
#define LCD_D7  7
#define LCD_BL  10
#define LCD_BTN  0
#define NUM_KEYS   5
#define BTN_DEBOUNCE 10
#define BTN_RIGHT  0
#define BTN_UP     1
#define BTN_DOWN   2
#define BTN_LEFT   3
#define BTN_SELECT 4

const int adc_key_val[NUM_KEYS] = { 30, 180, 360, 535, 760 };
int       key = -1;
int       oldkey = -1;
boolean   BTN_PRESS = 0;
boolean   BTN_RELEASE = 0;
boolean   CYCLE = 0;
boolean   DEV_STAT = 0;
//boolean   NMOS;
boolean   STEP_INSTR = 1; //1=SINGLE STEP
int   INTERACTIVE = -1;
int   INTERACTIVE_PREV = -1;
long AUTO_TIME;
int IDX = 1;
int syn;
int keySer, incomingByte;
int tilde=0;
int charDelay = 20;

unsigned long previousMillis = 0;

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
int backlightSet = 1023;

void setup() {
  for (int n = 0; n < 16; n += 1) {
    pinMode(ADDR[n], INPUT);
  }
  for (int n = 0; n < 8; n += 1) {
    pinMode(DATA[n], INPUT);
  }
  pinMode(CLOCK, INPUT);
  pinMode(IRQ, INPUT);
  pinMode(NMI, INPUT);
  pinMode(READ_WRITE, INPUT);
  pinMode(SYN, INPUT);
  pinMode(S_I, OUTPUT);
  digitalWrite(S_I, LOW);
  pinMode(RST, INPUT);             //RESET 6502, normally is set as INPUT
  pinMode(LCD_BL, OUTPUT);
  pinMode(FREE_SINGLE, OUTPUT);
  digitalWrite(FREE_SINGLE, LOW);   //FREE RUN
  pinMode(DIV_NC, OUTPUT);
  pinMode(DIV_NO, OUTPUT);
  diverterRelease();
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);
  pinMode(B4, OUTPUT);
  pinMode(B5, OUTPUT);
  pinMode(B6, OUTPUT);
  pinMode(B7, OUTPUT);
  pinMode(CLRSCR, INPUT);
  pinMode(STROBE, OUTPUT);
  digitalWrite(STROBE, LOW);
  RESET();
  Serial.begin(115200);
  MENU();
  lcd.begin(16, 2);
  analogWrite(LCD_BL, backlightSet);//FULL LIGHT
  lcd.setCursor(0, 0);
  lcd.print("P-LAB APPLE-1");
  lcd.setCursor(0, 2);
  lcd.print("6502 1-Stepper");
  attachInterrupt(digitalPinToInterrupt(CLOCK), onClock, RISING);
  displayRunMode();
  Serial.println("FREE RUN MODE SELECTED AT STARTUP");
}

void onClock() {
  int RW = digitalRead(READ_WRITE);
  int syn = digitalRead(SYN);
  int irq = digitalRead(IRQ);
  int nmi = digitalRead(NMI);

  char output[15];
  unsigned int address = 0;
  for (int n = 0; n < 16; n += 1) {
    int bit = digitalRead(ADDR[n]);
    Serial.print(bit);
    address = (address << 1) + bit;
  }
  Serial.print("  ");
  unsigned int data = 0;
  for (int n = 0; n < 8; n += 1) {
    int bit = digitalRead(DATA[n]);
    Serial.print(bit);
    data = (data << 1) + bit;
  }
  Serial.print("  ");
  //if (NMOS == 1) {
  //  RW = 1;
  //}
  if (syn == 1) {
    sprintf(output, "$%0.4X $%0.2X %c %4s %d%d", address, data, RW ? 'r' : 'W', opcodeMatrix[data], irq, nmi);
  }
  else {
    sprintf(output, "$%0.4X $%0.2X %c   %0.2X %d%d", address, data, RW ? 'r' : 'W', data, irq, nmi);
  }
  Serial.println(output);
  lcd.setCursor(0, 0);
  lcd.print(output);
}

void btn_Pressed_Select()
{
  lcdInterval();
  char tmp[7];
  lcd.setCursor(0, 2);
  if (digitalRead(FREE_SINGLE) == 1 ) {
    displayRunMode();
    Serial.println("FREE RUN MODE SELECTED");
    digitalWrite(FREE_SINGLE, LOW);
  }
  else {
    lcd.print("SINGLE ");
    Serial.println("SINGLE STEP MODE SELECTED");
    digitalWrite(FREE_SINGLE, HIGH);
    CYCLE = 0;
  }
}

void btn_Pressed_Left()
{
  if (digitalRead(FREE_SINGLE) == 1) {
    diverterActivate();
    delay(100);
    diverterRelease();
  }
}

void btn_Pressed_Right()
{
  if (digitalRead(FREE_SINGLE) == 1) {
    CYCLE = !CYCLE;
    lcd.setCursor(0, 2);
    if (CYCLE == 0) {
      lcd.print("SINGLE ");
      diverterRelease();
    }
    if (CYCLE == 1) {
      lcd.print("AUTO  ");
    }
  }
  else {              //select SINGLE STEP or SINGLE INSTRUCTION
    STEP_INSTR = !STEP_INSTR;
    displayRunMode();
  }
}

void btn_Pressed_Up()
{
  if (digitalRead(FREE_SINGLE) == 1) {
    IDX = IDX + 1;
    if (IDX > 27) {
      IDX = 27;
    }
    lcdInterval();
  }
}

void btn_Pressed_Down()
{
  if (digitalRead(FREE_SINGLE) == 1) {
    IDX = IDX - 1;
    if (IDX < 0) {
      IDX = 0;
    }
    lcdInterval();
  }
}

void displayRunMode() {
  lcd.setCursor(0, 2);
  if (STEP_INSTR == 1) {
    digitalWrite(S_I, HIGH);
    lcd.print("RUN -> 1-CYCLE  ");
    Serial.println("Single CYCLE mode selected");
  }
  else {
    digitalWrite(S_I, LOW);
    lcd.print("RUN -> 1-INSTR ");
    Serial.println("Single INSTRUCTION mode selected");
  }
}

void lcdInterval()
{
  AUTO_TIME = STEP_S[IDX];
  lcd.setCursor(7, 2);
  lcd.print(AUTO_TIME);
  lcd.print(" S/s    ");
}

void process_lcdkeypad()
{
  if ( getKey() ) {
    if ( BTN_PRESS ) {
      if (key == BTN_SELECT) btn_Pressed_Select();
      if (key == BTN_UP)     btn_Pressed_Up();
      if (key == BTN_DOWN)   btn_Pressed_Down();
      if (key == BTN_LEFT)   btn_Pressed_Left();
      if (key == BTN_RIGHT)  btn_Pressed_Right();
    }
  }
}

int getKey()
{
  key = get_key2();
  if (key != oldkey)
  {
    delay(BTN_DEBOUNCE);
    key = get_key2();
    if (key != oldkey) {
      oldkey = key;
      if (key == -1)
        BTN_RELEASE = 1;
      else
        BTN_PRESS = 1;
    }
  } else {
    BTN_PRESS = 0;
    BTN_RELEASE = 0;
  }
  return (key != -1);
}

int get_key2()
{
  int k;
  int adc_key_in;

  adc_key_in = analogRead( LCD_BTN );
  for ( k = 0; k < NUM_KEYS; k++ )
  {
    if ( adc_key_in < adc_key_val[k] )
    {
      return k;
    }
  }
  if ( k >= NUM_KEYS )
    k = -1;
  return k;
}

void diverterActivate() {
  //momentary diverter activated
  digitalWrite(DIV_NC, LOW);     //DIVERTER NC SICE: OPEN
  digitalWrite(DIV_NO, HIGH);    //DIVERTER NO SIDE: CLOSED
}

void diverterRelease() {
  //momentary diverter released
  digitalWrite(DIV_NO, LOW);     //DIVERTER NO SIDE: OPEN
  digitalWrite(DIV_NC, HIGH);    //DIVERTER NC SICE: CLOSED
}

void loop() {
  process_lcdkeypad();
  if (CYCLE == 1) {
    if (digitalRead(FREE_SINGLE) == 1) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= (500 / AUTO_TIME)) {
        previousMillis = currentMillis;
        if (DEV_STAT == LOW) {
          DEV_STAT = HIGH;
          diverterActivate();
        } else {
          DEV_STAT = LOW;
          diverterRelease();
        }
      }
    }
  }
  // READS COMMANDS FROM SERIAL USB
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    keySer = (incomingByte);
    if (keySer == 9 && (digitalRead(FREE_SINGLE) == 0)) {
      INTERACTIVE = -INTERACTIVE; //switch between Stepper command mode or direct typing on to Apple-1 (interactive mode) with TAB key
      if (INTERACTIVE == 1 && INTERACTIVE_PREV == -1) {
        Serial.println("NOW: Interactive Mode    ~~~ for programs");
        keySer=0;
      }
      if (INTERACTIVE == -1 && INTERACTIVE_PREV == 1) {
        Serial.println("NOW: Stepper Command Mode");
        keySer=0;
      }
    }
    if (keySer >= 97 && keySer <= 122) {
      keySer = keySer - 32; //force upper case
    }
    if (INTERACTIVE == 1 && tilde < 3 ) {     //trap ~~~ to go to Program Loading feature
      switch (keySer) {
        case 126:
          tilde = tilde + 1; //count ~
          break;
        default:
          tilde = 0;
          sendKey();
          break;
      }
    }
    if (tilde == 3) {
      //Serial.print("interactive ");Serial.println(INTERACTIVE);
      //Serial.print("tilde ");Serial.println(tilde);
      Serial.println();
      Serial.println("PROGRAM LOADING");
      Serial.println();
      Serial.println("Please choose one:");
      Serial.println();
      Serial.println("1   S. Wozniak ASCII Characters test");
      Serial.println("    - run with 0R");
      Serial.println();
      Serial.println("2   M. Willegal RAM test");
      Serial.println("    - set limits in zero page (i.e. 0: 0 E0 0 F0)");
      Serial.println("    - run with 280R and wait for PASS/error");
      Serial.println();
      tilde = 4;
      //Serial.print("interactive ");Serial.println(INTERACTIVE);
      //Serial.print("tilde ");Serial.println(tilde);
    }
    if (tilde >= 4) {
      keySer = keySer - 48;
      if (keySer < 3 && keySer > 0) { // only key 1 and 2 will work (ASCII 49 and 50)
        sendProgram();
      }
    }
    if (INTERACTIVE == -1) {
      parseSer();
    }
  }
  INTERACTIVE_PREV = INTERACTIVE;
}



void parseSer() {
  switch (keySer) {
    case 83: //S = toggle between Single Ops / Free Run
      btn_Pressed_Select();
      break;
    case 73: //I = toggle between Single Step / Single Instruction (Selection possible only in FREE RUN mode)
      if (digitalRead(FREE_SINGLE) == 0) {
        btn_Pressed_Right();
      }
      break;
    case 65: //A = toggle between Single Ops / AUTO MODE (Selection possible only in SINGLE OPS mode)
      if (digitalRead(FREE_SINGLE) == 1) {
        btn_Pressed_Right();
      }
      break;
    case 43: //+ increase AUTO speed
      btn_Pressed_Up();
      break;
    case 45: //- decrease AUTO speed
      btn_Pressed_Down();
      break;
    case 32: //SPACE Single Step/Operation Signal
      btn_Pressed_Left();
      break;
    case 33: //SPACE Single Step/Operation Signal
      RESET();
      break;
    case 63: //? recall main menu
      MENU();
      break;
    case 27: //ESC Clear Screen
      CLEARSCREEN();
      break;
    case 26: //? recall main menu
      RESET();
      break;
  }
}

void RESET() {
  Serial.println();
  Serial.print("Sending RESET signal to 6502...");
  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(500);
  pinMode(RST, INPUT);
  Serial.println(" done!");
}

void sendKey() {
  Serial.print(char(keySer));
  //RETURN must be remapped to 0x0D (13 DEC)
  if (keySer == 10) {
    keySer = 13;
  }
  if (keySer == 13) {
    Serial.println();
  }
  if (keySer == 27) {
    CLEARSCREEN();
  }
  if (keySer == 26) {
    RESET();
  }
  //data out
  digitalWrite(B1, keySer & (1 << 0));
  digitalWrite(B2, keySer & (1 << 1));
  digitalWrite(B3, keySer & (1 << 2));
  digitalWrite(B4, keySer & (1 << 3));
  digitalWrite(B5, keySer & (1 << 4));
  digitalWrite(B6, keySer & (1 << 5));
  digitalWrite(B7, keySer & (1 << 6));
  delay(1);
  //strobe
  digitalWrite(STROBE, HIGH);
  delay(5);
  digitalWrite(STROBE, LOW);
}

void MENU() {
  Serial.println("---------------------");
  Serial.println("P-LAB APPLE-1-STEPPER");
  Serial.println("---------------------");
  Serial.println();
  Serial.println("PAUSE/BREAK    Reset 6502");
  Serial.println("ESCAPE         Clear Apple-1 screen");
  Serial.println();
  Serial.println("? to recall this menu anytime");
  Serial.println();
  Serial.println("S Single Step mode / Free Run mode");
  Serial.println("I Single CYCLE / INSTRUCTION mode (in Free Run mode only)");
  Serial.println("A MANUAL/AUTO Single Step (in Single Step mode only)");
  Serial.println("+/- modify AUTO speed when applicable");
  Serial.println("SPACE send Single CYCLE/INSTRUCTION signal");
  Serial.println();
  Serial.println("TAB Interactive mode ON/OFF (in Free Run mode only)");
  Serial.println();
}

void CLEARSCREEN() {
  pinMode(CLRSCR, OUTPUT);
  digitalWrite(CLRSCR, HIGH);
  delay(30);
  pinMode(CLRSCR, INPUT);
  keySer = 0;
}

void sendProgram() {
  Serial.print(keySer);
  Serial.println(" will be loaded...");
  char PRG[2][586] =
  {
    "0000:A900AA20EFFFE88A4C0200 ",
    "0280:A9008507A900A8AA8506A5008504A5018505C004D004A5048506C005D004A5058506A5068104A104C506D02EE604D002E605A502C504D004A503C505D0D4A5008504A5018505C004D004A5048506C005D004A5058506A104C506D069E604D002E605A502C504D004A503C505D0D8C000D008A9FF8506C84C8A02C001D004A901D0F2C002D008060690EDA9FED0E6C003D008382606B0E0C8D0DDC004F0F9A950206C03A941206C03A953206C03A953206C03209C03E607A507207003209103EAEAEA4C84024898207003209C03A505207003A504207003209C03A506207003209C03682070032091034C00FF20EFFF60484A4A4A4A290F0930C93A90026906206C0368290F0930C93A90026906206C0360A90D206C03A90A206C0360A920206C0360 "
  };
  int IDX = keySer - 1;
  int lineDelimiter = -4;
  for (int i = 0; i <= sizeof(PRG[IDX]) - 1; i++) {
    keySer = PRG[IDX][i];
    if (keySer == 0) {      //detect EOT of char
      break;
    }
    if (keySer == 58) {     //detect ":" in order to count 8 bytes
      lineDelimiter == 0;
    }
    sendKey();
    lineDelimiter = lineDelimiter + 1;  //insert space every byte (2 characters), up to 8 byte per line
    switch (lineDelimiter) {
      case 3:
      case 5:
      case 7:
      case 9:
      case 11:
      case 13:
      case 15:        
        keySer = 32;                    //add SPACE every two characters
        delay(charDelay);
        sendKey();
        break;
      case 17:
        keySer = 13;                    //CR and begin a new line with :
        sendKey();
        delay(charDelay*7);
        keySer = 58;                    //send :
        sendKey();
        delay(charDelay);
        lineDelimiter = 1;
        break;
    }
    delay(charDelay);
  }
  keySer = 13;                          //final CR
  delay(charDelay);
  sendKey();
  tilde=0;
  Serial.println();
  Serial.println("Loading complete, Interactive Mode resumed");
  Serial.println();
}
