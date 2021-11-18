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

// PBO-as gomb portjanak es pinjenek definialasa
#define PB0_PORT    gpioPortB
#define PB0_PIN     9
// PB1-es gomb portjanak es pinejnek definialasa
#define PB1_PORT    gpioPortB
#define PB1_PIN     10
// Hasznalando LCD szegmensek szama
#define SEG_NUM		4

// Koordinata struktura a kacsa, vadasz es tolteny szamara
typedef struct coordinate
{
    uint8_t x;
    uint8_t y;
}coordinate;

// Globalis Valtozok
int diff = 0;				// Nehezsegi szint valtozoja
bool shoot;					// Van-e aktiv loves
bool gameStart = false;		// Jatek allapota (0- menu, 1 - jatek)
uint8_t duckCount = 0;		// Generalt kacsak szama
uint8_t duckHits = 0;		// Eltalalt kacsak szama
struct coordinate bullet;	// Tolteny koordinatai
struct coordinate hunter;	// Vadasz koordinatai
struct coordinate duck;		// Kacsa koordinatai

// Szoftveres keslelteto fuggveny
void delay(int lim)
{
    for(int d=0;d < lim;d++);
}
// Komment
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
                upperCharSegments[i].raw=63;    // 7'b0111111
            }
            else{
                upperCharSegments[i].raw=0;     // 7'b0000000
            }
            break;
        case 1:
            upperCharSegments[i].raw=6;         // 7'b0000110
            break;
        case 2:
            upperCharSegments[i].raw=91;        // 7'b1011011
            break;
        case 3:
            upperCharSegments[i].raw=79;        // 7'b1001111
            break;
        case 4:
            upperCharSegments[i].raw=102;       // 7'b1100110
            break;
        case 5:
            upperCharSegments[i].raw=109;       // 7'b1101101
            break;
        case 6:
            upperCharSegments[i].raw=125;       // 7'b1111101
            break;
        case 7:
            upperCharSegments[i].raw=7;         // 7'b0000111
            break;
        case 8:
            upperCharSegments[i].raw=127;       // 7'b1111111
            break;
        case 9:
            upperCharSegments[i].raw=111;       // 7'b1101111
            break;
        }
    }
    SegmentLCD_UpperSegments(upperCharSegments);
}

/* Also LCD sor egyeni szamozasa a kezeleshez
 *  y
 *  ^
 *  3 -----------    -----------    -----------    -----------
 *  |
 *  |      |              |              |              |
 *  2      |              |              |              |
 *  |      |              |              |              |
 *  |
 *  |      |              |              |              |
 *  1      |              |              |              |
 *  |      |              |              |              |
 *  |
 *  0 -----------    -----------    -----------    -----------
 * -+------0--------------1--------------2--------------3-------> x
 */

