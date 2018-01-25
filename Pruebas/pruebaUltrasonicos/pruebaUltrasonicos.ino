#include <NewPing.h>
 
//trigger,echo,max distance
NewPing sonar1(8, 7, 200);
NewPing sonar2(6, 5, 200);
NewPing sonar3(4, 3, 200);
 
void setup() {
  Serial.begin(9600);
}
 
void loop() {
  delay(50);
  Serial.print("Ultra1: ");
  Serial.print(sonar1.ping_cm());
  Serial.print("  ,Ultra2: ");
  Serial.print(sonar2.ping_cm());
  Serial.print("  ,Ultra3: ");
  Serial.println(sonar3.ping_cm());
}
