#include "em_acmp.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_core.h"
#include "em_device.h"
#include "em_gpio.h"
#include "em_lesense.h"
#include "em_system.h"
#include "em_timer.h"

#include "caplesense.h"

#include "segmentlcd.h"
#include "segmentlcd_individual.h"

#include "stdlib.h"
#include <stdint.h>
#include <stdbool.h>

SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS];
SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

// Defines for PB0 & PB1
#define PB0_PORT    gpioPortB
#define PB0_PIN     9

#define PB1_PORT    gpioPortB
#define PB1_PIN     10

#define SEG_NUM		4

//STRUCT
typedef struct coordinate
{
    uint8_t x;
    uint8_t y;
}coordinate;

//VARIABLES
bool shoot;
bool gameStart = false;
uint8_t duckCount = 0;
uint8_t duckHits = 0;
struct coordinate bullet;
struct coordinate hunter;
struct coordinate duck;

//FUNCTIONS
void delay(int lim)
{
    for(int d=0;d < lim;d++);
}
void upperLcdUpdate (uint8_t DucksQty, uint8_t HitsQty)
{
    SegmentLCD_Symbol(LCD_SYMBOL_COL10, 1);

    uint8_t segments[4], i;
    segments[3]=DucksQty/10;
    segments[2]=DucksQty%10;
    segments[1]=HitsQty/10;
    segments[0]=HitsQty%10;
    for(i=0;i<4;i++){
        switch(segments[i]){
        case 0:
            if(i%2==0){
                upperCharSegments[i].raw=63;
                /*upperCharSegments[i].a=1;
                upperCharSegments[i].b=1;
                upperCharSegments[i].c=1;
                upperCharSegments[i].d=1;
                upperCharSegments[i].e=1;
                upperCharSegments[i].f=1;
                upperCharSegments[i].g=0;*/
            }
            else{
                upperCharSegments[i].raw=0;
            }
        break;
        case 1:
            upperCharSegments[i].raw=6;
            /*upperCharSegments[i].a=0;
            upperCharSegments[i].b=1;
            upperCharSegments[i].c=1;
            upperCharSegments[i].d=0;
            upperCharSegments[i].e=0;
            upperCharSegments[i].f=0;
            upperCharSegments[i].g=0;*/
            break;
        case 2:
            upperCharSegments[i].raw=91;
            /*upperCharSegments[i].a=1;
            upperCharSegments[i].b=1;
            upperCharSegments[i].c=0;
            upperCharSegments[i].d=1;
            upperCharSegments[i].e=1;
            upperCharSegments[i].f=0;
            upperCharSegments[i].g=1;*/
            break;
        case 3:
            upperCharSegments[i].raw=79;
            /*upperCharSegments[i].a=1;
            upperCharSegments[i].b=1;
            upperCharSegments[i].c=1;
            upperCharSegments[i].d=1;
            upperCharSegments[i].e=0;
            upperCharSegments[i].f=0;
            upperCharSegments[i].g=1;*/
            break;
        case 4:
            upperCharSegments[i].raw=102;
            /*upperCharSegments[i].a=0;
            upperCharSegments[i].b=1;
            upperCharSegments[i].c=1;
            upperCharSegments[i].d=0;
            upperCharSegments[i].e=0;
            upperCharSegments[i].f=1;
            upperCharSegments[i].g=1;*/
        break;
        case 5:
            upperCharSegments[i].raw=109;
            /*upperCharSegments[i].a=1;
            upperCharSegments[i].b=0;
            upperCharSegments[i].c=1;
            upperCharSegments[i].d=1;
            upperCharSegments[i].e=0;
            upperCharSegments[i].f=1;
            upperCharSegments[i].g=1;*/
        break;
        case 6:
            upperCharSegments[i].raw=125;
            /*upperCharSegments[i].a=1;
            upperCharSegments[i].b=0;
            upperCharSegments[i].c=1;
            upperCharSegments[i].d=1;
            upperCharSegments[i].e=1;
            upperCharSegments[i].f=1;
            upperCharSegments[i].g=1;*/
        break;
        case 7:
            upperCharSegments[i].raw=7;
            /*upperCharSegments[i].a=1;
            upperCharSegments[i].b=1;
            upperCharSegments[i].c=1;
            upperCharSegments[i].d=0;
            upperCharSegments[i].e=0;
            upperCharSegments[i].f=0;
            upperCharSegments[i].g=0;*/
        break;
        case 8:
            upperCharSegments[i].raw=127;
            /*upperCharSegments[i].a=1;
            upperCharSegments[i].b=1;
            upperCharSegments[i].c=1;
            upperCharSegments[i].d=1;
            upperCharSegments[i].e=1;
            upperCharSegments[i].f=1;
            upperCharSegments[i].g=1;*/
        break;
        case 9:
            upperCharSegments[i].raw=111;
            /*upperCharSegments[i].a=1;
            upperCharSegments[i].b=1;
            upperCharSegments[i].c=1;
            upperCharSegments[i].d=1;
            upperCharSegments[i].e=0;
            upperCharSegments[i].f=1;
            upperCharSegments[i].g=1;*/
        break;
        }
    }
    SegmentLCD_UpperSegments(upperCharSegments);
}
void lowerLcdUpdate(struct coordinate Duck, struct coordinate Bullet,struct coordinate Hunter)
{
    uint8_t i,j;
    bool light;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            light=false;
            if(i == Duck.x && j == Duck.y){
                lowerCharSegments[i].a = 1;
                light=true;
            }
            if((i == Bullet.x && j == Bullet.y) && (0 < j && j < 3)){
                if(j==2){
                    lowerCharSegments[i].j = 1;
                }
                else{
                    lowerCharSegments[i].p = 1;
                }
                light=true;
            }
            if(i == Hunter.x && j == Hunter.y){
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
                }
            }
        }
    }
    SegmentLCD_LowerSegments(lowerCharSegments);
}
void clearLowerLcd()
{
    uint8_t i;
    for(i=0;i<4;i++){
        lowerCharSegments[i].raw = 0;
    }
    SegmentLCD_LowerSegments(lowerCharSegments);
}
void lcdDifficulty(int difficulty)
{
    uint8_t i;
    for(i = 0; i < 8; i++){
        if(i <= difficulty){
            SegmentLCD_ARing(i, 1);
        }
        else{
            SegmentLCD_ARing(i, 0);
        }
    }
}
void lcdDuckHit(struct coordinate Duck)
{
    uint8_t i;
    for(i=0;i<3;i++){
        lowerCharSegments[Duck.x].a = 0;
        SegmentLCD_LowerSegments(lowerCharSegments);
        delay(200000);

        lowerCharSegments[Duck.x].a = 1;
        SegmentLCD_LowerSegments(lowerCharSegments);
        delay(200000);
    }
    lowerCharSegments[i].a = 0;
    SegmentLCD_LowerSegments(lowerCharSegments);
    delay(100000);
}
void duckNewPosition()
{
    if(duckCount < 25){
        uint8_t newPosition;
        do{
            uint32_t seed = TIMER_CounterGet(TIMER0);
            srand(seed);
            newPosition = rand() % 4;
        } while(duck.x == newPosition);

        duck.x = newPosition;
        duckCount++;
    }
    else{
        gameStart = false;
    }
}
void hunterPosition()
{
    //0-48
    uint32_t loc = CAPLESENSE_getSliderPosition();

    for(uint8_t i = 0; i < 4; i++){
        if(loc >= 48 * i / 4 && loc <= 48 * (i + 1) / 4){
            if(abs(i - hunter.x) <= 1)
        		hunter.x = i;
        }
    }
}
int difficultyLevel(uint32_t loc)
{
    for(uint8_t i = 0; i < 8; i++){
        if(loc >= 48 * i / 8 &&loc <= 48 * (i + 1) / 8){
            return i;
        }
    }
    return -1;
}
void initVariables()
{
    duckNewPosition();
    duck.y = 3;
    bullet.x = 0;
    bullet.y = 0;
    hunter.x = 0;
    hunter.y = 0;
    duckCount = 0;
    duckHits = 0;
    shoot = false;
}
void TIMER1_IRQHandler()
{
    duckNewPosition();
    TIMER_IntClear(TIMER1, TIMER_IF_OF);      // Clear overflow flag
}
// Interrupt Service for PB0
void GPIO_ODD_IRQHandler()
{
    shoot = true;
    bullet.x = hunter.x;
    GPIO_IntClear(1 << PB0_PIN);
}
// Interrupt Service for PB1
void GPIO_EVEN_IRQHandler()
{
    gameStart = !gameStart;
    GPIO_IntClear(1 << PB1_PIN);
}

