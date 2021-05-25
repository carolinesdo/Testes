/*
    ARQUIVO DE TESTES
    MAIORES INFORMAÇÕES LEIA O README.MD
*/

//Include the SoftwareSerial library
#include "SoftwareSerial.h"

//Create a new software  serial
SoftwareSerial bluetooth(2, 3); //TX, RX (Bluetooth)
  
//const int ledPin = 13; // the pin that the LED is attached to
int LeituraBluetooth;      // a variable to read incoming serial data into

// Canal_1 == RED; Canal_2 == GREEN; Canal_3 == BLUE
#define Canal_1 9
#define Canal_2 10
#define Canal_3 11

// TAMANHO DO VETOR CANAIS
#define MAXCANAL 3

// VETOR QUE DEFINE TODOS OS CANAIS
int Canal[MAXCANAL] = {Canal_1, Canal_2, Canal_3};

// CONTROLE DE BRILHO DOS CANAIS
#define MinBrilho 0
#define MaxBrilho 255
int BrilhoCanal[MAXCANAL];
int BrilhoInicial[MAXCANAL];
int BrilhoFinal[MAXCANAL];

// CONTROLE DE TEMPO
int Tempo = 2000;
int TempoGradual = 100;
int TempoLoop = 500;
int TempoPisca = 150;
int UmSegundo = 1000;

// CONTADORES
int i=0;
int j;
int k;
int n; // NUMERO DE EVENTOS DE UMA PROGRAMAÇÃO


void setup(){
    Serial.begin(9600);
    IniciarPortas();    //INICIA TODAS AS PORTAS EM USO

    //Initialize the software serial
    bluetooth.begin(9600);

    //  TESTE DE CONEXÃO 
    Serial.println("INICIO DO TESTE VISUAL DE CONEXAO");
    for (int contador = 0; contador < 5; contador++){ 
      PiscaPisca();
    }
    Serial.println("TERMINO DO TESTE VISUAL DE CONEXAO");
}

void loop(){
   
  if (bluetooth.available() > 0) {
    LeituraBluetooth = bluetooth.read(); 
    if (LeituraBluetooth == 'L') {
      AcenderLeds();
      bluetooth.println("LEDS: ON");
    }
    if (LeituraBluetooth == 'D') {
      ApagarLeds();
      bluetooth.println("LEDS: OFF");
    }
    if(LeituraBluetooth == 'a'){
      //bluetooth.println("PROGRAMACAO ATIVA");
      Programacao(); 
    }
    if(LeituraBluetooth == 'p'){
      PiscaPisca();  
    }
  }
}  

//  FUNÇÃO QUE INICIA AS PORTAS UTILIZADAS NO CIRCUITO
void IniciarPortas(){
    for(i = 0; i < MAXCANAL; i++ ){
        pinMode(Canal[i], OUTPUT);
    }
}

//  FUNÇÃO QUE ACENDE, DE FORMA GRADUAL, TODOS OS LEDS DO CIRCUITO
void AcenderLeds(){
    for(i = 0; i < MAXCANAL; i++){
        for(j = MinBrilho; j <= MaxBrilho; j++){
            analogWrite(Canal[i], j);
            delay(TempoGradual);
        }
    }
}

// FUNÇÃO QUE ACENDE IMEDIATAMENTE TODOS OS LEDS DO CIRCUITO
void Acender(){
  for(i = 0; i < MAXCANAL; i++){
    digitalWrite(Canal[i], HIGH);
    }
}

//  FUNÇÃO QUE APAGA, DE FORMA GRADUAL, TODOS OS LEDS DO CIRCUITO
void ApagarLeds(){
    for(i = 0; i < MAXCANAL; i++){
        for(j = MaxBrilho; j >= MinBrilho; j--){
            analogWrite(Canal[i], j);
            delay(TempoGradual);
        }
    }
}

// FUNÇÃO QUE APAGA IMEDIATAMENTE TODOS OS LEDS DO CIRCUITO
void apagar(){
  for(i = 0; i < MAXCANAL; i++){
    digitalWrite(Canal[i], LOW);
  }
}

//  FUNÇÃO PISCAPISCA DE RECONHECIMENTO DA CONEXÃO DO ARDUINO
void PiscaPisca(){
    for(i = 0; i < MAXCANAL; i++){
      analogWrite(Canal[i], MaxBrilho);
      delay(TempoPisca);
      analogWrite(Canal[i], MinBrilho);
    }
}

// VALORES INICIAL E FINAL DE INTENSIDADE DE BRILHO QUE CADA CANAL UM DOS CANAIS RECEBE
void ControleCanais(){
  for(i = 0; i < MAXCANAL; i++){
    if(BrilhoInicial[i] > BrilhoFinal[i]){
      Serial.println("BRILHO INICIAL MAIOR QUE O BRILHO FINAL DO CANAL: O CANAL APAGA!");
      for(j = BrilhoInicial[i]; j >= BrilhoFinal[i]; j--){ 
        analogWrite(Canal[i], j);
        delay(TempoGradual);
      }
      delay(TempoLoop);
    } 
    if(BrilhoInicial[i] < BrilhoFinal[i]){
      Serial.println("BRILHO INICIAL MENOR QUE O BRILHO FINAL DO CANAL: O CANAL ACENDE!");
      for(j = BrilhoInicial[i]; j <= BrilhoFinal[i]; j++){ 
        analogWrite(Canal[i], j);
        delay(TempoGradual);
      }
        delay(TempoLoop);
    }
    // INICIAL = FINAL
    if(BrilhoInicial[i] == BrilhoFinal[i]){
      Serial.println("BRILHOS INICIAL E FINAL IDENDICOS: O BRILHO DO CANAL SE MANTEM!");
      analogWrite(Canal[i], BrilhoInicial[i]);
      delay(TempoGradual);
    }
    delay(TempoLoop);
  }  
}

// TESTE INICIAL DO QUE SERÁ A PROGRAMAÇÃO DOS EVENTOS DESTE PROJETO NO FUTURO
void Programacao(){
//  PRIMEIRA FASE
  BrilhoInicial[0] = 0;
  BrilhoInicial[1] = 0;
  BrilhoInicial[2] = 0;

  BrilhoFinal[0] = 64;
  BrilhoFinal[1] = 128;
  BrilhoFinal[2] = 179;

  ControleCanais();
  bluetooth.println("EVENTO 1 ATIVO");
  delay(Tempo);

// SEGUNDA FASE
  Serial.println("INICIO DA SEGUNDA FASE DA PROGRAMACAO");
  BrilhoInicial[MAXCANAL] = BrilhoFinal[MAXCANAL];

  BrilhoFinal[0] = 128;
  BrilhoFinal[1] = 26;
  BrilhoFinal[2] = 128;

  ControleCanais();
  bluetooth.println("EVENTO 2 ATIVO");
  delay(Tempo);

// TERCEIRA FASE
  Serial.println("INICIO DA TERCEIRA FASE DA PROGRAMACAO");
  BrilhoInicial[MAXCANAL] = BrilhoFinal[MAXCANAL];

  BrilhoFinal[0] = 128;
  BrilhoFinal[1] = 102;
  BrilhoFinal[2] = 192;

  ControleCanais();
  bluetooth.println("EVENTO 3 ATIVO");
  delay(Tempo);
}
