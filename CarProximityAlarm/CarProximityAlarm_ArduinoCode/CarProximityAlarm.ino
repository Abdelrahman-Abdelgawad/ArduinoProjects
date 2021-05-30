/**********************************************************************
 *
 * PROJECT: Car Proximity Alarm                                                          
 * DATE:    28/05/2021
 *
 *********************************************************************/


/******************************** Macros *****************************/

#define TriggerPin                   7    // trigger pin in ultrasonic sensor
#define EchoPin                      6    // echo pin in ultrasonic sensor

#define SafeDistanceLED_Pin          8    // pin safe distance led
#define CautionDistanceLED_Pin       9    // pin caution distance Led 
#define DangerDistanceLED_Pin        10   // pin danger distance led

#define BuzzerPin                    11   // pin danger buzzer 

#define LimitUpper                   300  // 3 meter
#define LimitLower                   100  // 1 meter

#define SpeedOfSound_us_cm           29   // speed of sound in microseconds per centimeters 

#define TriggerInterval_ms           100  // interval between each trigger and the next in milliseconds

#define TriggerHighPulseDuration_us  10   // the high pulse duration of the trigger pin of the ultrasonic sensor 
#define TriggerPrePulseDuration_us   2    // the low duration before the trigger pulse

#define SerialBaudRate               9600 // the baud rate of the serial communication


/******************************** Setup *****************************/

void setup() 
{
  // configuring LED pins to output
  pinMode(SafeDistanceLED_Pin, OUTPUT);       
  pinMode(CautionDistanceLED_Pin, OUTPUT);
  pinMode(DangerDistanceLED_Pin, OUTPUT);

  // configuring buzzer pin to output
  pinMode(BuzzerPin, OUTPUT);

  // configuring ultrasonic pins to output
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // setting LED pins to low
  digitalWrite(SafeDistanceLED_Pin, LOW);
  digitalWrite(CautionDistanceLED_Pin, LOW);  
  digitalWrite(DangerDistanceLED_Pin, LOW);
  
  // setting buzzer pin to low
  digitalWrite(BuzzerPin, LOW);
  
  // initialize serial communication
  Serial.begin(SerialBaudRate);  
}


/********************************* Loop ******************************/

void loop()
{ 
  // variables for duration of the trigger and the distance result in centimeters
  long echoDuration_us, distance_cm; 

  /* 
   * The trigger pin is triggered by a HIGH pulse of 2 or more microseconds.
   * Give a short LOW pulse beforehand to ensure a clean HIGH pulse. 
   */
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(TriggerPrePulseDuration_us);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(TriggerHighPulseDuration_us);
  digitalWrite(TriggerPin, LOW);
 
  /*
   * Assign to duration the time (in microseconds) from the sending
   * of the trigger to the reception of its echo off of an object. 
   */
  echoDuration_us = pulseIn(EchoPin, HIGH);

  /*
   * The speed of sound is 340 m/s or 29 microseconds per centimeter.
   * The trigger travels out and back, so to find the distance of the object
   * we take half of the distance travelled, thus "/ 2". 
   */
  distance_cm = echoDuration_us / SpeedOfSound_us_cm / 2;
  

  // writting the distance to the serial (for debugging)
  Serial.print(distance_cm);

  /*
   * Checks if the distance is lower than the defined lower limit; 
   *    If so, execute Danger()
   */
  if(distance_cm < LimitLower)
    Danger();

  /*
   * Else if the distance is lower than the defined upper limit and higher than the defined lower limit;
   *    If so, execute Caution()
   */
  else if((distance_cm < LimitUpper) && (distance_cm > LimitLower))
    Caution();
  
  // Else, execute Safe()
  else
    Safe();

  // Delay for the defined trigger interval time
  delay(TriggerInterval_ms);

  Serial.println();
  Serial.println();
}


/********************************* Functions ******************************/

// Indicates that the distance is in safe range.
void Safe (void)
{
  digitalWrite(BuzzerPin, LOW);               // Sets the buzzer pin to low (Turns off the buzzer)
  digitalWrite(DangerDistanceLED_Pin, LOW);   // Sets the danger distance LED pin to low
  digitalWrite(CautionDistanceLED_Pin, LOW);  // Sets the caution distance LED pin to low
  
  digitalWrite(SafeDistanceLED_Pin, HIGH);    // Sets the safe distance LED pin to high
  
  Serial.print("  Safe");                     // Prints safe message (for debugging)
}


// Indicates that the distance is in caution range.
void Caution (void)
{
  digitalWrite(BuzzerPin, LOW);                // Sets the buzzer pin to low (Turns off the buzzer)
  digitalWrite(SafeDistanceLED_Pin, LOW);      // Sets the safe distance LED pin to low
  digitalWrite(DangerDistanceLED_Pin, LOW);    // Sets the danger distance LED pin to low
  
  digitalWrite(CautionDistanceLED_Pin, HIGH);  // Sets the caution distance LED pin to high
  
  Serial.print("  Caution");                   // Prints caution message (for debugging)
}


// Indicates that the distance is in danger range.
void Danger (void)
{
  digitalWrite(SafeDistanceLED_Pin, LOW);     // Sets the safe distance LED pin to low
  digitalWrite(CautionDistanceLED_Pin, LOW);  // Sets the caution distance LED pin to low
  
  digitalWrite(DangerDistanceLED_Pin, HIGH);  // Sets the danger distance LED pin to high
  digitalWrite(BuzzerPin, HIGH);              // Sets the buzzer pin to high (Turns on the buzzer)
  
  Serial.print("  !!Danger!!");               // Prints danger message (for debugging)
}

