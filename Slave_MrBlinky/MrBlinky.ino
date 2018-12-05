/*******************************************************************************
This is the code for charlieplexing 6 leds.  

Version1: Straightforeward, indipendent control of any/all LEDS. It provides for some 
    degree of greyscale (4 levels with some flickering) but max brightness is 1/6 duty cycle.

Version2: From here on, we'll assume that this will be a 6 lite chase with only 2 lights
    possibly being on (to some) level, at a time.  If each LED had its own series resistor,
    the 2 LEDs could be driven simultaneously (AB/AC, AC/BC, BC/BA, BA/CA, CA/CB, CB/AB...), 
    but for this version2, we'll multiplex just a pair at a time, with max brightness now 
    being 1/2 duty cycle.

Version3: Add the AtoD stuff.  Also, multiple scripts.

Version4: Debugged the AtoD stuff. Add script speed var.

Version5: added repeat & delayInterval (including random)to scriptScript_TypeDef.  Added
    halt/AWU for sleep state for consumption considerations.

Version6: added working rf button code)but unused).  Split up sories.  Randomize speed.
Version7: Radio button now toggles thru stories.  Script changes.
Version 1.0 - Moved to Arduino platform from STM8
********************************************************************************/






typedef char s8;
typedef signed short s16;



#include "scripts.h"
#include "TimerOne.h"

extern scriptScript_TypeDef* storyPtrs[];
scriptScript_TypeDef* storyPtr;
scriptScript_TypeDef* scriptPtr;

TimerOne Timer1;

u16 mSeCounter;
u16 mSeCounterABS;
u8 scriptStepIndex;
u8 profileStepIndex;
u8 profileRepeatCounter;
u8 storyStepIndex;
s16 delayInterval;
s8  storyStepSpeed;

bool sleepState;
bool sleepReq; 

////////////////// RF (pushbutton) input stuff ///////////////
//#define RF_INP_PORT  GPIOB
//#define RF_INP_PIN   GPIO_PIN_6
//#define RF_ENAB_PIN  GPIO_PIN_7
//static bool buttonCmd = false;
//INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler,4)
//{
//  disableInterrupts();
//#define BUTTONTRACKERLEVEL 10
//#define BUFFSIZE 20
//#define HITARG 194
//#define LOTARG 171
//    static u8   buttonTracker=0;
//    static bool buttonTrackerPressed=false;
//    static u16 valI=0;
//    static u16 valLast=0;
//    static u16 vals[BUFFSIZE] ={0};
//    
//    u16 x = TIM3_GetCounter();
//    u16 dif = x - valLast;
//    if (dif>HITARG/2)
//    {
//        vals[valI++] =  dif; 
//        valLast = x;
//        if (valI==BUFFSIZE)
//            valI=0;
//
//        u8 goodOnes=0;
//        for (int i=0; i<BUFFSIZE; i++)
//        {
//          if ((vals[i] > LOTARG) && (vals[i] < HITARG))
//            goodOnes++;
//        }  
//        if (goodOnes > (BUFFSIZE*2)/4)
//        {//pressed
//          GPIO_WriteLow(GPIOD, GPIO_PIN_0);   //little green led ON
//          if (buttonTracker<BUTTONTRACKERLEVEL)
//            buttonTracker+=3;
//        }
//        else
//        {//released
//          GPIO_WriteHigh(GPIOD, GPIO_PIN_0);   //little green led OFF
//          if (buttonTracker!=0)
//            buttonTracker--;
//        }
//        if (buttonTracker>BUTTONTRACKERLEVEL)
//          buttonTrackerPressed=true;
//        else if (buttonTrackerPressed && (buttonTracker==0))
//        {
//          buttonTrackerPressed=false;
//          buttonCmd = true;
//        }
//    }
//    enableInterrupts();
//}

///////////////////  LED definition stuff //////////////////
#define NUM_OF_GROUPS  3
#define NUM_OF_LINES   NUM_OF_GROUPS*2

static int groupPin[NUM_OF_LINES] = {
    /*Group1LoPin*/   1,
    /*Group1HiPin*/   2,
    /*Group2LoPin*/   3,
    /*Group2HiPin*/   4,
    /*Group3LoPin*/   5,
    /*Group3HiPin*/   6  };

#define GROUP1   0              
#define GROUP2   1
#define GROUP3   2
#define POS      0              
#define NEG      1
typedef enum
{	
	TRISTATE_HI  	= (u8)0,
	TRISTATE_LO  	= (u8)1,
	TRISTATE_FLOAT	= (u8)2,
}triState_TypeDef;

typedef struct
{
  u8                 groups[2];
  u8                 level;          //desired brightness; 0->MAX_LED_LEVEL (0=off)
  u8                 curLevel;       //counts to MAX_LED_LEVEL by 1 every multplex chance, when level>0
}ledState_TypeDef;
#define MAX_LED_LEVEL 8  //MAX_LED_LEVEL levels 
#define NUM_OF_LEDS   6
ledState_TypeDef ledStates[NUM_OF_LEDS];

