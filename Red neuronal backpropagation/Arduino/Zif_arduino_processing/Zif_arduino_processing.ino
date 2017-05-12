#include <NewPing.h>

#define DISTANCIA 60
//trigger,echo,max distance
NewPing sonar1(8, 7, 200);
NewPing sonar2(6, 5, 200);
NewPing sonar3(4, 3, 200);
int ultra1,ultra2,ultra3;
String mensaje;
int motorA1=9;
int motorA2=10;
int motorB1=11;
int motorB2=12;

char val; 
int bocina = 13; 

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

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 pinMode(motorA1,OUTPUT);
 pinMode(motorA2,OUTPUT);
 pinMode(motorB1,OUTPUT);
 pinMode(motorB2,OUTPUT);
 pinMode(bocina,OUTPUT);
}

void loop() {
  Serial.flush();
  if(sonar1.ping_cm()< DISTANCIA && sonar1.ping_cm()>0){
    ultra1=1;
  }
  if(sonar1.ping_cm()> DISTANCIA || sonar1.ping_cm()==0){
    ultra1=0;
  }

   if(sonar2.ping_cm()< DISTANCIA && sonar2.ping_cm()>0){
    ultra2=1;
  }
  if(sonar2.ping_cm()> DISTANCIA || sonar2.ping_cm()==0){
    ultra2=0;
  }

   if(sonar3.ping_cm()< DISTANCIA && sonar3.ping_cm()>0){
    ultra3=1;
  }
  if(sonar3.ping_cm()> DISTANCIA || sonar3.ping_cm()==0){
    ultra3=0;
  }
  
  mensaje = String(ultra1)+","+String(ultra2)+","+String(ultra3)+",";
  Serial.println(mensaje);


  if (Serial.available()) 
   { 
     val = Serial.read(); 
   }
   if (val == '1') 
   { 
     digitalWrite(bocina,LOW);
     avanzar();
   } 
    if (val == '2') 
   { 
    digitalWrite(bocina,HIGH);
    girarDerecha();
   } 
    if (val == '3') 
   { 
    digitalWrite(bocina,HIGH);
    girarIzquierda();
   } 
   if (val == '0') 
   { 
    digitalWrite(bocina,LOW);
    alto();
   } 
   
   
   delay(10);
}
