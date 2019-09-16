#include <Wire.h>                    //inclui as bibliotecas
#include <LiquidCrystal_I2C.h> 
#include <SPI.h>
#include <MFRC522.h>
#define ledVerde 2                   //define o pino do led verde
#define ledVermelho 6                //define o pino do led vermelho
#define SS_PIN 10 
#define RST_PIN 9
#define botao 8                      //define o botão (para quando deixa a correspondência
#define botaoG 4                     //define o botão (para gravar outro cartão)
String senhaAtual="";                    //string que contém o código para abrir o correio
int flag=0;                           //variável que altera o seu valor quando um cartão é gravado
int caixa=0;                          //variável que informa se há correspondência ou não
int valorGravar=0;                    //status do botão para gravar outro código
int valorBotao1=0;                    //status do botão atual
int valorBotao2=1;                    //status do botão anteriormente
LiquidCrystal_I2C lcd(0x3F,16,2);     //declara o display LCD
MFRC522 mfrc522(SS_PIN, RST_PIN);  
MFRC522::MIFARE_Key key;

String modo_gravacao(String senha){         //função para gravar outro código de cartão, substituindo o antigo
  while(1){
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      continue;
    }
  
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      continue;
    }
    senha = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)                 //percorre a leitrua da UDI do cartão e armazena
    {                                                           //na string senha
       senha.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    flag=1;                                                     //iguala flag a 1, simbolizando que a senha é de um
    break;                                                      //cartão gravado
  }
  return senha;                                                 //retorna a string com a UDI do cartão
}

void setup() 
{ 
  Serial.begin(9600);                                           // Inicia a serial
  SPI.begin();                                                  // Inicia  SPI bus
  mfrc522.PCD_Init();                                           // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  lcd.begin(16, 2);                                             //Define o número de colunas e linhas do LCD:  
  lcd.init();
  lcd.setCursor(0,0);
  lcd.print("");
  lcd.backlight();
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(botaoG,INPUT);
  pinMode(botao, INPUT);
  senhaAtual="20be2419";                                       //define a senha padrão (o cartão original do usuário)

}
 
void loop()
{ 
  digitalWrite(ledVerde, LOW);                           //ambos os leds ficam apagados
  digitalWrite(ledVermelho,LOW);  
  
  valorGravar = digitalRead(botaoG);

  if(valorGravar==HIGH){                                //se o valor recebido pelo botão(gravar) for 1, ele 
    senhaAtual = modo_gravacao(senhaAtual);            //       chama a função "modo_gravação"
    delay(2000);
  }
  
  valorBotao1= digitalRead(botao);

  if(valorBotao1==HIGH){                     //se o valor do botão atual for diferente do anterior
    caixa=1;                                           // ele iguala a variável caixa a 1
  }
  
  valorBotao2=valorBotao1;
  
  if(caixa==1){                                         //se o valor de caixa for diferente de zero
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CORRESPONDENCIA");                   //mostra no display que há correspondência
  }
  else{                                               //se o valor de caixa for diferente de zero
    lcd.setCursor(6, 0);
    lcd.print("SEM");
    lcd.setCursor(0, 1);
    lcd.println("CORRESPONDENCIA");                  //mostra no display que não há correspondência
  }
  if ( ! mfrc522.PICC_IsNewCardPresent())            //verifica a presença de algum cartão lido
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  String chave = "";                                 //declara a string 'chave' para armazenar a UDI do cartão lido
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     chave.concat(String(mfrc522.uid.uidByte[i], HEX));    //aramzena cada termo na string chave
  }
  
  if(flag=0){
    chave.toUpperCase();
  }
  
  if (chave.substring(0) == senhaAtual)                //se todos os termos da string chave forem iguais ao da senha 
  {                                                    // do cartão gravado
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho,LOW);
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);                                 //mostra no LCD a mensagem de "acesso permitido"
    lcd.print("Acesso permitido");                      //  e abre a caixa de correio
    caixa=0;                                            //a variável fica = 0, mostrando como se não houvesse correspondência
    delay(2000);                                   
    mensageminicial();                                 //chama a função
  }

  if(chave == senhaAtual && flag==0){                  //se a string chave for igual a senha do cartão original
    digitalWrite(ledVerde,HIGH);
    digitalWrite(ledVermelho,LOW);
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);                                  //mostra no LCD a mensagem de "acesso permitido"
    lcd.print("Acesso permitido");                       //  e abre a caixa de correio
    caixa=0;                                             //a variável fica = 0, mostrando como se não houvesse correspondência
    delay(2000);
    mensageminicial();                                   //chama a função
  }
 
  else if (chave.substring(0) != senhaAtual && chave.substring(0) !=0) //caso o código do cartão detectado seja diferente
  {                                                                    //da senha atual e não seja iguala zero
    digitalWrite(ledVerde,LOW);
    digitalWrite(ledVermelho,HIGH);                                    //acende o led vermelho
    Serial.println();
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Acesso Negado");                                     //mostra no LCD a mensagem de "acesso negado"
    delay(3000);                                                      //e não abre a caixa de correio
    mensageminicial();                                                //chama a função
  }
} 
 
void mensageminicial()                                                //mensagem para aparecer no monitor serial
{
  lcd.clear();
 Serial.println("Aproxime o seu cartão");
 Serial.println();
}