/////////////////////////// AtoD stuff //////////////////////
#define SOLAR_INP                 5
#define BATT_INP                  4
#define SOLAR_DIV_FACTOR         30
#define BATT_DIV_FACTOR          10
#define BATT_ON_THREASHHOLD     5000  //5.0V
#define BATT_OFF_THREASHHOLD    4300  //4.3v
#define SOLAR_DAY_THREASHHOLD   3000  //2.0V
#define SOLAR_NIGHT_THREASHHOLD  400  //0.4V

// the setup function runs once when you press reset or power the board
//#define DEV     1
void setup() {
    sleepState = false;
#ifdef DEV
    sleepReq = false;            //always start in sleepstate
#else
    sleepReq = true;
#endif
    setupStoryParams(false);

    Init_Configuration();

}

// the loop function runs over and over again until power down or reset
void loop() {
 
}

/***********************************************************************************************/



s16 GetADCValue(u8 chan) {return 0;}	
bool evalVoltages()
{
    bool retStat = false;
    static u8 runningChangeCounter=0;
    u16 bat = GetADCValue(BATT_INP);
    u16 sol = GetADCValue(SOLAR_INP);       
    if ( sleepState ) //If not running, turn on?
    {
      if ((bat > BATT_ON_THREASHHOLD) && 
          (sol< SOLAR_NIGHT_THREASHHOLD))
      {
        if (++runningChangeCounter >2)
          retStat = true;
      }
      else
        runningChangeCounter = 0;
    }
    else //Else running. Turn off?
    {
      if (bat < BATT_OFF_THREASHHOLD)
      {
        if (++runningChangeCounter >4)
          retStat = true;
      }
      else if(sol > SOLAR_DAY_THREASHHOLD)              //daybreak.  Wont last that long
      {
        if (++runningChangeCounter >4)
          retStat = true;
      }
      else
        runningChangeCounter = 0;
    }
    if (retStat)
       runningChangeCounter=0;
    return retStat;
}
/*************************************************************/
void Init_Configuration(void)
{

    ///*** AWU **************/
    //AWU_LSICalibrationConfig(127450);          //127450
    ///* The delay corresponds to the time we will stay in Halt mode */
    //AWU_Init(AWU_TIMEBASE_30S); //AWU_TIMEBASE_12S, AWU_TIMEBASE_30S

    /*** A/D **************/
//    Init_ADC();
    
 // /*** TIM3 **************/
 // //** TIM3_PRESCALER_128 results in 125/2ms
 // //** TIM3_PRESCALER_256 results in 62-63/2ms
 // //** TIM3_PRESCALER_512 results in 31-32/2ms
 // TIM3_TimeBaseInit(TIM3_PRESCALER_128, 0xffff);
 // TIM3_PrescalerConfig(TIM3_PRESCALER_128, TIM3_PSCRELOADMODE_UPDATE);
 // TIM3_Cmd(ENABLE);

 // /*** TIM4 **************/
 // #define TIM4_PERIOD       124
 ///* TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
 //  clock used is 16 MHz / 128 = 125 000 Hz
 // - With 125 000 Hz we can generate time base:
 //     max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
 //     min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
 // - In this example we need to generate a time base equal to 1 ms
 //  so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */
 // TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);   /* Time base configuration */
 // TIM4_ClearFlag(TIM4_FLAG_UPDATE);                     /* Clear TIM4 update flag */
 // TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);                /* Enable update interrupt */
 // TIM4_Cmd(ENABLE);
  

  /*** Initialize I/Os  - LEDs */
    for (int i=0; i<NUM_OF_LINES; i++)
    {
		pinMode(groupPin[i], OUTPUT);
    }
    u8 i =0;
    ledStates[i].groups[POS]=GROUP3;
    ledStates[i].groups[NEG]=GROUP1;
    ledStates[i++].level=0;

    ledStates[i].groups[POS]=GROUP1;
    ledStates[i].groups[NEG]=GROUP3;
    ledStates[i++].level=0;

    ledStates[i].groups[POS]=GROUP2;
    ledStates[i].groups[NEG]=GROUP3;
    ledStates[i++].level=0;

    ledStates[i].groups[POS]=GROUP3;
    ledStates[i].groups[NEG]=GROUP2;
    ledStates[i++].level=0;

    ledStates[i].groups[POS]=GROUP2;
    ledStates[i].groups[NEG]=GROUP1;
    ledStates[i++].level=0;

    ledStates[i].groups[POS]=GROUP1;
    ledStates[i].groups[NEG]=GROUP2;
    ledStates[i++].level=0;

	Timer1.initialize(500);	//# of microseconds (500mSec)
	Timer1.attachInterrupt(TimerInterrupt);

}


void UpdateGroupLines(u8 group, triState_TypeDef newState)
{
      bool lo_pin = false;
      bool hi_pin = false;
      if (newState == TRISTATE_HI)
        hi_pin=true;
      else if (newState == TRISTATE_LO)
        lo_pin=true;

      digitalWrite(groupPin[(group*2)],   lo_pin);
      digitalWrite(groupPin[(group*2)+1], hi_pin);
}

