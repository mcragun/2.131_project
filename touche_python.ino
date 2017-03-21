
#define LED_PIN 4



void setup() {
  // put your setup code here, to run once:
pinMode(LED_PIN, OUTPUT);
Serial.begin(9600);
Serial.println("Hello");
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Testing");
  blink_led();
  delay(2000);
}


void blink_led(){
   digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}
