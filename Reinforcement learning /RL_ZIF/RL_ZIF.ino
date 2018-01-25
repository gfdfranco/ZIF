#include <NewPing.h>

NewPing sonar(6, 5, 200);


int rewards [4][6] =
  { 
   {0, 0, -1, -1, -1, -1}, //>26 <20 
   {0, 0, 0, 0, 0, 0}, //<26 >20
   {0, 0, 0, 0, 0, 0}, //<25 >21
   {-1, -1, 0, 0, 0, 0} //<24 >22 
  };

int weights [4][6] = 
 {
   {0, 0, 0, 0, 0, 0},//>26 <20 
   {0, 0, 0, 0, 0, 0}, //<26 >20
   {0, 0, 0, 0, 0, 0}, //<25 >21
   {0, 0, 0, 0, 0, 0} //<24 >22 
 };

int motorA1=9;
int motorA2=10;
int motorB1=11;
int motorB2=12;
int bocina = 13;


int ultra_detectado = 0;
int seccion=0;
int n=0;
int num_random = 0;
int num_max = 0;
bool bandera=true;


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

void avanzar_pausado(){
      alto();
      delay(800);
      avanzar();
      delay(200);
}

void advertencia(){
      for(int i=0; i<4; i++){
        digitalWrite(bocina,HIGH);
        delay(450);
        digitalWrite(bocina,LOW);
        delay(450);
      }     
}

void accion(int n){
  switch(n){
    case 0:
      alto();
      delay(1500);
      avanzar();
      delay(100);
      alto();
      delay(1000);
      break;
    case 1:
      alto();
      delay(1500);
      avanzar();
      delay(200);
      alto();
      delay(1000);
      break;
    case 2:
      alto();
      delay(1500);
      girarDerecha();
      delay(150);
      alto();
      delay(1000);
      break;
     case 3:
      alto();
      delay(1500);
      girarDerecha();
      delay(300);
      alto();
      delay(1000);
      break;
     case 4:
      alto();
      delay(1500);
      girarIzquierda();
      delay(150);
      alto();
      delay(1000);
      break;
     case 5:
      alto();
      delay(1500);
      girarIzquierda();
      delay(300);
      alto();
      delay(1000);
      break;
  }
}

void imprimir(int actual, int detectado, int re, int peso, int seccion){
   Serial.print("ULTRASONICO ACTUAL: ");
   Serial.println(actual);
   Serial.print("ULTRASONICO ESTADO: ");
   Serial.println(detectado);
   Serial.print("RECOMPENSA: ");
   Serial.println(re);
   Serial.print("PESO: ");
   Serial.println(peso);
   Serial.print("SECCION: ");
   Serial.println(seccion);

   Serial.println();
   Serial.println("RECOMPENSAS ARREGLO:");

   for(int i =0; i< 4; i++){
    for(int j=0; j<6; j++){
      Serial.print(rewards[i][j]);
      Serial.print(",\t");
    }
    Serial.println();
   }

   Serial.println();
   Serial.println("PESOS ARREGLO:");

   for(int i =0; i< 4; i++){
    for(int j=0; j<6; j++){
      Serial.print(weights[i][j]);
      Serial.print(",\t");
    }
    Serial.println();
   }

   alto();
   delay(3000);
}

void setup() {
  pinMode(motorA1,OUTPUT);
  pinMode(motorA2,OUTPUT);
  pinMode(motorB1,OUTPUT);
  pinMode(motorB2,OUTPUT);
  pinMode(bocina, OUTPUT);   
  advertencia();
  //Serial.begin(9600);
}

void loop() {
  while(n < 250){ 
    while(sonar.ping_cm() > 19 || sonar.ping_cm() ==0){ 
        if(sonar.ping_cm() > 30){
          avanzar_pausado();
        }
        else if(sonar.ping_cm() ==0){
          retroceder();
          delay(600);
          alto();
          delay(300);
        }
        else{
          if(bandera == true){
              if(sonar.ping_cm()==0){
                retroceder();
                delay(300);
                alto();
                delay(300);
                break;
              }
              ultra_detectado = sonar.ping_cm();
              //Serial.print("DETECTADO: ");
              //Serial.println(ultra_detectado);
              bandera = false;
           }
          
             //seleccionamos seccion
             if(ultra_detectado <= 30 && ultra_detectado > 26){
                seccion = 0;
              }          
              if(ultra_detectado <= 26 && ultra_detectado >= 25){
                 seccion = 1;
              }
              if(ultra_detectado <= 24 && ultra_detectado >= 22){
                 seccion = 2;
              }
              if(ultra_detectado <= 21 && ultra_detectado >= 20){
                 seccion = 3;
              }

             //Se selecciona la accion
             num_max = weights[seccion][0]; 
             num_random = 0;
             for(int i=1; i<6; i++){ 
                if(weights[seccion][i] > num_max){
                     num_max = weights[seccion][i];
                     num_random = i;
                }
             }
             if(rewards[seccion][num_random] == -1){
                  num_random = random(0, 5);
                  while(rewards[seccion][num_random] < 0){
                   num_random = random(0, 5);
                }
             }
             
           /*  Serial.println();
             Serial.print("CICLO: ");
          Serial.println(n);
           Serial.print("SECCION: ");
          Serial.println(seccion); 
          Serial.print("ACCION CON MAYOR PESO: ");
          Serial.println(num_random);*/

          //HACEMOS LA ACCION PENSADA
          accion(num_random);
          //Serial.print("DESPUES DE LA ACCION: ");
          //Serial.println(sonar.ping_cm());
          //ESTABLECEMOS PUNTOS
          alto();
          delay(1000);
          if(sonar.ping_cm()==0){
                retroceder();
                delay(300);
                alto();
                delay(300);
                break;
              }
          if(sonar.ping_cm() > 30 || sonar.ping_cm() <= 20){
             rewards[seccion][num_random] = -1;
          
          }          
          if(sonar.ping_cm() <= 30 && sonar.ping_cm() > 26){
             rewards[seccion][num_random] = 5;
          }
          if(sonar.ping_cm() <= 26 && sonar.ping_cm() >= 25){
             rewards[seccion][num_random] = 15;
          }
          if(sonar.ping_cm() <= 24 && sonar.ping_cm() >= 23){
             rewards[seccion][num_random] = 15;
          }
          if(sonar.ping_cm() <= 22 && sonar.ping_cm() >= 21){
             rewards[seccion][num_random] = 50;
          }
          //ESTABLECEMOS PESOS
          num_max = weights[num_random][0];
          for(int i=1; i<6; i++){ 
            if(weights[num_random][i] > num_max){
               num_max = weights[num_random][i];
            }
          }
    
          weights[seccion][num_random] = rewards[seccion][num_random] + 2 * num_max;
          
          //imprimir(sonar.ping_cm(), ultra_detectado, rewards[seccion][num_random], weights[seccion][num_random], seccion );
          //Serial.print("N: ");
          //Serial.println(n);
          //Serial.print("RECOMPENSA: ");
          //Serial.println(rewards[ultra_detectado][num_random]);
          bandera=true;
        }
    }
    alto();
    delay(500);
    advertencia();
    n++;
  }
  alto();
    delay(500);
    n=0;
}