/******************************************/
u16 genrandIfNeg(s16 max, u8 min)
{
	if (max>=0)
		return (max);
	return random(0-max, min);
}

void setupStoryParams(bool inc)
{
    static u8 index=0;
    storyStepIndex=scriptStepIndex=profileStepIndex=0;
    mSeCounter = mSeCounterABS = 0;
    u8 sz= sizeof(storyPtrs)/sizeof(storyPtrs[0]);
    if (inc && ++index>(sz-1))
      index=0;
    storyPtr = storyPtrs[index];
    scriptPtr = &storyPtr[storyStepIndex];
    profileRepeatCounter = scriptPtr->repeat;
    storyStepSpeed = genrandIfNeg(scriptPtr->speed, 1);
    delayInterval=0;
}

void AllLEDsOff(void)
{
    UpdateGroupLines(0, TRISTATE_FLOAT);     //turn off any/all LEDs
    UpdateGroupLines(1, TRISTATE_FLOAT);
    UpdateGroupLines(2, TRISTATE_FLOAT);
}
/*************************************************************/

void UpdateLED(u8 index)   //called by the timed interrupt for the charlieplexing..
{                          //...turn prev one off and next one On, if needed.
  u8 prevIndex = 1;        //index is 0 or 1 to toggle which of the profile...
  if (index == 1)          //...pairs we'll be updating here.
    prevIndex = 0;
  
  u8 ledNum  = scriptPtr->script[scriptStepIndex].profile[index].ledNum;
  u8 prevLed = scriptPtr->script[scriptStepIndex].profile[prevIndex].ledNum;
  
  UpdateGroupLines(ledStates[prevLed].groups[POS], TRISTATE_FLOAT);
  UpdateGroupLines(ledStates[prevLed].groups[NEG], TRISTATE_FLOAT);

  if (ledStates[ledNum].level != 0)
  {
    if (ledStates[ledNum].level > ledStates[ledNum].curLevel)
    {
        UpdateGroupLines(ledStates[ledNum].groups[POS], TRISTATE_HI);
        UpdateGroupLines(ledStates[ledNum].groups[NEG], TRISTATE_LO);
    }
    if ( ++ledStates[ledNum].curLevel >= MAX_LED_LEVEL)
      ledStates[ledNum].curLevel=0;   //start over
  }
  else 
    ledStates[ledNum].curLevel=0;
}
/***********************************************/
void TimerInterrupt()
{
//    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    bool delay = false;
    mSeCounterABS++;

    //if (buttonCmd)
    //{
    //  buttonCmd=false;
    //  AllLEDsOff();
    //  setupStoryParams(true);
    //}

    if (delayInterval !=0)
    {
      delayInterval--;
      delay = true;
    }
    else
      UpdateLED(mSeCounter& 0x0001);    //update the 2 LEDs (alternately)...
                                            //....at the appropraite level 
    if (!delay)
    {
      if (++mSeCounter >= MAX_LED_LEVEL * storyStepSpeed) //some multiple of MAX_LED_LEVEL
      {
        mSeCounter=0;
        
        if (++profileStepIndex >= scriptPtr->script[scriptStepIndex].profilesize)
        {
            if (evalVoltages())
                sleepReq=true;
            profileStepIndex=0;

            //turn off that last led addressed in the old profile
            int ledNum = scriptPtr->script[scriptStepIndex].profile[1].ledNum;
            UpdateGroupLines(ledStates[ledNum].groups[POS], TRISTATE_FLOAT);
            UpdateGroupLines(ledStates[ledNum].groups[NEG], TRISTATE_FLOAT);

            // goto next profile in script 
            if (scriptPtr->script[++scriptStepIndex].profile[0].ledNum == END_OF_SCRIPT)
            {
              scriptStepIndex=0;              //end of script
              if (--profileRepeatCounter != 0)
              {
                delayInterval = genrandIfNeg(scriptPtr->delayInterval, 0);
                storyStepSpeed = genrandIfNeg(scriptPtr->speed, 1);
              }
              else if (delayInterval==0)
              {
                if ((&storyPtr[++storyStepIndex])->delayInterval == ENDSTORIES)
                  storyStepIndex = 0;
                scriptPtr = &storyPtr[storyStepIndex];
                storyStepSpeed = genrandIfNeg(scriptPtr->speed, 1);
                profileRepeatCounter = scriptPtr->repeat;
              }
            }
        }
          
        for (int i=0; i<2; i++)
        {
          scriptEle_TypeDef scriptEle = scriptPtr->script[scriptStepIndex].profile[i];
          ledStates[scriptEle.ledNum].level = scriptPtr->script[scriptStepIndex].profile[i].profilePtr[profileStepIndex];          //get new level from profile
          ledStates[scriptEle.ledNum].curLevel=0;             //start over
        }
      }  
    }
}
