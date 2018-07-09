// Rohan_Any_MRtriggerDelay.ino, may 20, 2016, by jdcircuits
// User patameters input starts at the beginning of the "void loop()" line 30
int BreathingRatePerMin; // User: Breaths Per Minute
int InhalationDuration; // User: ms of inhale time
int BreathHoldDuration; // User: ms of Breath Hold
int MRtriggerDelay;     // User: ms delay from Inhale Start Time 
int MRtriggerDuration;
int BreathsPerTrigger;    // User: 1 and 8 have been used in some experiments
int BreathDurationMS;         // see calculation in "setup"
int ExhaleDurationMS;         // see calculation in "setup"
int O2pin =      53;      // the number of the Arduino I/O pin
int N2pin =      51;      // the number of the Arduino I/O pin
int HPpin =      49;      // the number of the Arduino I/O pin
int MRtrigPin =  43;      // the number of the Arduino I/O pin
int ExhalePin =  47;      // the number of the Arduino I/O pin
int HPmodePin =  45;      // the state of the switch driven Arduino I/O pin
int val;                 //variable to store the read value
int BPM;                 // Counter for digitalWrite(MRtrigPin, HIGH);

void setup() {
  // Serial.begin(9600); // open the serial port at 9600 bps:
pinMode(O2pin, OUTPUT);
pinMode(N2pin, OUTPUT);
pinMode(HPpin, OUTPUT);
pinMode(MRtrigPin, OUTPUT);
pinMode(ExhalePin, OUTPUT);
pinMode(HPmodePin, INPUT_PULLUP);

}
void loop() {
BreathingRatePerMin = 80; // User: Breaths Per Minute 80
InhalationDuration = 140; // User: ms of inhale time 140
BreathHoldDuration = 200; // User: ms of Breath Hold 200
MRtriggerDelay = 180;     // User: ms delay from Inhale Start Time "time zero", 180 typical
MRtriggerDuration = 1;    // User: ms Scan Trigger Width  5
BreathsPerTrigger = 1;    // User: 1 and 8 have been used in some experiments 1
//Allowed MRtriggerDelays with MRtriggerDuration = 1 : 0 to 138 ok |  141 to 338 ok | 341 to 748 ok 
BreathDurationMS = (60000 / BreathingRatePerMin) ;         // 750
ExhaleDurationMS = (BreathDurationMS - (InhalationDuration + BreathHoldDuration)); // 410

if ((MRtriggerDelay + MRtriggerDuration) < InhalationDuration ) {
    //  Serial.println("TrigInhale");
    //  Serial.println(MRtriggerDelay);
  TrigInhale();
}

if ((MRtriggerDelay > InhalationDuration) && ((MRtriggerDelay  + MRtriggerDuration)) < (InhalationDuration + BreathHoldDuration)) {
    //  Serial.println("TrigHold");
    //  Serial.println(MRtriggerDelay);
  TrigHold();
}

if ((MRtriggerDelay ) > (InhalationDuration + BreathHoldDuration) && ((MRtriggerDelay  + MRtriggerDuration)) < (BreathDurationMS)  ) {
    //  Serial.println("TrigExhale");
    //  Serial.println(MRtriggerDelay);
 TrigExhale();
}

} // Go back to the beginning

void TrigInhale() {
  int S1; // inhale time
  int S2; // time from end of inhale to beginning of scan trigger  40
  int S3; // time that scan trigger is high 5
  int S4; // time from end of scan trigger going low and end of breath hold 200 - 45 = 155
  int S5; // exhale time 750 - (140 + 200) = 410
  int S6;

//  Serial.println("Hello");
  S1 = MRtriggerDelay; // 70
  S2 = MRtriggerDuration; //2
  S3 = ((InhalationDuration - (S1 + S2) )); // 68
  S4 = BreathHoldDuration; //  200
  S5 = (BreathDurationMS - (InhalationDuration + BreathHoldDuration)); //410
// (MRtriggerDelay - InhalationDuration);
  val = digitalRead(HPmodePin); // HPmodePin = low = led on, HPmode is ON 
  //Start Inhale
  if (val == LOW) {
    digitalWrite(O2pin, HIGH);  
    digitalWrite(N2pin, LOW);
    digitalWrite(HPpin, HIGH);  
    digitalWrite(ExhalePin, LOW);
  }else{
    digitalWrite(O2pin, HIGH);  
    digitalWrite(N2pin, HIGH);
    digitalWrite(HPpin, LOW);  
    digitalWrite(ExhalePin, LOW);
  }  
  delay (S1); // MRtriggerDelay // 70

// Start Scan Trigger high unless BreathsPerTrigger is count up
  if (BreathsPerTrigger == 1) {
    digitalWrite(MRtrigPin, HIGH);
  } else if ((BPM) == (BreathsPerTrigger )) {     
     digitalWrite(MRtrigPin, HIGH);
     BPM = 0;
     BPM = (BPM + 1);
      }else {        
        digitalWrite(MRtrigPin, LOW);
        BPM = (BPM + 1);  
        }
delay (S2); // MRtriggerDuration = 2

  // Start Scan Trigger low
  digitalWrite(MRtrigPin, LOW);
  delay (S3); // Continue inhale for the inhale time that is left 68 
  ////////////////////////////////////////////////////
  // Start BreathHoldDuration  200
  digitalWrite(O2pin, LOW);  
  digitalWrite(N2pin, LOW);
  digitalWrite(HPpin, LOW);  
  digitalWrite(ExhalePin, LOW);
  delay (S4); //BreathHoldDuration - (S2 + S3); //  200 - 45 = 155

  //Start Exhale
  digitalWrite(O2pin, LOW);  
  digitalWrite(N2pin, LOW);
  digitalWrite(HPpin, LOW);  
  digitalWrite(ExhalePin, HIGH);
  delay (S5); //BreathDurationMS - (InhalationDuration + BreathHoldDuration); = 410
  
} // end of TrigInhale Function

