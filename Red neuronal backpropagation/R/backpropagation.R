rm(list=ls())
#Librerías necesarias...........
library(grid)
library(MASS)
library(neuralnet)

#Función para hacer un delay a la hora de leer los datos
testit <- function(x)
{
    p1 <- proc.time()
    Sys.sleep(x)
    proc.time() - p1 # The cpu usage should be negligible
}

#Leemos los datos para entrenar nuestra red
datos <- read.csv("E://entrenar.csv",header=TRUE)
datos_evaluar  <- read.csv("E://evaluaciones.csv",header=TRUE)

#Modelo de nuestra red Neuronal
red1<-neuralnet(Respuesta ~ Ultra1+Ultra2+Ultra3+Camara,datos,hidden=10,threshold=0.01)
#Checamos error
red1$result.matrix


#Checamos que corresponen los datos con los entrenados
salida<-compute(red1,datos[-5])$net.result
salida

#Hacemos las predicciones
salida<-compute(red1,datos_evaluar[-5])$net.result
write.csv(round(salida,0), file = "E://MyData.csv")

#CREAMOS UN CICLO PARA QUE SIEMPRE ESTE LEYENDO LOS DATOS QUE MANDA EL ARDUINO
x<-2
while(x>1){
	testit(0.1)
	#LEEMOS DATOS ACTUALES PARA SABER QUE HACER 
	input <- read.csv("E://bandera.csv",header=TRUE)
	#Si la bandera es 0, cargamos los datos del ultrasonico y escrimos la salida en un csv
	if(input[1,1]==0){
     		datos_ultra<- read.csv("E://ultrasonicos.csv",header=TRUE)
     		salida<-compute(red1,datos_ultra)$net.result
     		write.csv(round(salida,0), file = "E://MyData.csv")
	}
}