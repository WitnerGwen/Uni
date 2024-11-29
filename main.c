#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

// Custom "registers"
volatile uint8_t LEDS; // LED diodes state register
volatile uint8_t BTNS; // Buttons state register
volatile uint8_t SEG7[5]; // 7-segment display state registers

#define	SPI_CS (1 << 2)
#define SPI_MOSI (1 << 3)
#define SPI_MISO (1 << 4)
#define	SPI_SCK (1 << 5)

void SPI_Init()
{
  PORTB |= SPI_CS; // chip select disable
  DDRB |= SPI_SCK | SPI_MOSI | SPI_CS; // Sets SCK, MOSI, CS as outputs
  DDRB &= ~SPI_MISO; // Sets MISO as input
  SPCR |= (1 << MSTR); // Sets the device as SPI master.
  SPCR |= (1 << SPR1) | (1 << SPR0); // Sets a slower SPI clock speed (divides system clock by 128).
  SPCR |= (1 << SPIE); // Enables SPI interrupt.
  SPCR |= (1 << SPE); // Enables the SPI module.
  sei(); // Enable global interrupts
  PORTB &= ~SPI_CS; // chip select to enable communication
  SPDR = LEDS; // Loads 'LEDS' into the SPI Data Register to start transmission over SPI.
}

static uint8_t spi_state;

ISR(SPI_STC_vect)
{
  switch(spi_state)
  {
    case 0: BTNS = SPDR;
    case 1: case 2: case 3: case 4: SPDR = SEG7[spi_state]; break;
    case 5: PORTB |= SPI_CS; SPDR = LEDS; break;
    case 6: PORTB &= ~SPI_CS; SPDR = LEDS; break;
  }
  spi_state++;
  if(spi_state > 6) { spi_state = 0; }
}

void SEG7_Sign(char sign, uint8_t pos, bool dot){
  switch (sign)
  {
  case 'c': //c = clear
    SEG7[pos] = 0b00000000;    //empty display
    break;
  case 1:
    if(dot == true){
      SEG7[pos] = 0b10000110;  //1.
    }else{
      SEG7[pos] = 0b00000110;  //1
    }
    break;
  case 2:
    if(dot == true){
      SEG7[pos] = 0b11011011;  //2.
    }else{
      SEG7[pos] = 0b01011011;  //2
    }
    break;
  case 3:
    if(dot == true){
      SEG7[pos] = 0b11001111;  //3.
    }else{
      SEG7[pos] = 0b01001111;  //3
    }
    break;
  case 4:
    if(dot == true){
      SEG7[pos] = 0b11100110;  //4.
    }else{
      SEG7[pos] = 0b01100110;  //4
    }
    break;
  case 5:
    if(dot == true){
      SEG7[pos] = 0b11101101;  //5.
    }else{
      SEG7[pos] = 0b01101101;  //5
    }
    break;
  case 6:
    if(dot == true){
      SEG7[pos] = 0b11111101;  //6.
    }else{
      SEG7[pos] = 0b01111101;  //6
    }
    break;
  case 7:
    if(dot == true){
      SEG7[pos] = 0b10000111;  //7.
    }else{
      SEG7[pos] = 0b00000111;  //7
    }
    break;
  case 8:
    if(dot == true){
      SEG7[pos] = 0b11111111;  //8.
    }else{
      SEG7[pos] = 0b01111111;  //8
    }
    break;
  case 9:
    if(dot == true){
      SEG7[pos] = 0b11101111;  //9.
    }else{
      SEG7[pos] = 0b01101111;  //9
    }
    break;
  case 0:
    if(dot == true){
      SEG7[pos] = 0b10111111;  //0.
    }else{
      SEG7[pos] = 0b00111111;  //0
    }
    break;
  case 'h':
    if(dot == true){
      SEG7[pos] = 0b11110110;  //H.
    }else{
      SEG7[pos] = 0b01110110;  //H
    }
    break;
  case 'e':
    if(dot == true){
      SEG7[pos] = 0b11111001;  //E.
    }else{
      SEG7[pos] = 0b01111001;  //E
    }
    break;
  case 'l':
    if(dot == true){
      SEG7[pos] = 0b10111000;  //L.
    }else{
      SEG7[pos] = 0b00111000;  //L
    }
    break;
  case 'r':
    if(dot == true){
      SEG7[pos] = 0b11010000;  //r. making a capital R look good on a seg7 display is impossible
    }else{
      SEG7[pos] = 0b01010000;  //r
    }
    break;
  case 'o':
    if(dot == true){
      SEG7[pos] = 0b11011100;  //o. 
    }else{
      SEG7[pos] = 0b01011100;  //o
    }
    break;
  default:
    break;
  }
}

int main(void)
{
  // The HELLO intro
  SPI_Init();
  SEG7_Sign('h',0,false);
  _delay_ms(400);
  SEG7_Sign('e',1,false);    
  _delay_ms(400);
  SEG7_Sign('l',2,false);
  _delay_ms(400);
  SEG7_Sign('l',3,false);
  _delay_ms(400);
  SEG7_Sign(0,4,false);   
  _delay_ms(3000); 
  // This code hates for loops :(
  SEG7_Sign('c',0,false);
  SEG7_Sign('c',1,false);
  SEG7_Sign('c',2,false);
  SEG7_Sign('c',3,false);
  SEG7_Sign('c',4,false);   
  // Declaring ints
  int16_t i4 = 0;
  int16_t i3 = 0;
  int16_t i2 = 0;
  int16_t i1 = 8;
  int16_t i0 = 9;
  int16_t delay = 0;
  while(1){
    delay++;
    if(delay > 1){    // The loop executes every 1000 ms
      delay = 0;
      i4++;
      if(i4 == 10){
        i4 = 0;
        i3++;
      }
      if(i3 == 10){
        i3 = 0;
        i2++;
      }
      if(i2 == 10){
        i2 = 0;
        i1++;
      }
      if(i1 == 10){
        i1 = 0;
        i0++;
      }
      SEG7_Sign(i4,4,false); //5th number
      SEG7_Sign(i3,3,false); //4th number
      SEG7_Sign(i2,2,false); //3rd number
      SEG7_Sign(i1,1,false); //2nd number
      SEG7_Sign(i0,0,false); //1st number
      if(i0 > 9){
        SEG7_Sign('e',0,false);
        SEG7_Sign('r',1,false);
        SEG7_Sign('r',2,false);
        SEG7_Sign('o',3,false);
        SEG7_Sign('r',4,false);
      }           
    }
    _delay_ms(1);
  }
}