// Komment
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
// Komment
void clearLowerLcd()
{
    uint8_t i;
    for(i=0;i<4;i++){
        lowerCharSegments[i].raw = 0;
    }
    SegmentLCD_LowerSegments(lowerCharSegments);
}
// Komment
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
// Komment
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
// Kacsa uj poziciojat generalto fuggveny
void duckNewPosition()
{
	// Ha a kacsak szama nem erte el meg a maximumom ujat general
    if(duckCount < 25){
        uint8_t newPosition;							// Uj pozicio 8 biten
        do{
            uint32_t seed = TIMER_CounterGet(TIMER0);	// Random fuggvenynek seed a TIMER0 aktualis erteke alapjan
            srand(seed);								// Random fuggvenybe seed betoltese
            newPosition = rand() % 4;					// Random szam generalasa a szegmensek szamaig
        } while(duck.x == newPosition);					// Addig general ujat, amig a pozicio nem valtozik

        duck.x = newPosition;							// Kacsa uj poziciojanak beallitasa
        duckCount++;									// Kacsak eddigi szamanak novelese
    }
    // Ha a kacsak szama mar elerte a maximumot, megszakitja a jatekot -> menu toltodik be
    else{
        gameStart = false;
    }
}
// Vadasz uj poziciojat beallito fuggveny
void hunterPosition()
{
    // Kapacitiv csuszka poziciojanak kiolvasasa
	// (-1, ha nincs erintkezese, 0-48-ig aranyosan ha van)
    int loc = CAPLESENSE_getSliderPosition();
    // Szegmensek szamanak aranyaban felosztja a 0-48-as tartomanyt
    for(uint8_t i = 0; i < 4; i++){
        if(loc >= 48 * i / 4 && loc <= 48 * (i + 1) / 4){
        	// Ha az uj pozicio a mostanival, vagy azzal szomszedossal egyezik meg
            if(abs(i - hunter.x) <= 1)
        		hunter.x = i;				// Beallitja a poziciot
        }
    }
}
// Nehezsegi szintet beallito fuggveny, bemeneti parameter a kapacitiv csuszka pozicioja
// (-1, ha nincs erintkezese, 0-48-ig aranyosan ha van)
int difficultyLevel(int loc)
{	// 8 elerheto nehezsegi szint aranyaban felosztja a 0-48-as tartomanyt
    for(uint8_t i = 0; i < 8; i++){
        if(loc >= 48 * i / 8 &&loc <= 48 * (i + 1) / 8){
            return i;						// Visszater egy 0-7 kozotti szammal, ami a nehezsegnek felel meg
        }
    }
    // Ha nem ertelmezheto a bemenet (nem 0-48 kozotti), -1-el ter vissza
    return -1;
}
// Valtozokat kezdeti ertekre allito fuggveny
void initVariables()
{
    duckNewPosition();		// Kacsa x koordinataja random fuggveny alapjan
    duck.y = 3;				// Kacsa y koordinataja 3
    bullet.x = 0;			// Tolteny x koordinataja kezdetben 0
    bullet.y = 0;			// Tolteny y koordinataja kezdetben 0
    hunter.x = 0;			// Vadasz x koordinataja kezdetben 0
    hunter.y = 0;			// Vadasz y koordinataja kezdetben 0
    duckCount = 1;			// Megjelent kacsak szama kezdetben 1
    duckHits = 0;			// Eltalalt kacsak szama kezdetben 0
    shoot = false;			// Kezdetben nincsen loves
}
// TIMER1 megszakitast kezelo fuggvenye -> uj kacsa generalas
void TIMER1_IRQHandler()
{
    duckNewPosition();						// Kacsa x koordinatajanak ujrageneralasa
    TIMER_IntClear(TIMER1, TIMER_IF_OF);	// Megszakitast jelzo flag torlese
}
// GPIO paratlan portjainak megszakitast kezelo fuggvenye -> PB0 -> aktiv loves
void GPIO_ODD_IRQHandler()
{
    shoot = true;					// Lovest aktivva teszi
    bullet.x = hunter.x;			// Tolteny koordinataja a vadasz kezdeti koordinataja lesz
    GPIO_IntClear(1 << PB0_PIN);	// Torli a megszakitast jelzo flaget.
}
// GPIO paros portjainak megszakitast kezelo fuggvenye -> PB1 -> jatek inditasa/megszakitasa
void GPIO_EVEN_IRQHandler()
{
	// Ha a jatek ennek a lenyomasnak hatasara fog elindulni
	if(!gameStart){
        TIMER_TopSet(TIMER1,12000 - diff * 1000);		// Timer1 maximalis erteket allitjuk a nehezseg fuggvenyeben
        TIMER_CounterSet(TIMER1, 0);					// Betolti a kezdeti 0 erteket
        initVariables();								// Inicializalja a valtozokat
	}
    gameStart = !gameStart;			// Jatek allapotot invertalja
    GPIO_IntClear(1 << PB1_PIN);	// Torli a megszakitast jelzo flaget.
}

