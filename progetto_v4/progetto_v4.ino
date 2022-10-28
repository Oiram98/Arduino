
#include <LiquidCrystal.h>
const int rs=7,en=8,d4=9,d5=10,d6=11,d7=12;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
const int easy_mode = 300 ;  //costanti settaggio difficoltà
const int medium_mode = 600 ;
const int hard_mode = 900 ;
int pin_botton = 2 ; //
int led_rosso = 3 ;
int led_verde = 5 ;
int led_blu = 6 ;
int buzzer_pin = 13;
#define JOYSTICK_X A5 
#define JOYSTICK_Y A4 
int JOYSTICK_BUTTON = 0 ;
int best_easy=0;
int best_medium=0;
int best_hard=0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(pin_botton , INPUT );
  pinMode(led_rosso , OUTPUT) ;
  pinMode(led_verde , OUTPUT) ;
  pinMode(led_blu ,OUTPUT) ;
  pinMode(JOYSTICK_BUTTON,INPUT_PULLUP); //Resistenza interna pull-up attivata
  pinMode(buzzer_pin ,OUTPUT);
}

//Dichiarazioni funzioni
int settings () ; //ritorna il valore flag da dare in input alla funzione 'game'
int game(int flag); // funzione che gestisce il gioco vero e proprio . Ritorna il punteggio del giocatore.

