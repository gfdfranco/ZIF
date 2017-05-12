library(grid)
library(MASS)
library(neuralnet)

testit <- function(x)
{
    p1 <- proc.time()
    Sys.sleep(x)
    proc.time() - p1 # The cpu usage should be negligible
}

datos <- read.csv("H://entrenar.csv",header=TRUE)
attach(datos)
red1<-neuralnet(Respuesta ~ Ultra1+Ultra2+Ultra3,datos,hidden=10,threshold=0.01)
salida<-compute(red1,datos[-4])$net.result
write.csv(round(salida,1), file = "H://MyData.csv")
x<-2
while(x>1){
	testit(0.1)
	input <- read.csv("H://bandera.csv",header=TRUE)
	if(input[1,1]==0){
     		datos_ultra<- read.csv("H://ultrasonicos.csv",header=TRUE)
     		salida<-compute(red1,datos_ultra)$net.result
     		write.csv(round(salida,1), file = "H://MyData.csv")
	}
}