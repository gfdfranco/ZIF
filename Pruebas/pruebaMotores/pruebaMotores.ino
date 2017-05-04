int motorA1=9;
int motorA2=10;
int motorB1=11;
int motorB2=12;

void avanzar(){
  digitalWrite(motorA1,HIGH);
  digitalWrite(motorA2,LOW);
  digitalWrite(motorB1,LOW);
  digitalWrite(motorB2,HIGH);
  
}

void retroceder(){
  digitalWrite(motorA1,LOW);
  digitalWrite(motorA2,HIGH);
  digitalWrite(motorB1,HIGH);
  digitalWrite(motorB2,LOW);
}

void alto(){
  digitalWrite(motorA1,LOW);
  digitalWrite(motorA2,LOW);
  digitalWrite(motorB1,LOW);
  digitalWrite(motorB2,LOW);
}

void girarDerecha(){
  digitalWrite(motorA1,HIGH);
  digitalWrite(motorA2,LOW);
  digitalWrite(motorB1,HIGH);
  digitalWrite(motorB2,LOW);
}

void girarIzquierda(){
  digitalWrite(motorA1,LOW);
  digitalWrite(motorA2,HIGH);
  digitalWrite(motorB1,LOW);
  digitalWrite(motorB2,HIGH);
}

void setup(){
 pinMode(motorA1,OUTPUT);
 pinMode(motorA2,OUTPUT);
 pinMode(motorB1,OUTPUT);
 pinMode(motorB2,OUTPUT);
 delay(5000);

}

void loop(){
  avanzar();
  delay(1000);
  alto();
  delay(500);
  retroceder();
  delay(1000);
  girarDerecha();
  delay(500);
  alto();
  delay(500);
  girarIzquierda();
  delay(500);
  alto();
  delay(10500);
  
}
