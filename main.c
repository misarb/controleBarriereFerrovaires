// Interfacing  PIC16F84A with HC-SR04 ultrasonic sensor CCS C code
#include <16F84A.h>
#fuses HS,NOWDT,PUT,NOPROTECT
#use delay(clock = 1M)
#use fast_io(A)



unsigned int8 count;
unsigned int16 i, distance;
#INT_TIMER0
void timer0_isr(){
  count++;
  clear_interrupt(INT_TIMER0);
}
int1 wait_sensor(){
  i = 0;
  set_timer0(0);
  count = 0;                             // Reset Timer0
  while(!input(PIN_A1) && (i < 1000))
    i = count * 256 + get_timer0();
  if(i > 990)
    return 0;
  else
    return 1;
}
unsigned int16 get_distance(){
  i = 0;
  set_timer0(0);
  count = 0;
  while(input(PIN_A1) && (i < 25000))
    i = count * 256 + get_timer0();
  return i;
}
void ledController(int a,int b,int c){
        output_bit(PIN_B4, a); // LED RED 
        output_bit(PIN_B5, b); //LED YELLOW 
        output_bit(PIN_B6, c); //ED GREEN 
        }
 void servoDegreRotation(int dt1, int dt2 ){
         output_high(PIN_B0);
         delay_us(dt1);
         output_low(PIN_B0);
         delay_us(dt2);
 
 }
void main(){
  output_a(0);
  set_tris_a(2);                                      // Configure RA1 as input
  clear_interrupt(INT_TIMER0);
  enable_interrupts(GLOBAL);
  enable_interrupts(INT_TIMER0);
  setup_timer_0 (T0_INTERNAL | T0_DIV_2);             // Configure Timer0 module
  while(TRUE){
    // Send 10us pulse to HC-SR04 Trigger pin
    output_high(PIN_A0);
    delay_us(10);
    output_low(PIN_A0);
    // Read pulse comes from HC-SR04 Echo pin
    if(wait_sensor()) {
      distance = get_distance();
        distance = i/58;                              // Calculate the distance
        set_tris_b(0x00); // TRIS B AS AN output
        if(distance < 20){ // the BARE CLOSED AND RED LED LEIGHT 0 Degree
        ledController(1,0,0);
        servoDegreRotation(900,19100);
       //output_high(PIN_B7);
      //delay_us(900);
      // output_low(PIN_B7);
      // delay_us(19100);
        }else  if(distance == 25){ // BE ATTANITION SO CLOSE FOR THE SECURITY THE BARE CLOSED 0Degree
        ledController(0,1,0);
        servoDegreRotation(900,19100);
//!        output_high(PIN_B7);
//!      delay_us(1500);
//!      output_low(PIN_B7);
//!       delay_us(18500);
        }else if(distance > 32){ //  GREEN THE BARE AS OPEN U CAN PASSE 180 Degree
        ledController(0,0,1);
        servoDegreRotation(1500,18500);
//!        output_high(PIN_B7);
//!       delay_us(2100);
//!      output_low(PIN_B7);
//!       delay_us(17900);
        }
      
    }
  //delay_ms(100);
  }
}