void loop() {
  lcd.clear(); // Ripulisce lo schermo ad ogni riavvio dello skatch
  lcd.setCursor(0,0);
  lcd.print("Set Level and");
  lcd.setCursor(0,1);   
  lcd.print("press Button");
  delay(2000);
  lcd.clear();
  int flag = settings();
  delay(2000);
  int punteggio = game(flag);
  tone(buzzer_pin,1000);
  delay(1500);
  noTone(buzzer_pin);
  while((digitalRead(pin_botton)) != HIGH ){
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("GAME OVER");
      lcd.setCursor(0,1);
      lcd.print(punteggio);
      lcd.setCursor(6,1);
      switch(flag) {
        case 1: {
          if (punteggio>best_easy) {
            best_easy = punteggio ;
          }
          lcd.print("Goal:5");
          delay(4000);
          lcd.setCursor(6,1);
          lcd.print("*Best:");
          lcd.setCursor(12,1);
          lcd.print(best_easy);
          delay(4000);
          break;
        }
        case 2: {
          if (punteggio>best_medium) {
            best_medium = punteggio ;
          }
          lcd.print("Goal:15");
          delay(4000);
          lcd.setCursor(6,1);
          lcd.print("*Best:");
          lcd.setCursor(12,1);
          lcd.print(best_medium);
          delay(4000);
          break;
        }
        case 5: {
          if (punteggio>best_hard) {
            best_hard = punteggio ;
          }
          lcd.print("Goal:25");
          delay(4000);
          lcd.setCursor(6,1);
          lcd.print("*BEST:");
          lcd.setCursor(12,1);
          lcd.print(best_hard);
          delay(4000);
          break;
        }
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Press Button");
      lcd.setCursor(0,1);
      lcd.print("to restart");
      delay(2000);
    }
}

int settings() {
  int flag = 0; 
  while((digitalRead(pin_botton)) != HIGH ) { /*Settare difficoltà.
                                                Fintantoché non il bottone non viene premuto , rimani nel loop.*/
    Serial.println(analogRead(A0)); // Scrivi in seriale il valore analogico letto dal potenziometro ad ogni ciclo.
    delay(200);
    if(((analogRead(A0)) < easy_mode )){ //se il valore analogico letto è minore della costante easy_mode ,
      lcd.clear();                       //rendiamo blu il led RGB , stampiamo su schermo LCD e diamo un certo valore al flag.
      lcd.setCursor(0,0);
      analogWrite(led_verde,0);
      analogWrite(led_blu,255);
      analogWrite(led_rosso,0);
      lcd.print("EASY MODE");
      flag = 1 ;
       }
    if(((analogRead(A0)) > easy_mode) && (((analogRead(A0)) < medium_mode))) { //se il valore analogico letto è tra easy_mode e medium_mode
      lcd.clear();                                                            // rendiqamo verde il led RGB,stampiamo su schermo LCD e diamo
      lcd.setCursor(0,0);                                                     //un certo valore al flag.
      analogWrite(led_verde,255);
      analogWrite(led_blu,0);
      analogWrite(led_rosso,0);
      flag = 2;
      lcd.print("MEDIUM MODE");
      }
    if((analogRead(A0)) > hard_mode ){  
      lcd.clear();     //Necessario per evitare che si verifichino sovrascritture.
      lcd.setCursor(0,0);
      analogWrite(led_verde,0);
      analogWrite(led_blu,0);
      analogWrite(led_rosso,255);
      flag = 5 ;
      lcd.print("HARD MODE");
      }
    }

    lcd.clear();
    //Spegnere LED RGB
    analogWrite(led_verde,0);
    analogWrite(led_blu,0);
    analogWrite(led_rosso,0);
    while(digitalRead(JOYSTICK_BUTTON) != false) { //Attivando la resistenza di pull-up , che è 
      lcd.setCursor(0,0);                           //collegata ad un polo positivo , abbiamo un
      lcd.print("Press Joystick");                  //valore HIGH quando il pulsante non è
      lcd.setCursor(0,1);                           // schiacciato ..
      lcd.print("Button to start");
    }
    return flag ;
}

int game(int flag){
  int score = 0 ;
  int tempo_limite = 1500 ; 
  int tempo_trascorso = 0; 
  int inizio=(int)random(4); //Inizio casuale
  int x=flag ;
  boolean up =  false ;
  boolean down = false ;
  boolean right = false ;
  boolean left = false ;
  
  
  while (tempo_limite > 0) { //Ad ogni azione giusta compiuta dal giocatore rimaniamo in uno switch,
                        // finchè non si azzera il tempo_limite o impieghiamo troppo tempo nel rispondere.
        tempo_trascorso = 0;
        //Serial.println(tempo_limite);
        switch(inizio) {
        
        case 0 : 
          left = true ;
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("LEFT");
          while ((analogRead(JOYSTICK_X)) < 900){
            if (((analogRead(JOYSTICK_X) < 100) && (right == false))|| ((analogRead(JOYSTICK_Y) > 900 ) && (up == false)) || ((analogRead(JOYSTICK_Y) < 100 ) && (down == false))){
              tempo_limite=0;
            }
            if ((analogRead(JOYSTICK_X)<650) && (analogRead(JOYSTICK_X)>450) && (analogRead(JOYSTICK_Y)<650) && (analogRead(JOYSTICK_Y)>450)) {
              up=false;
              right=false;
              down=false;
            } 
            tempo_limite = tempo_limite - 2 - tempo_trascorso  ;  
            tempo_trascorso = tempo_trascorso + x ;
            delay(100);                                   
            }
          if (tempo_limite > 0 && tempo_trascorso <= 50 ) { 
            score ++ ;
          }
          else tempo_limite = 0;
          while (inizio == 0 ) { //mai 2 volte la stessa direzione indicata
            inizio = millis() % 4 ;
          }
          break;
        
          
        
          
        case 1 :
          right=true;
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("RIGHT");
          while ((analogRead(JOYSTICK_X)) > 100){
            if (((analogRead(JOYSTICK_X) > 900) && (left == false))||((analogRead(JOYSTICK_Y) > 900 ) && (up == false)) || ((analogRead(JOYSTICK_Y) < 100 ) && (down == false))){
              tempo_limite=0;
            }
            if ((analogRead(JOYSTICK_X)<650) && (analogRead(JOYSTICK_X)>450) && (analogRead(JOYSTICK_Y)<650) && (analogRead(JOYSTICK_Y)>450)) {
              up=false;
              left=false;
              down=false;
            } 
            tempo_limite = tempo_limite - 2 - tempo_trascorso ;  
            tempo_trascorso = tempo_trascorso + x ;
            delay(100);                                   
            }
          if (tempo_limite > 0 && tempo_trascorso <= 50 ) { 
            score ++ ;
          }
          else tempo_limite = 0;
          while (inizio == 1 ) { //mai 2 volte la stessa direzione indicata
            inizio = millis() % 4 ;
          }
          break;
        
        
          
        
        case 2 :
          down=true;
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("DOWN");
          while ((analogRead(JOYSTICK_Y)) > 100){
            if (((analogRead(JOYSTICK_X) < 100) && (right == false)) || ((analogRead(JOYSTICK_X) > 900 ) && (left == false)) || ((analogRead(JOYSTICK_Y) > 900 ) && (up == false))){
              tempo_limite=0;
            }
            if ((analogRead(JOYSTICK_X)<650) && (analogRead(JOYSTICK_X)>450) && (analogRead(JOYSTICK_Y)<650) && (analogRead(JOYSTICK_Y)>450)) {
              up=false;
              right=false;
              left=false;
            }
            tempo_limite = tempo_limite - 2 - tempo_trascorso ;  
            tempo_trascorso = tempo_trascorso + x ;
            delay(100);                                   
            } 
          if (tempo_limite > 0 && tempo_trascorso <= 50 ) { 
            score ++ ;
          }
          else tempo_limite = 0;
         while (inizio == 2 ) { //mai 2 volte la stessa direzione indicata
            inizio = millis() % 4 ;
          }
          break;
        
        

          
        case 3  :
          up=true;
          lcd.clear();
          lcd.setCursor(6,0);
          lcd.print("UP");
          while ((analogRead(JOYSTICK_Y)) < 900){
            if (((analogRead(JOYSTICK_X) < 100) && (right == false))|| ((analogRead(JOYSTICK_X) > 900 ) && (left == false)) || ((analogRead(JOYSTICK_Y) < 100 ) && (down == false))){
              tempo_limite=0;
            }
            if ((analogRead(JOYSTICK_X)<650) && (analogRead(JOYSTICK_X)>450) && (analogRead(JOYSTICK_Y)<650) && (analogRead(JOYSTICK_Y)>450)) {
              left=false;
              right=false;
              down=false;
            }
            tempo_limite = tempo_limite - 2 -tempo_trascorso ;  
            tempo_trascorso = tempo_trascorso + x ;
            delay(100);                                   
            } 
          if (tempo_limite > 0 && tempo_trascorso <= 50 ) { 
            score ++ ;
          }
          else tempo_limite = 0;
          while (inizio == 3 ) { //mai 2 volte la stessa direzione indicata
            inizio = millis() % 4 ;
          }
          break;
       
        }
        //Serial.println(tempo_limite);
      }
      return score ;
}
