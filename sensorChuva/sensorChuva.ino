
/*
Nome: Sensor de Chuva
Autor: Odirlei Vial Machado
Data Criação: 04/09/2020
Descrição: Projeto com um sensor de chuva e um transmissor de rádio frequência para fechar janela em caso de chuva. 
*/

#define pinChuva 3 //Pino do sensor de chuva 
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch(); // ligar o transmissor de RF no pino 2


int count_chuva = 0;
long tempo_verificacao =  60000; //tempo em ms para fazer a verificação se está chevendo e mandar o comando de fechar a janela
long tempo_inicial = millis();

// Dados do controle remoto:
//  botão de fechar: Received 154058277 / 28bit Protocol: 6
//  botão de abrir:  Received 154058261 / 28bit Protocol: 6
//  botão de pausa:  Received 154058293 / 28bit Protocol: 6
long codigo_controle = 154058277;
int  tamanho_pacote = 28; 
int protocolo_controle = 6;


void setup() {
    pinMode(pinChuva, INPUT);
    Serial.begin(9600);

    mySwitch.enableTransmit(10);
    mySwitch.setProtocol(protocolo_controle);
    mySwitch.setRepeatTransmit(5);
}

void loop() {
    //verifica se atigiu o tempo definido na variável tempo_verificacao, 
    // se o tempo foi atingido envia o sinal para fechar a janela através da função fecha_janela()
    if (tempo_inicial + tempo_verificacao >= millis()){
        tempo_inicial = millis();
        if (status_chuva()){
            fecha_janela();
        }
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
    mySwitch.send(codigo_controle, tamanho_pacote);

}
