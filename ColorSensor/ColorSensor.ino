// Define os pinos dos Leds 
int ledArray[] = {2,3,4};

// usamos um boolean para saber se o sistema foi configurado
boolean balanceSet = false;

//guarda os valores para as cores detectadas
int red = 0;
int green = 0;
int blue = 0;

//guarda nosso array de cores
float colourArray[] = {0,0,0};
float whiteArray[] = {0,0,0};
float blackArray[] = {0,0,0};


//guarda a media da leitura
int avgRead;

void setup(){
 
  //configura a saida dos leds
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
 
  //configura velocidade da porta serial
  Serial.begin(9600);
 
}

void loop(){

    checkBalance();
    checkColour();
    printColour();
}

void checkBalance(){
  //checa se o sistema foi configurado
  if(balanceSet == false){
    setBalance();
  }
}


void setBalance(){
  //configura o branco
  //Serial.println("Coloque o cartao BRANCO sobre o sensor e aguarde");
   delay(5000);                              //ESPERA 5 SEGUNDOS PARA O USUARIO COLOCAR A COR SOBRE O SENSOR
  //escaneia a a amostra branca.
  //passa por cada cor, fazendo a leitura, e guarda a base da leitura para cada cor vermelho, verde, e azul quando expostas ao branco e guarda no array branco
  //Serial.println("Calibrando...");
  for(int i = 0;i<=2;i++){
     digitalWrite(ledArray[i],HIGH);
     delay(100);
     getReading(5);          //faz cinco leituras, para melhorar o resultado, mas apenas uma leitura ja seria o suficiente.
     whiteArray[i] = avgRead;
     digitalWrite(ledArray[i],LOW);
     delay(100);
  }

   //configurando o preto
   //Serial.println("Coloque o cartao PRETO sobre o sensor e aguarde");
    delay(5000);              //ESPERA 5 SEGUNDOS PARA POR A COR PRETA SOBRE O SENSOR
  //comeca a leitura para cada cor e seta a base das leituras para as cores red, green, e blue quando expostas ao preto e guarda no array preto
  //Serial.println("Calibrando...");
  for(int i = 0;i<=2;i++){
     digitalWrite(ledArray[i],HIGH);
     delay(100);
     getReading(5);
     blackArray[i] = avgRead;
     digitalWrite(ledArray[i],LOW);
     delay(100);
  }
   //seta que foi configurado para nao fazer isso denovo
  balanceSet = true;
  Serial.println("Calibrado!!");
  delay(5000);     //Espera mais 5 segundos
}
  
  
void checkColour(){
    for(int i = 0;i<=2;i++){
     digitalWrite(ledArray[i],HIGH);  //liga os leds vermelho, azul e amarelo, um de cada vez
     delay(100);                      //pausa para o CdS estabilizar, pois sao meio lentos
     getReading(5);                  //pega a leitura X vezes
     colourArray[i] = avgRead;        //seta a cor lida para o array
     float greyDiff = whiteArray[i] - blackArray[i];                   //O maior retorno possivel menos o menor retorno nos da uma area de valores entre eles
     colourArray[i] = (colourArray[i] - blackArray[i])/(greyDiff)*255; //A leitura menos o menor valor dividido pela area de valores multiplicado por 255, nos da um valor entre 0 e 255 representando o o valor do reflexo da cor que foi exposta, ou que foi escaneada
     digitalWrite(ledArray[i],LOW);   //desliga o LED atual      
     delay(100);
  }
}

void getReading(int times){
  int reading;
  int tally=0;
  //tira quantas leituras foi requisitada
  for(int i = 0;i < times;i++){
     reading = analogRead(0);
     tally = reading + tally;
     delay(10);
  }
//calcula a media das leituras
avgRead = (tally)/times;
}

//mostra o retorno para cada um dos valores R,G,B na saida serial, o mesmo eh pego pelo processing que mostra o retorno em forma de cores na tela.
void printColour(){
  Serial.print("R = ");
  Serial.println(int(colourArray[0]));
  Serial.print("G = ");
  Serial.println(int(colourArray[1]));
  Serial.print("B = ");
  Serial.println(int(colourArray[2]));
  //delay(2000);
}
