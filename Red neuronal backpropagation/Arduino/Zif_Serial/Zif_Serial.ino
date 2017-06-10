String mensaje;
int ultra1=1,ultra2=0,ultra3=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.flush();
  mensaje = String(ultra1)+","+String(ultra2)+","+String(ultra3)+",";
  Serial.println(mensaje);
  delay(10);
}