int main()
{
    CHIP_Init();                               // This function addresses some chip errata and should be called at the start of every EFM32 application (need em_system.c)
    SegmentLCD_Init(false);

    CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
    CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_4);

    CMU_ClockEnable(cmuClock_TIMER0, true);   // Enable TIMER0 peripheral clock
    CMU_ClockEnable(cmuClock_TIMER1, true);   // Enable TIMER1 peripheral clock
    CMU_ClockEnable(cmuClock_GPIO, true);

    GPIO_PinModeSet(PB0_PORT, PB0_PIN, gpioModeInput, 1);
    GPIO_PinModeSet(PB1_PORT, PB1_PIN, gpioModeInput, 1);

    TIMER_Init_TypeDef timerInit =            // Setup Timer initialization
    {
        .enable     = true,                     // Start timer upon configuration
        .debugRun   = true,                     // Keep timer running even on debug halt
        .prescale   = timerPrescale1024,
        .clkSel     = timerClkSelHFPerClk,      // Set HF peripheral clock as clock source
        .fallAction = timerInputActionNone,     // No action on falling edge
        .riseAction = timerInputActionNone,     // No action on rising edge
        .mode       = timerModeUp,              // Use up-count mode
        .dmaClrAct  = false,                    // Not using DMA
        .quadModeX4 = false,                    // Not using quad decoder
        .oneShot    = false,                    // Using continuous, not one-shot
        .sync       = false,                    // Not synchronizing timer operation off of other timers
    };

    TIMER_IntEnable(TIMER1, TIMER_IF_OF);     // Enable Timer1 overflow interrupt

    NVIC_EnableIRQ(TIMER1_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);

    NVIC_SetPriority(TIMER1_IRQn, 1);
    NVIC_SetPriority(GPIO_ODD_IRQn, 3);
    NVIC_SetPriority(GPIO_EVEN_IRQn, 4);

    TIMER_Init(TIMER0, &timerInit);           // Configure and start Timer0
    TIMER_Init(TIMER1, &timerInit);           // Configure and start Timer1
    GPIO_IntConfig(PB0_PORT, PB0_PIN, true, false, true);
    GPIO_IntConfig(PB1_PORT, PB1_PIN, true, false, true);

    CAPLESENSE_Init(false);


    int diff = 0;
    while(1){
        clearLowerLcd(); //Clear Lower LCD
        int a = CAPLESENSE_getSliderPosition();
        if(a >= 0 && a <= 48){
            diff = difficultyLevel(a);
        }
        lcdDifficulty(diff);
        if(gameStart){
            TIMER_TopSet(TIMER1,12000 - diff * 1000);
            TIMER_CounterSet(TIMER1, 0);
            initVariables();
        }

        while(gameStart){
            if(shoot){
                if(bullet.y == 3){
                    if(bullet.x == duck.x){
                        duckHits++;
                        TIMER_CounterSet(TIMER1, 0); //Ne generalogjon veletlenul se kacsa a hatterben
                        lcdDuckHit(duck);
                        duckNewPosition();
                        TIMER_CounterSet(TIMER1, 0);
                    }
                    bullet.y = 0;
                    shoot = false;
                }
                else{
                    bullet.y += 1;
                    delay(100000);
                }
            }

        lowerLcdUpdate(duck, bullet, hunter);
        upperLcdUpdate(duckCount, duckHits);
        hunterPosition();
        }
    }
}
