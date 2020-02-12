#include <MsTimer2.h>
int sensor=0;
//const unsigned int Tp= 200; // Intervalo de muestreo en ms
// Asignación de pines a segmentos
const byte segs[8]= {10,11,7,6,4,8,5,9}; //p-k-f-e-d-c-b-a

// Asignación de pines a dígitos
const byte digs[2]= {3,2}; // 1-2

// Segmentos que encienden cada dígito del 0 al 9
const byte digitos[11]= {0b00111111, 0b00000110, 0b11011011,
            0b01001111, 0b11100110, 0b11101101, 0b11111101,
            0b00000111, 0b11111111, 0b11101111,0b11000000};

// Variables globales
volatile int digito= 0; // Dígito encendido (1 o 2);
volatile byte byte1= 0;  // Valor a presentar en dígito 1
volatile byte byte2= 0;  // Valor a presentar en dígito 2
//volatile unsigned int tp=0;
int vmedio[5];
int a=0;
int b=0;
int lectura;
int temp;
void setup() {
  for (int pin= 2; pin< 12; pin++) { // Pone modo salidas
    pinMode(pin, OUTPUT);
  }
  analogReference(INTERNAL);  
  MsTimer2::set(5,multiplexar); // cada 5 ms
  MsTimer2::start();

}

void loop() {

}

// Interrupción de T2 cada 5 ms para multiplexado del display
void multiplexar() {
  lectura=analogRead(sensor);
  for(int i=0;i<39;i++){
    lectura+=analogRead(sensor);
  }
  lectura=lectura/40;  
  if(a++<5)
    vmedio[a]=lectura;
  else{
    a=0;
    vmedio[a]=lectura;
  }
  if(b++>=40){
    for(int i=0;i<5;i++){
      lectura=(lectura+vmedio[i])/2;
    }
    temp=(lectura-(5120/11))*110/1024;
    b=0;
  }
  
  if(temp<0){
    byte1=10;
    byte2=-temp%10;
  }
  else if(temp<(-9)){
    byte1=10;
    byte2=10;   
  }
  else{  
    byte1= temp / 10; // Calcula decenas
    byte2= temp % 10; // Calcula unidades
  }

  digitalWrite(digs[0],HIGH); // Apaga D1
  digitalWrite(digs[1],HIGH); // Apaga D2
  switch (digito) {
    case 0:
    segmentos(byte1); // Saca segmentos digito 1
    digitalWrite(digs[digito],LOW); // Enciende D1
    digito= 1;
    break;
    case 1:
    segmentos(byte2); // Saca segmentos digito 2
    digitalWrite(digs[digito],LOW); // Enciende D2
    digito= 0;
    break;
  }  
}

void segmentos(byte valor) {
  for(int seg= 0; seg< 8; seg++) {
    digitalWrite(segs[seg], !bitRead(digitos[valor], seg));
  }
}  
