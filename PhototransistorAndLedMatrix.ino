#define ledOnTreshold (100)

int latchPin = 8;  //Pin connected to LAT of module
int clockPin = 12; //Pin connected to CLK of module
int dataPin  = 11; //Pin connected to DI of module
 
void setup() {
  //Phototransistor
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  Serial.begin(9600);
  //LED matrix
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 
}

void shiftIt(byte dataOut) {
  // Shift out 8 bits LSB first, on rising edge of clock
  boolean pinState;
  digitalWrite(dataPin, LOW); //clear shift register read for sending data
  for (int i=0; i<8; i++) { // for each bit in dataOut send out a bit
    //set clockPin to LOW prior to sending bit
    digitalWrite(clockPin, LOW);
    if ( dataOut & (1<<i) ) {
      pinState = HIGH; // if the value of DataOut and (logical AND) a bitmask are true, set pinState to 1 (HIGH)
    }
    else {
      pinState = LOW;
    }
    digitalWrite(dataPin, pinState); //sets dataPin to HIGH or LOW depending on pinState
    digitalWrite(clockPin, HIGH); //send bit out on rising edge of clock
    digitalWrite(dataPin, LOW);
  }
  digitalWrite(clockPin, LOW); //stop shifting
}

void screenUpdate(int ledsAtRow[8], unsigned long delay_ms) { // function to display image
  unsigned long startime=millis();
  while(millis()-startime<delay_ms) {
    byte row = B10000000; // row 1 
    for (int iRow = 0; iRow < 8; iRow++) {
      digitalWrite(latchPin, LOW); // open latch ready to receive data
      shiftIt(~row); // row binary number
      shiftIt(ledsAtRow[iRow]); // LED array (inverted) 
      digitalWrite(latchPin, HIGH); // Close the latch, sending the data in the registers out to the matrix
      row = row >> 1; // bitshift right
    }
  } 
}

void turnOnLed(int (&ledsAtRow)[8], int iRow, int iCol) {
  ledsAtRow[iRow] |= 1<<(7-iCol);//pow(2, 7-iCol);
}

void loop() { 
  int ledsAtRow[8];
  for(int iRow=0; iRow < 8; iRow++) { //initialize ledsAtRow[]
    ledsAtRow[iRow]=0;
  }
  int val1 = analogRead(A1);
  int val2 = analogRead(A2);
  Serial.print("A1: ");
  Serial.print(val1);
  Serial.print(", A2: ");
  Serial.println(val2);
  if(val1 > ledOnTreshold) {
    turnOnLed(ledsAtRow, 0, 0);
  }
  if(val2 > ledOnTreshold) {
    turnOnLed(ledsAtRow, 0, 7);
  }
  screenUpdate(ledsAtRow, 50);
}
