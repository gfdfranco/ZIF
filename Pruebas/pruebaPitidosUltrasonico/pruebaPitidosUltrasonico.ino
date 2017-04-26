#include <NewPing.h>
 
//trigger,echo,max distance
NewPing sonar1(8, 7, 200);
NewPing sonar2(6, 5, 200);
NewPing sonar3(4, 3, 200);
int bocina = 13;
int t;
void setup() {
  pinMode(bocina, OUTPUT); 
}
 
void loop() {
  t = map(sonar1.ping_cm(), 0, 200, 0, 1000);
  if(sonar1.ping_cm() > 0 && sonar1.ping_cm() < 50 ) {
    digitalWrite(bocina, HIGH);  
    delay(t);               
    digitalWrite(bocina, LOW);    
    delay(t);  
  }
  else{
     digitalWrite(bocina, LOW); 
  }
}
