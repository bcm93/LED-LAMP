#include <AcceleroMMA7361.h>

AcceleroMMA7361 accelero;
static int x = 0;
static int xAnterior = 0;
static int y = 0;
static int yAnterior = 0;
static int z = 0;
static int zAnterior = 0;
int estado = 0;
int rojo = 5;
int verde = 6;
int azul = 3;
int numR;
int numV;
int numA;
static int timeout = 0;
static int contador = 0;
static int parpadeo = 0;
static float tiempoAnterior = 0;
static float tiempoActual = 0;

void setup()
{
  Serial.begin(9600);
  accelero.begin(13, 12, 11, 10, A2, A1, A0);
  accelero.setARefVoltage(5);                   //sets the AREF voltage to 3.3V
  accelero.setSensitivity(LOW);                   //sets the sensitivity to +/-6G
  accelero.calibrate();
}

void loop()
{
  xAnterior = x;
  yAnterior = y;
  zAnterior = z;
  x = accelero.getXAccel();
  y = accelero.getYAccel();
  z = accelero.getZAccel();

  if(Serial.available()>0) {
    estado = Serial.read();
  }
  
  //LÁMPARA
  if(estado == '1') { //Luz blanca
    analogWrite(rojo, 255);
    analogWrite(verde, 255);
    analogWrite(azul, 255);
  }
  if( estado == '3'){ //Luz roja
    analogWrite(rojo, 255);
    analogWrite(verde, 0);
    analogWrite(azul, 0);
  }
  if( estado == '4'){ //Luz verde
    analogWrite(rojo, 0);
    analogWrite(verde, 255);
    analogWrite(azul, 0);
  }
  if( estado == '5'){  //Luz azul
    analogWrite(rojo, 0);
    analogWrite(verde, 0);
    analogWrite(azul, 255);
  }

  //PATATA CALIENTE
  if(estado == '2') { 
    
    //Control del TIMEOUT
    if(timeout == 0){
      timeout = random(100, 150);
      parpadeo = timeout / 8;
    }
    else {
      contador ++;
    }
        
    if (contador > (timeout - parpadeo)){ //Parpadeo
      if(contador == timeout){ //Fin del TIMEOUT
        timeout = 0;
        contador = 0;
        estado = 15;
        parpadeo = 0;
        analogWrite(rojo, 255);
        analogWrite(verde, 0);
        analogWrite(azul, 0);
      }

      else{
        if( (x > xAnterior + 150) || (x < xAnterior - 150) ||(y > yAnterior + 150) || (y < yAnterior - 150) ||(z > zAnterior + 150)){ //Lanzamiento
          tiempoActual = millis();
          if(tiempoActual > (tiempoAnterior + 500)) {
            numR = random(0, 256);
            numV = random(0, 256);
            numA = random(0, 256);
            analogWrite(rojo, numR);
            analogWrite(verde, numV);
            analogWrite(azul, numA);

            tiempoAnterior = tiempoActual;
          }
          else {
            analogWrite(rojo, 0);
            analogWrite(verde, 0);
            analogWrite(azul, 0);
            delay(1000);
            analogWrite(rojo, numR);
            analogWrite(verde, numV);
            analogWrite(azul, numA);
          }
        }
      
        else{ //Estático
          analogWrite(rojo, 0);
          analogWrite(verde, 0);
          analogWrite(azul, 0);
          delay(1000);
          analogWrite(rojo, numR);
          analogWrite(verde, numV);
          analogWrite(azul, numA);
        }

      }
    }
    else { 
      if( (x > xAnterior + 150) || (x < xAnterior - 150) ||(y > yAnterior + 150) || (y < yAnterior - 150) ||(z > zAnterior + 150)){ //Lanzamiento
        tiempoActual = millis();
        if(tiempoActual > (tiempoAnterior + 500)) {
          numR = random(0, 256);
          numV = random(0, 256);
          numA = random(0, 256);
          analogWrite(rojo, numR);
          analogWrite(verde, numV);
          analogWrite(azul, numA);

          
          tiempoAnterior = tiempoActual;
        }
      }
      
      else{ //Estático
        analogWrite(rojo, numR);
        analogWrite(verde, numV);
        analogWrite(azul, numA);
      }
    }
  }

  // MASCOTAS
  if(estado == '6') { 
    numR = random(0, 256);
    numV = random(0, 256);
    numA = random(0, 256);
    analogWrite(rojo, numR);
    analogWrite(verde, numV);
    analogWrite(azul, numA);
    estado = 15;
  }
  if(estado == '0') { //Luz Apagada
    analogWrite(rojo, 0);
    analogWrite(verde, 0);
    analogWrite(azul, 0);
    timeout = 0;
    contador = 0;
    parpadeo = 0;
  }
  delay(500);                                     
}
