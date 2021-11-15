#include "em_device.h"
#include "em_chip.h"

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

void duckNewPosition(uint8_t* Position){
	*Position = 2;
}

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  /* Initialize the LCD
   * (For this, we use the standard driver, located in files "segmentlcd.(c|h)"
   */
  SegmentLCD_Init(false);

  uint8_t duckPosition;
  duckNewPosition(&duckPosition);

  /* Infinite loop */
  while (1) {
     /* Demonstrating the extension driver, located in files "segmentlcd_individual.(c|h)"
      * The driver extension makes it possible to turn on or off individual segments on the
      * lower part of the display (containing 7 alphanumeric characters) and on the upper
      * part of the display (containing 4 seven-segment digits).
      */
     demoLowerSegments(duckPosition);
  }
}
