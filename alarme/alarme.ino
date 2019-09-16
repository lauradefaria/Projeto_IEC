#include <Keypad.h> byte
#define buzzer 3
#define presenca 13
#define motor1 2
#define motor2 7
#define botao A0
const byte linha = 4; 
const byte coluna = 4;
byte valorBotao=0; 
byte flag=0;
byte vezes=0;
byte ativa;
byte valorSensor;
char hexaKeys[linha][coluna] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte linPins[linha] = {12, 4, 5, 6}; // pinos das linhas do teclado
byte colPins[coluna] = {8, 9, 10, 11}; //pinos das colunas do teclado

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), linPins, colPins, linha, coluna); 

int abrirDentro(int valorBotao){  //abre a porta pelo lado de dentro(pressionando o botão)
      digitalWrite(motor1,HIGH);
      digitalWrite(motor2,LOW);
      delay(5000);
      digitalWrite(motor1,LOW);
      digitalWrite(motor2,LOW);
      delay(5000);
      digitalWrite(motor1,LOW);
      digitalWrite(motor2,HIGH);
      return 0;
}

int alarme(int valorSensor){
  /*feito pelo sensor de presença
  ativa a sirene e avisa ao dono
  a sirene só vai ser desligada quando o dono sinalizar no aplicativo?*/
  int i;
  if(valorSensor!=0){
    while(/*dono nao desliga o alarme e destrava o portao*/){
      digitalWrite(motor1,LOW);
      digitalWrite(motor2,LOW);
      for(i=0;i<256;i++){
        analogWrite(buzzer,i);
      }
      for(i=256;i>0;i--){
        analogWrite(buzzer,i);
      }
      delay(15000);
     }
  }
    return 0;                   //reinicia o valor do botão
}

void setup(){
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  pinMode(presenca, INPUT);
  pinMode(botao,INPUT);
  
}
  
void loop(){
  valorSensor = digitalRead(presenca); // lê o valor do sensor, se for 1 ele chama a função
  if(valorSensor==HIGH){
    valorSensor=alarme(valorSensor);
  }

  valorBotao= digitalRead(botao);     // lê o valor do botão, se for 1 ele chama a função
  if(valorBotao==HIGH){
    valorBotao= abrirDentro(valorBotao);
  }
  
  while(flag<3 && ativa!=5){
    
    digitalWrite(motor1,LOW);    //deixa os motores parados
    digitalWrite(motor2,LOW);
    vezes=0;
    
    valorSensor = digitalRead(presenca);  // lê o valor do sensor, se for 1 ele chama a função
    if(valorSensor==HIGH){
      valorSensor=alarme(valorSensor);
    }

    valorBotao= digitalRead(botao);     // lê o valor do botão, se for 1 ele chama a função
    if(valorBotao==HIGH){
      valorBotao= abrirDentro(valorBotao);
    }
    
    while(vezes<5){
        valorSensor = digitalRead(presenca);    // lê o valor do sensor, se for 1 ele chama a função
        if(valorSensor==HIGH){
           valorSensor=alarme(valorSensor);
        }
        
        valorBotao= digitalRead(botao);     // lê o valor do botão, se for 1 ele chama a função
        if(valorBotao==HIGH){
          valorBotao= abrirDentro(valorBotao);
        }
        
        char customkey = customKeypad.waitForKey();  //espera o usuário digitar no teclado
        if(customkey=='1' && vezes==0){
          ativa++;
          Serial.println("ATIVOU 1");
        }
        
        else{
          if(customkey=='5' && vezes==1){
            ativa++;
            Serial.println("ATIVOU 5");
          }
          
          else{
            if(customkey=='8' && vezes==2){
              ativa++;
              Serial.println("ATIVOU 8");
            }
            
            else{
              if(customkey=='6' && vezes==3){
                ativa++;
                Serial.println("ATIVOU 6");
              }
              
              else{
                if(customkey=='D' && vezes==4){
                  ativa++;
                  Serial.println("ATIVOU D");
                }
                else{
                  ativa=0;
                  Serial.println("NEGADO");
                }
              }
            }
          }
        }
        vezes++;
    }
    if(ativa==5){
      //porta abre
      digitalWrite(motor1,HIGH);  //motor gira para abrir, para por um período de 5 segundos
      digitalWrite(motor2,LOW);           // e gira pra fechar,depois fica parado
      delay(5000);
      digitalWrite(motor1,LOW);
      digitalWrite(motor2,LOW);
      delay(5000);
      digitalWrite(motor1,LOW);
      digitalWrite(motor2,HIGH);
      delay(5000);
      digitalWrite(motor1,LOW);
      digitalWrite(motor2,LOW);
      flag=0;
      ativa=0;
      Serial.println("PORTA ABRE");
      break;
    }
    else{
      //buzzer toca
      digitalWrite(buzzer,HIGH);
      delay(1000);
      digitalWrite(buzzer,LOW);
      flag++;
      Serial.println("BUZZER TOCA");
    }
  }
  if(flag==3){
    //manda msg pra o dono
    //espera um comando pra desligar e iguala flag a 0
    flag=alarme(flag);
    Serial.println("MENSAGEM DONO");
  }
  
}
