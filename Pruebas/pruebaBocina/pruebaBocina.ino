
int bocina = 13;

void setup() {                
  pinMode(bocina, OUTPUT);     
}


void loop() {
  digitalWrite(bocina, HIGH);  
  delay(1000);               
  digitalWrite(bocina, LOW);    
  delay(1000);               
}
