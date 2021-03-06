#include <LiquidCrystal.h>
#include <stream.h>
#define N 5
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;
int reply;
int print1=1,print2=1;

unsigned long time_cur;
unsigned long time_last;
const unsigned long time_int=60000;
const unsigned long time_count=3000;


int notes[] = {262,294,330,349,300};
int lights[N]={2,3,4,5,6};
int replyold=0;
int i;
int success=1;
int button[N]={0,0,0,0,0};
int light_on[N]={0,0,0,0,0};
int count=0, countbad=0,countnet,failure, state=0,stateold=0,changestate=0;


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2); // cols x rows
  //pinMode(switchPin, INPUT);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Starting from");
  
  for (i=0;i<N;i++) {
    pinMode(lights[i],OUTPUT);
  }
  Serial.begin(9600);

  for (i=0;i<N;i++) {
    digitalWrite(lights[i],LOW);
  }
  randomSeed(analogRead(A5));
  replyold=random(N);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sum1;
  int keyVal = analogRead(A0);
  int flag;
  
  time_cur=millis();

  if(time_cur>time_count & print2==1) {
    lcd.setCursor(0,1);
    lcd.print("now!!!");
    print2=0;
    for (i=0;i<N;i++) {
      if(i==replyold) {
        light_on[i]=1;
        digitalWrite(lights[i], HIGH);
        tone(13, notes[i]); // play a tone
        delay(100);
        noTone(13);
      }
    }    
    success=0;
  } else if (time_cur <= time_int+time_count & time_cur > time_count ) {
    
    delay(10);

    //if(keyVal > 0) {
    if(keyVal > 5) {
      state=1;
    } else {
      state=0;
    }
    
    if( stateold==0 & state==1) {
      changestate=1;
      stateold=state;
    } else {
      changestate=0;
      stateold=state;
    }
      
    
    for (i=0;i<N;i++) button[i]=0;
  
    // STORE WHICH BUTTON HAS BEEN PRESSED ///////////////////////////////////////////
    //Serial.println(keyVal);
    if(keyVal >= 1010 ) { // no resistor
       for (i=0;i<N;i++) {
        if(i==0) {
          button[i]=1;
        } else {
          button[i]=0;
        }
      }
    } else if(keyVal >= 990 && keyVal <= 1005 ) { // 220 ohm
      for (i=0;i<N;i++) {
        if(i==1) {
          button[i]=1;
        } else {
          button[i]=0;
        }
      }
    } else if(keyVal >= 540 && keyVal <= 750 ) { // 4.7 k ohm
      for (i=0;i<N;i++) {
        if(i==2) {
          button[i]=1;
        } else {
          button[i]=0;
        }
      }
    //} else if(keyVal >= 100 && keyVal <= 520 ) { // 10 k ohm
    } else if(keyVal >= 250 && keyVal <= 520 ) { // 10 k ohm
      for (i=0;i<N;i++) {
        if(i==3) {
          button[i]=1;
        } else {
          button[i]=0;
        }
      }
    //} else if(keyVal >= 3 && keyVal <= 20 ) { // 1 M ohm
    } else if(keyVal >= 50 && keyVal <= 200 ) { // 1 M ohm
      for (i=0;i<N;i++) {
        if(i==4) {
          button[i]=1;
        } else {
          button[i]=0;
        }
      }
    }
    //////////////////////////////////////////////////////////////////////////////////
  
    // CHECK TO SEE IF BUTTON PRESSED IS SAME AS LIGHT ON ////////////////////////////
    for (i=0;i<N;i++) {
      if (button[i] > 0)
      {
      if((button[i]==light_on[i]) /*& button[i] > 0*/) {
        success=1;
        break;
      }
      else
      {
        success = 2; // wrong button pressed
      }
      } 
    }  
    //////////////////////////////////////////////////////////////////////////////////

    if (changestate & success==2) {
      success = 0;
      countbad++;
      changestate=0;
      Serial.println("Bad"); //DEBUG
      Serial.println(keyVal);
    }
  
    // TURN A LIGHT ON ///////////////////////////////////////////////////////////////
    if (success == 1) {
      while (reply == replyold) {
        reply = random(N);
      }
      replyold=reply;
      for (i=0;i<N;i++) {
        if(i==reply) {
          light_on[i]=1;
          digitalWrite(lights[i], HIGH);
          tone(13, notes[i]); // play a tone
          delay(100);
          noTone(13);
          count++;
        } else {
          light_on[i]=0;
          digitalWrite(lights[i], LOW);
        }
      }
      success=0;

      Serial.println("Good"); //DEBUG
      Serial.println(keyVal);
    }
    //////////////////////////////////////////////////////////////////////////////////
    
     
  } else if ((time_cur > time_int+time_count) & print1==1) {
        countnet=count-countbad;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Score for ");
        lcd.print(time_int/1000);
        lcd.print(" s:");
        lcd.setCursor(0,1);
        lcd.print(count);
        lcd.print("-");
        lcd.print(countbad);
        lcd.print("=");
        lcd.print(countnet);
        print1=0;
        for (i=0;i<N;i++) {
          digitalWrite(lights[i],LOW);
        }

        // beep when we finish
        for (i=0;i<N;i++) {
          tone(13, notes[i]); // play a tone
          delay(200);
        }
        noTone(13);
  }
}    
