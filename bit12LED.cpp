/********************
 bit12LED 
 12LEDs drive library for bitDuino using charlieplexing.

 2014/11/16 @maris_HY
********************/

#include "bit12LED.h"
#include "wiring_private.h"

volatile byte pwmCount = 0;
volatile byte pwmLowPin = 0;
volatile byte pwmHighPin = 0;
byte LED_value[12];
byte LED_pin[4];

#if defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny10__)
ISR(TIM0_COMPA_vect)
//ISR(TIM0_OVF_vect)
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) 
ISR(TIMER1_COMPA_vect)
//ISR(TIMER1_OVF_vect)
#endif
{
  byte i;
  byte led;
  byte pinHIGH;
  if (pwmCount == 0){
    TurnOffAllLEDs();
    pwmLowPin++;
    pwmHighPin += 3;
    if(pwmLowPin>3){
      pwmLowPin = 0;
      pwmHighPin = 0;
    }
    pinMode(LED_pin[pwmLowPin],OUTPUT);
    digitalWrite(LED_pin[pwmLowPin],LOW);
  }

  for(i=0;i<3;i++){
    led = pwmHighPin+i;
    pinHIGH = LED_pin[i+((i<pwmLowPin)?0:1)];
    if((pwmCount == 0) && (LED_value[led] > 0)){
      pinMode(pinHIGH,OUTPUT);
      digitalWrite(pinHIGH,HIGH);
    } else if(LED_value[led] == pwmCount){
      pinMode(pinHIGH,INPUT);
    }
  } 
  pwmCount++;
}

void setLEDValue(byte led,byte value){
  LED_value[led] = value;
}

void TurnOn1LED(byte led){
  byte pinLOW = (led >> 1);
  byte pinHIGH = led - (pinLOW << 1);
  if(pinHIGH >= pinLOW){
    pinHIGH++;
  }
  TurnOffAllLEDs();
  pinMode(LED_pin[pinLOW] ,OUTPUT);digitalWrite(LED_pin[pinLOW ],LOW );
  pinMode(LED_pin[pinHIGH],OUTPUT);digitalWrite(LED_pin[pinHIGH],HIGH);
}

void TurnOffAllLEDs(){
  for(byte i=0;i<4;i++){
    pinMode(LED_pin[i],INPUT);
  }
}

void init12LED(byte pin0,byte pin1,byte pin2,byte pin3){
  LED_pin[0] = pin0;
  LED_pin[1] = pin1;
  LED_pin[2] = pin2;
  LED_pin[3] = pin3;
  TurnOffAllLEDs();
//  start6LED();
}

void start12LED(){
  byte i;
  for(i = 0;i < 12;i++){
    LED_value[i] = 0;
  }
#if defined(__AVR_ATtiny10__)
//  TCCR0A = (1 << WGM00);
  TCCR0A = (1 << WGM01); 
  TCCR0B = ((1 << WGM02) | (1 << CS00));
  
  // enable timer 0 overflow interrupt
  sbi(TIMSK0, TOIE0);

#elif defined(__AVR_ATtiny13__)
  // set timer 0 prescale factor to None
//  TCCR0A = 0;
  TCCR0A = (1 << WGM01);   // CTC mode

//  TCCR0B = 0;
//  sbi(TCCR0B, CS00);
  // enable timer 0 overflow interrupt
//  TCCR0B = (1<<WGM02) | (1<<CS00); // CTC mode , prescalar x1;
  TCCR0B = (1<<CS00); // prescalar x1;

  OCR0A = 254; 

//  sbi(TIMSK0, TOIE0);
  sbi(TIMSK0, OCIE0A);


#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) 
  // set timer 1 prescale factor to None
  TCCR1 = 0;
  sbi(TCCR1, CS00);
  // enable timer 0 overflow interrupt
  sbi(TIMSK, TOIE1);
#endif
  sei();
}

void stop12LED(){
#if defined(__AVR_ATtiny10__)
  cbi(TIMSK0, TOIE0);
#elif defined(__AVR_ATtiny13__)
  // disable timer 0 overflow interrupt
  cbi(TIMSK0, TOIE0);
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) 
  // set timer 1 prescale factor to None
  // disenable timer 0 overflow interrupt
  cbi(TIMSK, TOIE1);
#endif
}


