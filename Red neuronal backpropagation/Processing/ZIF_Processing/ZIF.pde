import processing.serial.*;
Serial port;

Table table;
Table table2;
Table table3;
Table table4;
int bandera;
int prediccion=0;
TableRow newRow; 
String mensaje = null;
TableRow valor1;

void setup() {
  port = new Serial(this, "COM3", 115200);
  table = loadTable("H://MyData.csv", "header");
  table2 = loadTable("H://bandera.csv", "header");
  table3 = new Table();
  table4 = new Table();
  println(table.getRowCount() + " total rows in table"); 
  for (TableRow row : table.rows()) {
    int species = row.getInt("V1");
    println(int(species));
  }
  for (TableRow row : table2.rows()) {
    int species2 = row.getInt("bandera");
    bandera=int(species2);
  }
  print("BANDERA: ");
  println(bandera);
  
  table3.addColumn("bandera");
  newRow = table3.addRow();
  newRow.setInt("bandera", 5);
  
  valor1 = table4.addRow();
  
  table4.addColumn("Ultra1");
  valor1.setInt("Ultra1", 0);
  
  table4.addColumn("Ultra2");
  valor1.setInt("Ultra2", 0);
  
  table4.addColumn("Ultra3");
  valor1.setInt("Ultra3", 0);
  saveTable(table3, "H://bandera.csv");
  saveTable(table4, "H://ultrasonicos.csv");
  port.clear();
}
void draw(){
  table = loadTable("H://MyData.csv", "header");
  table2 = loadTable("H://bandera.csv", "header");
  for (TableRow row : table2.rows()) {
    int species2 = row.getInt("bandera");
    bandera=int(species2);
  }
  print("BANDERA: ");
  println(bandera);
  
  if(bandera==5){
    if(port.available()>0){
       mensaje = port.readStringUntil('\n');
    }
    if(mensaje!=null){
      try{
        int[] values = int(split(mensaje,','));
        println("VALORES: " + values[0] + " " + values[1] + " " + values[2]);
        valor1.setInt("Ultra1", values[0]);
        valor1.setInt("Ultra2", values[1]);
        valor1.setInt("Ultra3", values[2]);
       
        for (TableRow row : table.rows()) {
           prediccion = row.getInt("V1");
          
        }
        newRow.setInt("bandera", 0);
        saveTable(table3, "H://bandera.csv");
        saveTable(table4, "H://ultrasonicos.csv");
        delay(25);
      }
      catch(Exception e){
        println("ERROR");
      }
    }
  }
  else{
    newRow.setInt("bandera", 5);
    saveTable(table3, "H://bandera.csv");
    delay(25);
  }
  
  if(table.getRowCount()==1){
     println("Prediccion:"); 
    println(prediccion);
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
      
    }
    //port.write(char(prediccion));  
  }
  else{
    port.write('0');
  }
  
  port.clear();
}
  
  
  