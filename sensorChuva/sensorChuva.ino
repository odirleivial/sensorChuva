
/*
Nome: Sensor de Chuva
Autor: Odirlei Vial Machado
Data Criação: 04/09/2020
Descrição: Projeto com um sensor de chuva e um transmissor de rádio frequência para fechar janela em caso de chuva. 
*/

#define pin_bt 7 // Botão
#define pin_led 13 // Led
#define pinChuva 3 //Pino do sensor de chuva 

#include <RCSwitch.h>
RCSwitch RF433Send = RCSwitch(); // ligar o transmissor de RF no pino 10

int count_chuva = 0;
long tempo_verificacao =  10000; //tempo em ms para fazer a verificação se está chevendo e mandar o comando de fechar a janela
long tempo_normal = 1000; //60000
long tempo_chuva = 1000; //300000
long tempo_inicial = millis();
long blink_inicial = millis();

void setup() {
    pinMode(pinChuva, INPUT);
    pinMode(pin_bt, INPUT_PULLUP);    
    pinMode(pin_led, OUTPUT);        
    digitalWrite(pin_led,LOW);
    
    Serial.begin(9600);

    RF433Send.enableTransmit(10);
    RF433Send.setProtocol(4);
}

void loop() {
  //verifica se atigiu o tempo definido na variável tempo_verificacao, 
  // se o tempo foi atingido envia o sinal para fechar a janela através da função fecha_janela()
  if(tempo_inicial + tempo_verificacao <= millis()){
    if(status_chuva()){
      fn_blink(5);
      fecha_janela();
      tempo_inicial = millis();
      tempo_verificacao = tempo_chuva; //altera o tempo de vefificação para 5 minutos caso esteja chevendo
    }else{
      tempo_verificacao = tempo_normal; //mantem o tempo de verificação em 1 minuto caso não esteja chevendo
    }
  } 
  
  if(!digitalRead(pin_bt)){
    fecha_janela();
    fn_blink(5);
  }
}

bool status_chuva(){
    //função para verificar se o sensor de chuva foi ativado, 
    // faz 10 leituras e se a metade ou mais indicar chuva retorna verdadeiro.

    for(int i = 0; i <= 10; i++ ){
        if (!digitalRead(pinChuva)){
            count_chuva++;
        } 
    }

    if(count_chuva >= 5){
        count_chuva = 0;
        return true;
    } else{
        count_chuva = 0;
        return false;
    }
}

void fecha_janela(){
    // Envia código do botão de fechar a janela
    RF433Send.sendQuadState("11F0001Q1Q10FF0F0101");
          Serial.println("FECHA!");


}

void fn_blink(int qtd){
  int aux = 0;
  while (aux < qtd){
    if (blink_inicial + 150 <= millis()){
      blink_inicial = millis();
      
      if(!digitalRead(pin_led)){
        digitalWrite (pin_led,HIGH);
      }else{
        digitalWrite (pin_led,LOW);
        aux++;
      }
    }
  }
  digitalWrite (pin_led,LOW);
}