void TrigHold(){
  int T1; // inhale time
  int T2; // time from end of inhale to beginning of scan trigger  40
  int T3; // time that scan trigger is high 2
  int T4; // time from end of scan trigger going low and end of breath hold 200 - 45 = 155
  int T5; // exhale time 750 - (140 + 200) = 410

    //  Serial.println("Hello");
  T1 = InhalationDuration; //140
  T2 = (MRtriggerDelay - InhalationDuration); //40
  T3 = MRtriggerDuration; //5
  T4 = BreathHoldDuration - (T2 + T3); //  200 - 45 = 155
  T5 = BreathDurationMS - (InhalationDuration + BreathHoldDuration); //410

  val = digitalRead(HPmodePin); // HPmodePin = low = led on, HPmode is ON 
  //Start Inhale
  if (val == LOW) {
    digitalWrite(O2pin, HIGH);  
    digitalWrite(N2pin, LOW);
    digitalWrite(HPpin, HIGH);  
    digitalWrite(ExhalePin, LOW);
  }else{
    digitalWrite(O2pin, HIGH);  
    digitalWrite(N2pin, HIGH);
    digitalWrite(HPpin, LOW);  
    digitalWrite(ExhalePin, LOW);
  }  
  delay (T1); // InhalationDuration; 140

  // Start Breath Held
  digitalWrite(O2pin, LOW);  
  digitalWrite(N2pin, LOW);
  digitalWrite(HPpin, LOW);  
  digitalWrite(ExhalePin, LOW);
  delay (T2); // (MRtriggerDelay - InhalationDuration); 40 to MRtrigger_start_high
  ///////////////////////////////////////////////////////
  // Start Scan Trigger high
  if (BreathsPerTrigger == 1) {
    digitalWrite(MRtrigPin, HIGH);
  } else if ((BPM) == (BreathsPerTrigger )) {     
     digitalWrite(MRtrigPin, HIGH);
     BPM = 0;
     BPM = (BPM + 1);
      }else {        
        digitalWrite(MRtrigPin, LOW);
        BPM = (BPM + 1);  
        }

  delay (T3); // MRtriggerDuration; pulse width of 5 is typical
  ////////////////////////////////////////////////////
  // Start Scan Trigger low
  digitalWrite(MRtrigPin, LOW);

  // Continue the breath hold time that is left
  digitalWrite(O2pin, LOW);  
  digitalWrite(N2pin, LOW);
  digitalWrite(HPpin, LOW);  
  digitalWrite(ExhalePin, LOW);
  delay (T4); //BreathHoldDuration - (T2 + T3); //  200 - 45 = 155

  //Start Exhale
  digitalWrite(O2pin, LOW);  
  digitalWrite(N2pin, LOW);
  digitalWrite(HPpin, LOW);  
  digitalWrite(ExhalePin, HIGH);
  delay (T5); //BreathDurationMS - (InhalationDuration + BreathHoldDuration); = 410

  } // end of TrigHold function

void TrigExhale() {
  int U1; // InhalationDuration; // 140
  int U2; // BreathHoldDuration; // 200
  int U3; // exhale time before the scan trigger goes high 160
  int U4; // time that the scan trigger is high 1
  int U5; // exhale time after the trigger goes low 249

  U1 = InhalationDuration; // 140
  U2 = BreathHoldDuration; // 200
  U3 = (MRtriggerDelay - (InhalationDuration + BreathHoldDuration) ) ; // 500 - (140 + 200) = 160
  U4 = MRtriggerDuration; // 1
  U5 = BreathDurationMS - (InhalationDuration + BreathHoldDuration + U3 + MRtriggerDuration); // 750-340-160-1=249
    //  Serial.print("U3   ");
    //  Serial.println( U3 );  

  val = digitalRead(HPmodePin); // HPmodePin = low = led on, HPmode is ON 
  //Start Inhale
  if (val == LOW) {
    digitalWrite(O2pin, HIGH);  
    digitalWrite(N2pin, LOW);
    digitalWrite(HPpin, HIGH);  
    digitalWrite(ExhalePin, LOW);
  }else{
    digitalWrite(O2pin, HIGH);  
    digitalWrite(N2pin, HIGH);
    digitalWrite(HPpin, LOW);  
    digitalWrite(ExhalePin, LOW);
  }  
  delay (U1); // InhalationDuration; 140

  // Start Breath Held
  digitalWrite(O2pin, LOW);  
  digitalWrite(N2pin, LOW);
  digitalWrite(HPpin, LOW);  
  digitalWrite(ExhalePin, LOW);
  delay (U2); // BreathHoldDuration 200
  
  //Start Exhale (before the mr trigger goes high)
  digitalWrite(O2pin, LOW);  
  digitalWrite(N2pin, LOW);
  digitalWrite(HPpin, LOW);  
  digitalWrite(ExhalePin, HIGH);  
  delay (U3); // 160
  
  // Start Scan Trigger high
  if (BreathsPerTrigger == 1) {
    digitalWrite(MRtrigPin, HIGH);
  } else if ((BPM) == (BreathsPerTrigger )) {     
     digitalWrite(MRtrigPin, HIGH);
     BPM = 0;
     BPM = (BPM + 1);
      }else {        
        digitalWrite(MRtrigPin, LOW);
        BPM = (BPM + 1);  
        }

  delay (U4); // MRtriggerDuration; 1

  // Start Scan Trigger low
  digitalWrite(MRtrigPin, LOW);
  
  delay (U5);
  
} // end of TrigExhale function