int main()
{
    CHIP_Init();					//CHIP inicializalasa
    SegmentLCD_Init(false);			//LCD inicializalesa

    // Nagyfrekvencials belso RC osszcillator engedelyezese, felallasi ido megvarasaval
    CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
    // Elozo oszcillator orajelenek 4es osztasa
    CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_4);

    // Orajel engedalyezese a TIMER0, TIMER1 es GPIO szamara
    CMU_ClockEnable(cmuClock_TIMER0, true);  	 // Enable TIMER0 peripheral clock
    CMU_ClockEnable(cmuClock_TIMER1, true);   	// Enable TIMER1 peripheral clock
    CMU_ClockEnable(cmuClock_GPIO, true);

    // PB0, PB1 gomb engedelyezese bemenetre
    GPIO_PinModeSet(PB0_PORT, PB0_PIN, gpioModeInput, 1);
    GPIO_PinModeSet(PB1_PORT, PB1_PIN, gpioModeInput, 1);

    // TIMER-ek inicializalasahoz szukseges dtruktora definialesa
    TIMER_Init_TypeDef timerInit =
    {
        .enable     = true,                     // Timer kezdeti engedelyezese
        .debugRun   = true,                     // Timer debug modban valo futasanak engedelyezese
        .prescale   = timerPrescale1024,		// Prescale ertek beallitasa
        .clkSel     = timerClkSelHFPerClk,      // orajelenek RC osszcillatorhoz valo beallitasa
        .fallAction = timerInputActionNone,     // Lefuto elre valo interakcio tiltasa
        .riseAction = timerInputActionNone,     // Felfuto elre valo interakcio tiltasa
        .mode       = timerModeUp,              // Felfele szamlalas beallitasa
        .dmaClrAct  = false,                    // DMA tiltasa
        .quadModeX4 = false,                    // quadMode tiltasa
        .oneShot    = false,                    // Folyamato szamlalas engedelyezese
        .sync       = false,                    // Tobbi TIMER-er valo szinkronizasio kikapcsolasa
    };

    TIMER_IntEnable(TIMER1, TIMER_IF_OF);		// Enable Timer1 overflow interrupt

    // Interruptok engedelyezese
    NVIC_EnableIRQ(TIMER1_IRQn);				// Interrupt engedelyezese TIMER1 szamara
    NVIC_EnableIRQ(GPIO_ODD_IRQn);				// Interrupt engedelyezese paratlan GPIO portok szamara
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);				// Interrupt engedelyezese paros GPIO portok szamara

    // Interrupt prioritasok beallitasa
    NVIC_SetPriority(GPIO_EVEN_IRQn, 0);		// Legmagasabb prioritas a paros GPIO-kra (PB1 - stop & start)
    NVIC_SetPriority(TIMER1_IRQn, 1);			// 1-es prioritas a TIMER1-nek (uj kacsa)
    NVIC_SetPriority(GPIO_ODD_IRQn, 2);			// 2-es prioritas a paratlan GPIO-kra (PB0 - loves)

    // Timerek konfiguraicoja
    TIMER_Init(TIMER0, &timerInit);           	// Timer0 konfiguracioja es inditasa
    TIMER_Init(TIMER1, &timerInit);          	// Timer1 konfiguracioja es inditasa

    // GPIO periferiak konfiguracioja
    GPIO_IntConfig(PB0_PORT, PB0_PIN, true, false, true); // PB0 aktivalasa felfutoelre
    GPIO_IntConfig(PB1_PORT, PB1_PIN, true, false, true); // PB1 aktivalasa felfutoelre

    // Kapacitiv csuszka engedelyezese
    CAPLESENSE_Init(false);

    lcdDifficulty(diff);	// Kezdeti 0-hoz tartozo szint megjelenitese

    // Menu ciklusa
    while(1){
        clearLowerLcd(); 								// Also LCD sor tartalmanak torlese
        int a = CAPLESENSE_getSliderPosition();			// Kapacitiv csuszka ertekenek kiolvasasa
        // Ha az erteke az ertelmes, 0-48as tartomanyban talalhato
        if(a >= 0 && a <= 48 && !gameStart){
            diff = difficultyLevel(a);		// Akkor a leosztas aranyaban beallitjuk az erteket
            lcdDifficulty(diff);			// es megjelenitjuk a kijelzon
        }
        // Jatek ciklusa
        while(gameStart){
        	// Ha a loves aktiv
            if(shoot){
            	// Ha a tolteny y koordinataja elerte a kacsak szintjet
                if(bullet.y == 3){
                	// es a tolteny x koordinataja a kacsaeval megegyszik
                    if(bullet.x == duck.x){
                        duckHits++;						// Talalatok szamat novelem
                        TIMER_CounterSet(TIMER1, 0); 	// TIMER1-be 0-t toltok, hogy a villogas alatt ne generalodjon uj kacsa
                        lcdDuckHit(duck);				// Kacsavillogtato fuggveny hivasa
                        duckNewPosition();				// uj kacsa generalasa
                        TIMER_CounterSet(TIMER1, 0);	// TIMER1 0-ba allitasa
                    }
                    bullet.y = 0;			// Tolteny y koordinatajanak 0-ba allitasa
                    shoot = false;			// Loves kikapcsolasa
                }
                // Ha a tolteny y koordinataja nem erte el a kacsak szintjat
                else{
                    bullet.y += 1;			// Akkor az y koordinata novelese
                    delay(100000);			// Loves sebessegenek beallitasa szoftveres kesleltetessel
                }
            }
			hunterPosition();						// Vadasz poziciojanak esetleges frissitese

			lowerLcdUpdate(duck, bullet, hunter);	// Also LCD sor frissitese
			upperLcdUpdate(duckCount, duckHits);	// Felso LCD sor frissitese
        }
    }
}
