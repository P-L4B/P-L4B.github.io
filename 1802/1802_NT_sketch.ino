/*

   TMSI ElectroniKit's 1802 Nametag programmer
   by Claudio Parmigiani - 2019 ---   v1.0

   based on http://www.sunrise-ev.com/membershipcard.htm

   Please refer to the schematic for the additional circuitry.

*/
//declaration of pins:
int ProgRun = 10; // Program/Run Relais
int FetchEx = 11; // Fetch/Execute Relais
int DataPin [] = {0, 2, 3, 4, 5, 6, 7, 8, 9};
//declaration of vars
String Frame[] = {"", "", "", "", ""};
String incomingString = "";
String tailString = "";
String userChar = "";
int A, B, C, D, E, F, G = 0;
int segVal[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int frameNumber = 0;

void setup() {
  setDataBusInput();
  pinMode(ProgRun, OUTPUT);
  pinMode(FetchEx, OUTPUT);
  Serial.begin(9600);
  Serial.println(F("TMSI ElectroniKit's 1802 Nametag programmer"));
  Serial.println(F("-------------------------------------------"));
  Serial.println();
  Run();
  Serial.println();
  Serial.println();
  for (int i = 1; i <= 4; i++) {
    Serial.print(F("Please type in Frame["));
    Serial.print(i);
    Serial.println(F("]"));
    readIncomingString();
    Frame[i] = incomingString;
    Serial.println();
    Serial.println(Frame[i]);
  }
  showFrames();
  for (frameNumber = 1; frameNumber < 5; frameNumber++) {
    program1802();
  }
}

void loop() {
}

void readIncomingString() {
  while (true) {
    if (Serial.available() > 0) {
      // read incoming serial data:
      incomingString = Serial.readString();
      break;
    }
  }
}

void showFrames() {
  for (int i = 1; i <= 4; i++) {
    Serial.print(F("Frame["));
    Serial.print(i);
    Serial.print(F("]="));
    Serial.print(Frame[i]);
    Serial.print(F(" is "));
    int frameLen = Frame[i].length();
    Serial.print(frameLen);
    Serial.print(F(" characters long."));
    if (frameLen < 8) {
      tailString = "";
      int tail = 8 - frameLen;
      for (int j = 1; j <= tail; j++) {
        tailString = tailString + "#";
      }
      Frame[i] = Frame[i] + tailString;
      Serial.print(F(" Normalized to: "));
      Serial.print(Frame[i]);
    }
    else if (frameLen > 8) {
      Serial.print(F(" Frame will be truncated."));
    }
    else {
      Serial.print(F(" Frame is OK."));
    }
    Serial.println();
    //Serial.print(
  }
}

void computeValues() {
  int temp = 0; //AT = 0; BT = 0; CT = 0; DT = 0, ET = 0; FT = 0; GT = 0;
  for (int i = 0; i <= 7; i++) {
    segVal[i] = 0;      //zeroing
  }
  //extract and compute value for segments A..F from Frame[frameNumber] from LSB to MSB
  for (int i = 0; i <= 7; i++) {
    userChar = Frame[frameNumber].substring(i, i + 1);
    Serial.print(userChar);
    charLookup();
    temp = ((A * (round(pow(2, (7 - i))))));
    segVal[1] = segVal[1] + temp;
    temp = ((B * (round(pow(2, (7 - i))))));
    segVal[2] = segVal[2] + temp;
    temp = ((C * (round(pow(2, (7 - i))))));
    segVal[3] = segVal[3] + temp;
    temp = ((D * (round(pow(2, (7 - i))))));
    segVal[4] = segVal[4] + temp;
    temp = ((E * (round(pow(2, (7 - i))))));
    segVal[5] = segVal[5] + temp;
    temp = ((F * (round(pow(2, (7 - i))))));
    segVal[6] = segVal[6] + temp;
    temp = ((G * (round(pow(2, (7 - i))))));
    segVal[7] = segVal[7] + temp;
  }
  Serial.print(" --> ");
  for (int i = 1; i <= 7; i++) {
    Serial.print(segVal[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void charLookup() {
  switch (toupper(userChar[0])) {
    case 'A': A = 1, B = 1, C = 1, D = 0, E = 1, F = 1, G = 1; break;
    case 'B': A = 0, B = 0, C = 1, D = 1, E = 1, F = 1, G = 1; break;
    case 'C': A = 1, B = 0, C = 0, D = 1, E = 1, F = 1, G = 0; break;
    case 'D': A = 0, B = 1, C = 1, D = 1, E = 1, F = 0, G = 1; break;
    case 'E': A = 1, B = 0, C = 0, D = 1, E = 1, F = 1, G = 1; break;
    case 'F': A = 1, B = 0, C = 0, D = 0, E = 1, F = 1, G = 1; break;
    case 'G': A = 1, B = 0, C = 1, D = 1, E = 1, F = 1, G = 0; break;
    case 'H': A = 0, B = 1, C = 1, D = 0, E = 1, F = 1, G = 1; break;
    case 'I': A = 0, B = 0, C = 0, D = 0, E = 1, F = 1, G = 0; break;
    case 'J': A = 0, B = 1, C = 1, D = 1, E = 1, F = 0, G = 0; break;
    case 'K': A = 1, B = 0, C = 1, D = 0, E = 1, F = 1, G = 1; break;
    case 'L': A = 0, B = 0, C = 0, D = 1, E = 1, F = 1, G = 0; break;
    case 'M': A = 1, B = 1, C = 1, D = 0, E = 1, F = 1, G = 0; break;
    case 'N': A = 0, B = 0, C = 1, D = 0, E = 1, F = 0, G = 1; break;
    case 'O': A = 0, B = 0, C = 1, D = 1, E = 1, F = 0, G = 1; break;
    case 'P': A = 1, B = 1, C = 0, D = 0, E = 1, F = 1, G = 1; break;
    case 'Q': A = 1, B = 1, C = 1, D = 0, E = 0, F = 1, G = 1; break;
    case 'R': A = 0, B = 0, C = 0, D = 0, E = 1, F = 0, G = 1; break;
    case 'S': A = 1, B = 0, C = 1, D = 1, E = 0, F = 1, G = 1; break;
    case 'T': A = 0, B = 0, C = 0, D = 1, E = 1, F = 1, G = 1; break;
    case 'U': A = 0, B = 0, C = 1, D = 1, E = 1, F = 0, G = 0; break;
    case 'V': A = 0, B = 1, C = 1, D = 1, E = 1, F = 1, G = 0; break;
    case 'W': A = 0, B = 1, C = 1, D = 1, E = 1, F = 1, G = 1; break;
    case 'X': A = 0, B = 1, C = 1, D = 0, E = 1, F = 1, G = 1; break;
    case 'Y': A = 0, B = 1, C = 1, D = 1, E = 0, F = 1, G = 1; break;
    case 'Z': A = 1, B = 1, C = 0, D = 1, E = 1, F = 0, G = 1; break;

    case ' ': A = 0, B = 0, C = 0, D = 0, E = 0, F = 0, G = 0; break;
    case '-': A = 0, B = 0, C = 0, D = 0, E = 0, F = 0, G = 1; break;
    case '=': A = 0, B = 0, C = 0, D = 1, E = 0, F = 0, G = 1; break;

    case '1': A = 0, B = 1, C = 1, D = 0, E = 0, F = 0, G = 0; break;
    case '2': A = 1, B = 1, C = 0, D = 1, E = 1, F = 0, G = 1; break;
    case '3': A = 1, B = 1, C = 1, D = 1, E = 0, F = 0, G = 1; break;
    case '4': A = 0, B = 1, C = 1, D = 0, E = 0, F = 1, G = 1; break;
    case '5': A = 1, B = 0, C = 1, D = 1, E = 0, F = 1, G = 1; break;
    case '6': A = 1, B = 0, C = 1, D = 1, E = 1, F = 1, G = 1; break;
    case '7': A = 1, B = 1, C = 1, D = 0, E = 0, F = 0, G = 0; break;
    case '8': A = 1, B = 1, C = 1, D = 1, E = 1, F = 1, G = 1; break;
    case '9': A = 1, B = 1, C = 1, D = 1, E = 0, F = 1, G = 1; break;
    case '0': A = 1, B = 1, C = 1, D = 1, E = 1, F = 1, G = 0; break;
  }
}

void program1802() {
  for (frameNumber = 1; frameNumber < 5; frameNumber++) {
    computeValues();
    int firstRegister = 160 + ( 8 * (  frameNumber - 1));
    int lastRegister = firstRegister + 7;
    for (int Register = firstRegister; Register <= lastRegister; Register++) {
      int segValIndex = lastRegister - Register + 1;
      Serial.print (F("Frame["));
      Serial.print(frameNumber);
      Serial.print(F("] - Register="));
      Serial.print(Register, HEX);
      Serial.print(F(" - Value="));
      Serial.println(segVal[segValIndex], HEX);
      //
      // program flow:
      Fetch();                        //fetch
      delay(100);
      if (Register == 160) {          //only before programming the first register: set PROGRAM Mode
        Program();
        delay(100);
      }
      outputBits(248);                //output 248 = F8 HEX = LDI instruction
      delay(100);
      Execute();                      //execute
      delay(100);
      outputBits(segVal[segValIndex]);//output segment value
      delay(100);
      Fetch();                        //fetch
      delay(100);
      outputBits(Register);           //output register value
      delay(100);
      Execute();                      //execute
      delay(100);
      setDataBusInput();              //set INPUT mode (disconnect outputs)
      delay(100);
      if (Register == 191) {          //if last register, go back to RUN mode
        Run();
        delay(100);
      }
    }
  }
}

void Run() {
  Serial.println(F("SETTING RUN MODE"));
  digitalWrite(ProgRun, 0);  //Relay in RUN Mode
  digitalWrite(FetchEx, 0);  //Relay in Fetch Mode
  setDataBusInput;
}

void Program() {
  Serial.println(F("SETTING PROGRAM MODE"));
  digitalWrite(ProgRun, 1);  //Relay in PROGRAM Mode
}

void Fetch() {
  Serial.println(F("Fetch..."));
  digitalWrite(FetchEx, 0);  //Relay in Fetch Mode
}
void Execute() {
  Serial.println(F("Execute..."));
  digitalWrite(FetchEx, 1);  //Relay in Execute Mode
}

void setDataBusInput() {
  for (int i = 1; i <= 8; i++) {
    pinMode(DataPin[i], INPUT);
  }
}

void setDataBusOutput() {
  for (int i = 1; i <= 8; i++) {
    pinMode(DataPin[i], OUTPUT);
  }
}

void outputBits(int inVal) {
  setDataBusOutput();
  for (int i = 7; i >= 0; i--) {
    int bitToCheck = round(pow(2, i)); //pow function needs to be rounded
    //Logical ANDing
    boolean bitToTransmit = inVal & bitToCheck;
    //Serial.print("Setting pin ");
    //Serial.print(i+2);
    //Serial.print(" to ");
    //Serial.println(bitToTransmit);
    digitalWrite((i + 2), bitToTransmit);
  }
}

