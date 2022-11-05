#include <LiquidCrystal_I2C.h>
#include <Wire.h> //para manejar bus i2c


//---Commons
#define BUZZER 8
#define FROM 100
#define UNTIL 200
#define TRUE 1
#define FALSE 0
#define STONES 6

//----Stones
#define RED_LED 6 //soul = gamora
#define GREEN_LED 5 //space = loki
#define YELLOW_LED 4 //reality = the colector
#define BLUE_LED 3 //power = star lord
#define WHITE_LED 2 //time = Dr Strange
#define ORANGE_LED A1 //mind = vision

//----LCD------------
#define RS 8
#define E 7
#define D4 10
#define D5 11
#define D6 12
#define D7 13

#define START_BTN 9
#define FIGHT_BTN A3


//creamos el lcd
LiquidCrystal_I2C lcdHero(34,16,2);
LiquidCrystal_I2C lcdFight(35,16,2);

typedef struct{
  char name[50];
  long power;
  int stone;
}Hero;


typedef struct{
  char name[50];
  long power;
  int stones[6];
}Enemy;

Hero heroes[6];
Enemy enemy;

void setup() {  
  initLedsBttns();
  
  lcdHero.init();
  lcdHero.backlight();  
  lcdFight.init();
  lcdFight.backlight();
  
  Serial.begin(9600);
}

void initLedsBttns(){
  pinMode(BUZZER,OUTPUT);
  
  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(YELLOW_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);
  pinMode(WHITE_LED,OUTPUT);
  pinMode(ORANGE_LED,OUTPUT);
  
  pinMode(START_BTN,INPUT);
  pinMode(FIGHT_BTN,INPUT);
}

void loop() {  
  int srtBtn = digitalRead(START_BTN); //press=1 leave=0  
  delay(300);
  
  if(srtBtn){
  	startGame();  
    
    for(int i=0;i<6;i++){
      showHero(i);
      startFight(i);
      cleanFightLcd();
    }
    
    checkFinalResult();
    resetGame();
    turnOfLeds();
  }
}

void stoneWin(){  
  tone(BUZZER,329,100);  
  tone(BUZZER,261,100);
  delay(300);  
}
void thanosWin(){
  for(int i=0;i<5;i++){
  	stoneWin();
  }
}

void startGame(){   
  initEnemy();
  initHeroes();    
}

void resetGame(){
  lcdHero.clear();
  lcdFight.clear();  
}

void startFight(int i){
  int fgtBtn = FALSE;
  int val;
  
  while(!fgtBtn){    
    val = analogRead(A0);  
    int thnPw = map(val,0,1023,90,190);
      
    enemy.power = thnPw;
    showThanosPower();    
    
    fgtBtn = digitalRead(FIGHT_BTN); //press=1 leave=0
    delay(300);    
  }
  Serial.println("--Fight--");
  comparePower(i);
}

void comparePower(int i){
  Hero hero = heroes[i];
  if(hero.power > enemy.power){
  	showFightMssg("Thanos lose"," :(");
  }else{
    stoneWin();  	    
    enemy.stones[i]=hero.stone;
    turnLedByPinId(enemy.stones[i]);    
    showFightMssg("Thanos win"," :)");
  } 
}

void checkFinalResult(){
  int stnsLng = getStonesLng();
  if(stnsLng==STONES){
    thanosWin();
    blinksLeds();
    showFightMssg("I don't want to "," go Mr Stark.");    
  }else{
    showFightMssg("Thanos lose "," the WAR..");  
    turnOfLeds();
  }
}

int getStonesLng(){  
  int i=0;
  while(enemy.stones[i++]!=0);  
  return (i-1);
}

int getRandomEnemyPw(){
  int rand = random(FROM, UNTIL);
}

void initHeroes(){
  strcpy(heroes[0].name,"Gamora");
  heroes[0].power = getRandomEnemyPw();
  heroes[0].stone = RED_LED;
  
  strcpy(heroes[1].name,"Loki");
  heroes[1].power = getRandomEnemyPw();
  heroes[1].stone = GREEN_LED;
  
  strcpy(heroes[2].name,"TheCollector");
  heroes[2].power = getRandomEnemyPw();
  heroes[2].stone = YELLOW_LED;
  
  strcpy(heroes[3].name,"Star Lord");
  heroes[3].power = getRandomEnemyPw();
  heroes[3].stone = BLUE_LED;
  
  strcpy(heroes[4].name,"Dr Strange");
  heroes[4].power = getRandomEnemyPw();
  heroes[4].stone = WHITE_LED;
  
  strcpy(heroes[5].name,"Vision");
  heroes[5].power = getRandomEnemyPw();
  heroes[5].stone = ORANGE_LED;
}

void initEnemy(){
  strcpy(enemy.name,"Thanos");
  enemy.power = 0;
  for(int i=0;i<6;i++){
    enemy.stones[i]=0;
  }	
}

void turnLedByPinId(int pnId){
  digitalWrite(pnId,HIGH);
}

void turnOfLeds(){
  digitalWrite(RED_LED,LOW);  
  digitalWrite(GREEN_LED,LOW);  
  digitalWrite(YELLOW_LED,LOW);  
  digitalWrite(BLUE_LED,LOW);  
  digitalWrite(WHITE_LED,LOW);  
  digitalWrite(ORANGE_LED,LOW);
}

void blinksLeds(){
  for(int i=0;i<4;i++){
    digitalWrite(RED_LED,LOW);
    delay(200);
    digitalWrite(GREEN_LED,LOW);
    delay(200);
    digitalWrite(YELLOW_LED,LOW);
    delay(200);
    digitalWrite(BLUE_LED,LOW);
    delay(200);
    digitalWrite(WHITE_LED,LOW);
    delay(200);
    digitalWrite(ORANGE_LED,LOW);
    
    digitalWrite(RED_LED,HIGH);
    delay(200);
    digitalWrite(GREEN_LED,HIGH);
    delay(200);
    digitalWrite(YELLOW_LED,HIGH);
    delay(200);
    digitalWrite(BLUE_LED,HIGH);
    delay(200);
    digitalWrite(WHITE_LED,HIGH);
    delay(200);
    digitalWrite(ORANGE_LED,HIGH);
  }

}

void showHero(int i){
  lcdHero.setCursor(0,0);
  lcdHero.print("hero:");  
  lcdHero.setCursor(5,0);
  lcdHero.print(heroes[i].name);
  lcdHero.print("           ");
    
  lcdHero.setCursor(0,1);
  lcdHero.print("stone:");
  lcdHero.setCursor(6,1);
  lcdHero.print(heroes[i].stone);
  lcdHero.print(" pw:");
  lcdHero.print(heroes[i].power);
    
  delay(2000);
}

void showThanosPower(){
  lcdFight.setCursor(0,0);
  lcdFight.print("Thanos choose PW:");
  lcdFight.setCursor(0,1);
  lcdFight.print(enemy.power);
  lcdFight.print("            ");
  delay(1000);
}

void showFightMssg(char* row1, char* row2){
  lcdFight.setCursor(0,0);
  lcdFight.print(row1);
  lcdFight.print("                 ");
  
  lcdFight.setCursor(0,1);
  lcdFight.print(row2);
  lcdFight.print("                 ");
  delay(2000);
}

void cleanFightLcd(){
  lcdFight.clear();
} 
