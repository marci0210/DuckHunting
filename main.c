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
void delay() {
   for(int d=0;d<150000;d++);
}

/*
 * This function demonstrates the usage of the extension driver for the upper part of the LCD.
 */
void demoUpperSegments() {
   // Turn on all segments one-by-one
   // Only one segment is turned on at any given time
   // Using 7 bit binary (raw) value
   for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_UPPER_CHARS; p++) {
      for (uint8_t s = 0; s < 8; s++) {
         upperCharSegments[p].raw = 1 << s;
         SegmentLCD_UpperSegments(upperCharSegments);
         delay();
      }
   }

   // Turn on all segments one-by-one
   // All the previous segments are left turned on
   // Using dedicated (bit field) values
   for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_UPPER_CHARS; p++) {
      upperCharSegments[p].a = 1;
      SegmentLCD_UpperSegments(upperCharSegments);
      delay();

      upperCharSegments[p].b = 1;
      SegmentLCD_UpperSegments(upperCharSegments);
      delay();

      upperCharSegments[p].c = 1;
      SegmentLCD_UpperSegments(upperCharSegments);
      delay();

      upperCharSegments[p].d = 1;
      SegmentLCD_UpperSegments(upperCharSegments);
      delay();

      upperCharSegments[p].e = 1;
      SegmentLCD_UpperSegments(upperCharSegments);
      delay();

      upperCharSegments[p].f = 1;
      SegmentLCD_UpperSegments(upperCharSegments);
      delay();

      upperCharSegments[p].g = 1;
      SegmentLCD_UpperSegments(upperCharSegments);
      delay();
   }

   // Clear all segments
   for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_UPPER_CHARS; p++) {
      upperCharSegments[p].raw = 0;
      SegmentLCD_UpperSegments(upperCharSegments);
   }
}

/*
 * This function demonstrates the usage of the extension driver for the lower part of the LCD.
 */
void demoLowerSegments() {
   // Turn on all segments one-by-one
   // Only one segment is turned on at any given time
   // Using 14 bit binary (raw) value
   for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_LOWER_CHARS; p++) {
      for (uint8_t s = 0; s < 15; s++) {
         lowerCharSegments[p].raw = 1 << s;
         SegmentLCD_LowerSegments(lowerCharSegments);
         delay();
      }
   }

   // Turn on all segments one-by-one
   // All the previous segments are left turned on
   // Using dedicated (bit field) values
   for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_LOWER_CHARS; p++) {
      lowerCharSegments[p].a = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].b = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].c = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].d = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].e = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].f = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].g = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].h = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].j = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].k = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].m = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].n = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].p = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();

      lowerCharSegments[p].q = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      delay();
   }

   // Clear all segments
   for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_LOWER_CHARS; p++) {
      lowerCharSegments[p].raw = 0;
      SegmentLCD_LowerSegments(lowerCharSegments);
   }
}

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  /* Initialize the LCD
   * (For this, we use the standard driver, located in files "segmentlcd.(c|h)"
   */
  SegmentLCD_Init(false);

  /* Infinite loop */
  while (1) {
     /* Demonstrating the extension driver, located in files "segmentlcd_individual.(c|h)"
      * The driver extension makes it possible to turn on or off individual segments on the
      * lower part of the display (containing 7 alphanumeric characters) and on the upper
      * part of the display (containing 4 seven-segment digits).
      */
     demoLowerSegments();
     demoUpperSegments();
  }
}
