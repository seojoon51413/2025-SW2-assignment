#define PIN_LED 7
unsigned int count, toggle;
int toggle_state(int toggle){
  if (toggle==0){
    toggle=1;
  }
  else{
    toggle=0;
  }
  return toggle;
} 
void setup() {
   pinMode(PIN_LED, OUTPUT);
   Serial.begin(115200); 
   while(!Serial){
      ;
    }
   count=toggle=0;
   digitalWrite(PIN_LED, toggle);
}

void loop() {
  delay(1000);
  toggle=1;
  while(count<5){
    toggle=toggle_state(toggle);
    digitalWrite(PIN_LED, toggle);
    delay(100);
    toggle=toggle_state(toggle);
    digitalWrite(PIN_LED,toggle);
    delay(100);
    count=count+1;
  }
  while(1){}
}
