/* sketch 1 
turn on a LED when the button is pressed
turn it off when the button is not pressed (or released)
*/

int pinButton = 2; //the pin where we connect the button
int LED = 3; //the pin we connect the LED
int buzzer = 4;

void setup() {
  Serial.begin(9600);
  pinMode(pinButton, INPUT); //set the button pin as INPUT
  pinMode(LED, OUTPUT); //set the LED pin as OUTPUT
  pinMode(buzzer, OUTPUT);
}

void loop() {
  int stateButton = digitalRead(pinButton); //read the state of the button
  Serial.println(stateButton);
  if(stateButton == 0) { //if is pressed
     digitalWrite(LED, HIGH); //write 1 or HIGH to led pin
     digitalWrite(buzzer, HIGH);
  } else { //if not pressed
     digitalWrite(LED, LOW);  //write 0 or low to led pin
     digitalWrite(buzzer, LOW);
  }
}
