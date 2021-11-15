#include "em_device.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_system.h"
#include "em_timer.h"
#include "em_chip.h"

#include "stdlib.h"


/* If we want to use the LCD driver, we have to include "segmentlcd.h"
 * This file is located outside the project folder (on a system path).
 * Press F3 to locate it.
 */
#include "segmentlcd.h"

/* If we want to turn on or off segments individually, we also need to
 * include "segmentlcd_individual.h". The easiest way is to put this file
 * (and the corresponding C file) inside the project folder.
 */
#include "segmentlcd_individual.h"

/* The segment data (which segments to turn on, which segments to turn off)
 * should be stored in arrays (a separate array belongs to the upper and the
 * lower part of the LCD).
 *
 * The elements in the arrays store the segment data for a single character.
 * The upper display has 4, seven-segment digits (can be used to display numbers).
 * The lower display has seven, 14 segment character places (these can be used to
 * display alphanumeric characters).
 *
 */
SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS];
SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

/*
 * A simple software delay loop used by this demo to slow down changes.
 */
struct coordinate{
	uint8_t x;
	uint8_t y;
}coordinate;
void delay() {
   for(int d=0;d<150000;d++);
}
/*
 * This function demonstrates the usage of the extension driver for the lower part of the LCD.
 */
void demoLowerSegments(uint8_t p) {
   // Turn on all segments one-by-one
   // All the previous segments are left turned on
   // Using dedicated (bit field) values

  lowerCharSegments[p].a = 1;
  SegmentLCD_LowerSegments(lowerCharSegments);
  delay();

  lowerCharSegments[p].a = 0;
  SegmentLCD_LowerSegments(lowerCharSegments);
}
void lowerLcdUpdate(struct coordinate Duck, struct coordinate Bullet,struct coordinate Hunter){
	uint8_t i,j;
	struct coordinate current;
	bool light;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			current.x=i;
			current.y=j;
			light=false;
			if(current==Duck){
				lowerCharSegments[i].a = 1;
				light=true;
			}
			if(current==Bullet&&(0<current.y&&current.y<3)){
				if(j==2){
				lowerCharSegments[i].j = 1;
				}
				else{
				lowerCharSegments[i].p = 1;
				}
				light=true;
			}
			if(current==Hunter){
				lowerCharSegments[i].d = 1;
				light=true;
			}
			if(light!=true){
				switch(j){
				case 0:
					lowerCharSegments[i].d = 0;
					break;
				case 1:
					lowerCharSegments[i].p = 0;
					break;
				case 2:
					lowerCharSegments[i].j = 0;
					break;
				case 3:
					lowerCharSegments[i].a = 0;
					break;
				default:
				}

			}
		}
	}
}

uint8_t duckNewPosition(uint8_t currPosition){
	uint8_t newPosition;
	do{
		uint32_t seed = TIMER_CounterGet(TIMER0);
		srand(seed);
		newPosition = rand() % 4;
	} while(currPosition == newPosition);
	return newPosition;
}

void TIMER1_IRQHandler(void)
{
  TIMER_IntClear(TIMER1, TIMER_IF_OF);      // Clear overflow flag                             // Increment counter
}

int main() {
  uint8_t duckP = 0;
  CHIP_Init();                               // This function addresses some chip errata and should be called at the start of every EFM32 application (need em_system.c)

  TIMER_TopSet(TIMER1, 50000);
  CMU_ClockEnable(cmuClock_TIMER0, true);   // Enable TIMER0 peripheral clock
  CMU_ClockEnable(cmuClock_TIMER1, true);   // Enable TIMER1 peripheral clock

  TIMER_Init_TypeDef timerInit =            // Setup Timer initialization
  {
    .enable     = true,                     // Start timer upon configuration
    .debugRun   = true,                     // Keep timer running even on debug halt
    .prescale   = timerPrescale1,           // Use /1 prescaler...timer clock = HF clock = 1 MHz
    .clkSel     = timerClkSelHFPerClk,      // Set HF peripheral clock as clock source
    .fallAction = timerInputActionNone,     // No action on falling edge
    .riseAction = timerInputActionNone,     // No action on rising edge
    .mode       = timerModeUp,              // Use up-count mode
    .dmaClrAct  = false,                    // Not using DMA
    .quadModeX4 = false,                    // Not using quad decoder
    .oneShot    = false,                    // Using continuous, not one-shot
    .sync       = false,                    // Not synchronizing timer operation off of other timers
  };

  TIMER_IntEnable(TIMER1, TIMER_IF_OF);     // Enable Timer0 overflow interrupt
  TIMER_Init(TIMER0, &timerInit);           // Configure and start Timer0
  TIMER_Init(TIMER1, &timerInit);           // Configure and start Timer1

  while(1)
  {
	  duckP = duckNewPosition(duckP);
  }
}
