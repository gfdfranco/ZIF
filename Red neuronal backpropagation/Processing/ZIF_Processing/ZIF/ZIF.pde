//Librerias necesarias
import processing.serial.*;

//librerias para opencv
import gab.opencv.*;
import processing.video.*;
import java.awt.Rectangle;


//Opencv
Capture video;
OpenCV opencv;
PImage src;
ArrayList<Contour> contours;

//Comunicación serial con arduino
Serial port;

//Archivos CSV
Table prediccion_csv;
Table bandera_csv;
Table modificar_bandera_csv;
Table valores_arduino_csv;
//Filas de archivos CSV
TableRow valor_fila_bandera; 
TableRow valor_fila_ultrasonicos;
//Variables necesarias
int bandera;
int prediccion=0;
int camara=0;
String mensaje = null;
//Condiguraciones opencv para detección de colores
int maxColors = 4;
int[] hues;
int[] colors;
int rangeWidth = 10;
//Variables y objetos que se utilizan para la deteccion
PImage[] outputs;
int colorToChange = -1;


//Configuraciones iniciales
void setup() {
   
  //conexión con arduino
  port = new Serial(this, "COM3", 115200);
  //Video
  String[] cameras = Capture.list();
  video = new Capture(this, 640, 480, cameras[1]);
  opencv = new OpenCV(this, video.width, video.height);
  contours = new ArrayList<Contour>();
  size(830, 480, P2D);
  //deteccion de colores
  colors = new int[maxColors];
  hues = new int[maxColors];
  outputs = new PImage[maxColors];
  
  //Se cargan archivos CSV
  prediccion_csv= loadTable("E://MyData.csv", "header");
  bandera_csv = loadTable("E://bandera.csv", "header");
  //Se crean nuevos archivos CSV
  modificar_bandera_csv = new Table();
  valores_arduino_csv = new Table();
  
  //Se obtiene la primer predicción 
  println(prediccion_csv.getRowCount() + " total rows in table"); 
  for (TableRow row : prediccion_csv.rows()) {
    int species = row.getInt("V1");
    println(int(species));
  }
  //Se obtiene el valor de la bandera actualmente
  for (TableRow row : bandera_csv.rows()) {
    int species2 = row.getInt("bandera");
    bandera=int(species2);
  }
  print("BANDERA: ");
  println(bandera);
  
  
  //SE CREAN ARCHIVOS CSV
  //La bandera se cambia a 5  con el fin que se puedan actualizar los valores en R
  modificar_bandera_csv.addColumn("bandera");
  valor_fila_bandera = modificar_bandera_csv.addRow();
  valor_fila_bandera.setInt("bandera", 5);
  
  //Los valores de los ultrasonicos se inician con 0
  valor_fila_ultrasonicos = valores_arduino_csv.addRow();
  
  valores_arduino_csv.addColumn("Ultra1");
  valor_fila_ultrasonicos.setInt("Ultra1", 0);
  
  valores_arduino_csv.addColumn("Ultra2");
  valor_fila_ultrasonicos.setInt("Ultra2", 0);
  
  valores_arduino_csv.addColumn("Ultra3");
  valor_fila_ultrasonicos.setInt("Ultra3", 0);
  
  valores_arduino_csv.addColumn("Camara");
  valor_fila_ultrasonicos.setInt("Camara", 0);
  
  //Se guardan con estos daros los archivos csv
  saveTable(modificar_bandera_csv, "E://bandera.csv");
  saveTable(valores_arduino_csv, "E://ultrasonicos.csv");
  //Es importante limpiar el puerto para la comunicación serial
  if(prediccion==5) prediccion= prediccion-1;
  port.clear();
  //que inicie video
  video.start();
}

//Se inicia ciclo con processing 
void draw(){
  
  background(150);
  if (video.available()) {
    video.read();
  }
  if(prediccion==5) prediccion= prediccion-1;
  opencv.loadImage(video);
  opencv.useColor();
  src = opencv.getSnapshot();
  opencv.useColor(HSB);
  detectColors();
  
  //DETECTAR OBJETO
  image(src, 0, 0);
  for (int i=0; i<outputs.length; i++) {
    if (outputs[i] != null) {
      image(outputs[i], width-src.width/4, i*src.height/4, src.width/4, src.height/4);
      noStroke();
      fill(colors[i]);
      rect(src.width, i*src.height/4, 30, src.height/4);
    }
  }
  textSize(20);
  stroke(255);
  fill(255);
  if (colorToChange > -1) {
    text("Click para cambiar el color " + colorToChange, 10, 25);
  } else {
    text("Selecciona un numero del 1 al 4", 10, 25);
  }
  displayContoursBoundingBoxes();
  //..........................
  //Se leen archivos csv
  prediccion_csv= loadTable("E://MyData.csv", "header");
  bandera_csv = loadTable("E://bandera.csv", "header");
  //Se imprime el valor de la bandera
  for (TableRow row : bandera_csv.rows()) {
    int species2 = row.getInt("bandera");
    bandera=int(species2);
  }
  print("BANDERA: ");
  println(bandera);
  
  //Si eciste un cambio en la bandera sobreescribimos los valores de arduino para que se actualizen los datos
  if(bandera==5){
    //SE LEEN LOS SENSORES POR MEDIO DE COMUNICACIÓN SERIAL
    if(port.available()>0){
       mensaje = port.readStringUntil('\n');
    }
    if(mensaje!=null){
      try{
        int[] values = int(split(mensaje,','));
        println("VALORES: " + values[0] + " " + values[1] + " " + values[2]);
        valor_fila_ultrasonicos.setInt("Ultra1", values[0]);
        valor_fila_ultrasonicos.setInt("Ultra2", values[1]);
        valor_fila_ultrasonicos.setInt("Ultra3", values[2]);
        valor_fila_ultrasonicos.setInt("Camara", camara);
       //SE LEE LA PREDICCIÓN QUE DETECTO R
        for (TableRow row : prediccion_csv.rows()) {
           prediccion = row.getInt("V1");
          
        }
        //Se sobreescriben los csv
        //se cambia la bandera en 0
        valor_fila_bandera.setInt("bandera", 0);
        saveTable(modificar_bandera_csv, "E://bandera.csv");
        saveTable(valores_arduino_csv, "E://ultrasonicos.csv");
        delay(25);
      }
      //En caso que algo pase, generalmente basura del serial
      catch(Exception e){
        println("ERROR");
      }
    }
  }
  //Si la bandera esta en 0
  else{
    //se cambia la bandera para que R pueda leer los nuevos datos
    valor_fila_bandera.setInt("bandera", 5);
    saveTable(modificar_bandera_csv, "E://bandera.csv");
    delay(25);
  }
  
  //SE MUESRA LA PREDICCION POR R
  if(prediccion_csv.getRowCount()==1){
    
    println("Prediccion:"); 
    println(prediccion);
    //SE MANDA POR SERIAL LA PREDICCION AL ARDUINO PARA QUE EJECUTE LOS MOVIMIENTOS
   
    switch(prediccion){
      case 0:
          port.write('0');
          break;
      case 1:
          port.write('1');
          break;
      case 2:
          port.write('2');
          break;
      case 3:
         port.write('3');
          break;
      case 4:
         port.write('4');
          break;
    }
    //port.write(char(prediccion));  
  }
  //Si no existe ninguna prediccion mandamos por serial que sea al grupo 0
  else{
    port.write('0');
  }
     textSize(20);
  stroke(255);
  fill(255);
  text("Predicción: " + prediccion, 400, 25);
    switch(prediccion){
      case 0:
          text("ALTO TOTAL", 200, 100);
          break;
      case 1:
          text("AVANZAR", 200, 100);
          break;
      case 2:
          text("GIRAR DERECHA", 200, 100);
          break;
      case 3:
        text("GIRAR IZQUIERDA", 200, 100);
          break;
      case 4:
         text("DETECCIÓN VISIÓN ARTIFICIAL", 200, 100);
          break;  
    }
  port.clear();
}
  
  
//FUNCIONES 
//.......................................................................................
void detectColors() {
  for (int i=0; i<hues.length; i++) {
    if (hues[i] <= 0) continue;
    opencv.loadImage(src);
    opencv.useColor(HSB);
    opencv.setGray(opencv.getH().clone());
    int hueToDetect = hues[i];
    opencv.inRange(hueToDetect-rangeWidth/2, hueToDetect+rangeWidth/2);
    opencv.erode();
    outputs[i] = opencv.getSnapshot();
  }
  
  if (outputs[0] != null) {
    opencv.loadImage(outputs[0]);
    contours = opencv.findContours(true,true);
  }
}

void displayContoursBoundingBoxes() {
 
    for (int i=0; i<contours.size(); i++) {
      if(contours.size()>20) camara=1;
      else camara=0;
      Contour contour = contours.get(i);
      Rectangle r = contour.getBoundingBox();
      
      if (r.width < 20 || r.height < 20){
       continue;
      }
      stroke(255, 0, 0);
      fill(255, 0, 0, 150);
      strokeWeight(2);
      rect(r.x, r.y, r.width, r.height);
    }

}
void mousePressed() {
    
  if (colorToChange > -1) {
    
    color c = get(mouseX, mouseY);
    println("r: " + red(c) + " g: " + green(c) + " b: " + blue(c));
   
    int hue = int(map(hue(c), 0, 255, 0, 180));
    
    colors[colorToChange-1] = c;
    hues[colorToChange-1] = hue;
    
    println("color ID " + (colorToChange-1) + ", value: " + hue);
  }
}
void keyPressed() {
  
  if (key == '1') {
    colorToChange = 1;
    
  } else if (key == '2') {
    colorToChange = 2;
    
  } else if (key == '3') {
    colorToChange = 3;
    
  } else if (key == '4') {
    colorToChange = 4;
  }
}

void keyReleased() {
  colorToChange = -1; 
}
  
  