#pragma once
typedef struct
{
  u8*                profilePtr;
  u8                 ledNum;
}scriptEle_TypeDef;

typedef struct
{
  scriptEle_TypeDef  profile[2];
  u8                 profilesize;
}scriptScriptStep_TypeDef;

#define END_OF_SCRIPT 99

/*************************************************************/
/// Each level value of a profile is a "profileStep"
/// For now, all profiles must be same size within a scriptStep.
static u8   profileRampUp16[27]   =  {0,1,1,1,1,2,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8};
static u8   profileRampUp[11]   =  {0,1,1,2,2,3,4,5,6,7,8};
static u8   profileRampDn[11]   =  {8,7,6,5,4,3,2,2,1,1,0};
//static u8   profileRampUp[8]   =  {1,2,3,4,5,6,7,8};
//static u8   profileRampDn[8]   =  {8,7,6,5,4,3,2,1};
static u8   profileRampUpDn[8] =  {1,2,4,8,8,4,2,1};
static u8   off8[8] =             {0,0,0,0,0,0,0,0};
static u8   off[1] =              {0};
static u8   fullOn[1]   =         {8};
static u8   fullOn8[8]   =        {8,8,8,8,8,8,8,8};

///Each line(step) in the script is a "scriptStep"
static scriptScriptStep_TypeDef scriptBottomOn[] = {   
  { {{fullOn, 5},          {fullOn, 5}}, 1},
  { {{off, END_OF_SCRIPT}, {off, END_OF_SCRIPT}}, 1}
}; 
static scriptScriptStep_TypeDef scriptTopOn[] = {   
  { {{fullOn, 0},          {fullOn, 0}}, 1},
  { {{off, END_OF_SCRIPT}, {off, END_OF_SCRIPT}}, 1}
}; 

static scriptScriptStep_TypeDef scriptPause[] = {   
  { {{off8, 5},          {off8, 0}}, 8},
  { {{off, END_OF_SCRIPT}, {off, END_OF_SCRIPT}}, 1}
}; 

static scriptScriptStep_TypeDef scriptFadedDrip[] = {   
  { {{profileRampUp16,0},{profileRampUp16,0}}, sizeof(profileRampUp16)},
  { {{profileRampUp, 1}, {profileRampDn, 0}}, sizeof(profileRampUp)},
  { {{profileRampDn, 1}, {profileRampUp, 2}}, 8},
  { {{profileRampUp, 3}, {profileRampDn, 2}}, 8},
  { {{profileRampDn, 3}, {profileRampUp, 4}}, 8},
  { {{profileRampUp, 5}, {profileRampDn, 4}}, 8}, 
  { {{profileRampDn, 5}, {off8, 4}},          8}, 
  { {{off, END_OF_SCRIPT}, {off, END_OF_SCRIPT}}, 1}
}; 

static scriptScriptStep_TypeDef scriptDiscreteDrip[] = {
  { {{profileRampUpDn, 0}, {off8, 4}}, 8}, 
  { {{profileRampUpDn, 1}, {off8, 4}}, 8}, 
  { {{profileRampUpDn, 2}, {off8, 4}}, 8}, 
  { {{profileRampUpDn, 3}, {off8, 4}}, 8}, 
  { {{profileRampUpDn, 4}, {off8, 1}}, 8}, 
  { {{profileRampUpDn, 5}, {off8, 4}}, 8}, 
  { {{off, END_OF_SCRIPT}, {off, END_OF_SCRIPT}}, 1}
 };

static scriptScriptStep_TypeDef scriptZigZagGreen[] = {
  { {{fullOn, 1}, {fullOn, 5}}, 1}, 
  { {{fullOn, 2}, {fullOn, 4}}, 1}, 
  { {{fullOn, 3}, {off, 2}}, 1}, 
  { {{fullOn, 4}, {fullOn, 2}}, 1}, 
  { {{fullOn, 5}, {fullOn, 1}}, 1}, 
  { {{off, END_OF_SCRIPT}, {off, END_OF_SCRIPT}}, 1}
 };
static scriptScriptStep_TypeDef scriptWarpCore[] = {
  { {{fullOn, 1}, {fullOn, 5}}, 1}, 
  { {{fullOn, 1}, {fullOn, 1}}, 1},     //
  { {{fullOn, 2}, {fullOn, 4}}, 1}, 
  { {{fullOn, 3}, {off, 2}}, 1}, 
  { {{fullOn, 4}, {fullOn, 2}}, 1}, 
  { {{fullOn, 1}, {fullOn, 1}}, 1},     //
  { {{fullOn, 5}, {fullOn, 1}}, 1}, 
  
  { {{fullOn, 1}, {fullOn, 5}}, 1}, 
  { {{fullOn, 2}, {fullOn, 2}}, 1},     //
  { {{fullOn, 2}, {fullOn, 4}}, 1}, 
  { {{fullOn, 3}, {off, 2}}, 1}, 
  { {{fullOn, 4}, {fullOn, 2}}, 1}, 
  { {{fullOn, 2}, {fullOn, 2}}, 1},     //
  { {{fullOn, 5}, {fullOn, 1}}, 1}, 
  
  { {{fullOn, 1}, {fullOn, 5}}, 1}, 
  { {{fullOn, 3}, {fullOn, 3}}, 1},     //
  { {{fullOn, 2}, {fullOn, 4}}, 1}, 
  { {{fullOn, 3}, {off, 2}}, 1}, 
  { {{fullOn, 4}, {fullOn, 2}}, 1}, 
  { {{fullOn, 3}, {fullOn, 3}}, 1},     //
  { {{fullOn, 5}, {fullOn, 1}}, 1}, 
  
  { {{fullOn, 1}, {fullOn, 5}}, 1}, 
  { {{fullOn, 4}, {fullOn, 4}}, 1},     //
  { {{fullOn, 2}, {fullOn, 4}}, 1}, 
  { {{fullOn, 3}, {off, 2}}, 1}, 
  { {{fullOn, 4}, {fullOn, 2}}, 1}, 
  { {{fullOn, 4}, {fullOn, 4}}, 1},     //
  { {{fullOn, 5}, {fullOn, 1}}, 1}, 
  
  { {{fullOn, 1}, {fullOn, 5}}, 1}, 
  { {{fullOn, 5}, {fullOn, 5}}, 1},     //
  { {{fullOn, 2}, {fullOn, 4}}, 1}, 
  { {{fullOn, 3}, {off, 2}}, 1}, 
  { {{fullOn, 4}, {fullOn, 2}}, 1}, 
  { {{fullOn, 5}, {fullOn, 5}}, 1},     //
  { {{fullOn, 5}, {fullOn, 1}}, 1}, 

  { {{off, END_OF_SCRIPT}, {off, END_OF_SCRIPT}}, 1}
 };
static scriptScriptStep_TypeDef scriptWhiteBoom[] = {
  { {{fullOn, 0}, {fullOn, 0}}, 1}, 
  { {{fullOn, 0}, {fullOn, 0}}, 1}, 
  { {{fullOn, 0}, {fullOn, 0}}, 1}, 
  { {{fullOn, 0}, {fullOn, 0}}, 1}, 
  { {{profileRampDn, 0}, {profileRampDn, 0}}, 8}, 
  { {{off, END_OF_SCRIPT}, {off, END_OF_SCRIPT}}, 1}
 };
typedef struct
{
  scriptScriptStep_TypeDef*  script;
  char                         speed;             //# of MAX_LED_LEVELs spent at any setp; <0 -> rnd(x)*MAX_LED_LEVELs intervals
  u8                        repeat;            //how many times it executes
  signed short              delayInterval;     //between repeats (if repeat>1) >0 -> x*10ms intervals, 
}scriptScript_TypeDef;                          //                              <0 -> rnd(x)*10ms intervals


/*********************  STORIES ***********************************/
#define ENDSTORIES 0xffff
static scriptScript_TypeDef stories_Drip[] = {
  {scriptFadedDrip, -20, 1, 0},             //script, speed, repeat, pause
  {scriptPause, 10,10,0},
  {0, 15,1,ENDSTORIES}
};
static scriptScript_TypeDef stories_discreteDrip[] = {
  {scriptFadedDrip, -9, 1, -80},             //script, speed, repeat, pause
  {0, 15,1,ENDSTORIES}
};
static scriptScript_TypeDef stories_ZigZagGreenBoom[] = {
  {scriptFadedDrip, -9, 17, -80},             //script, speed, repeat, pause
  
  {scriptPause, 1,127,0},               
  
  {scriptDiscreteDrip, -9, 17, -80},

  {scriptZigZagGreen, 40, 1, 0}, 
  {scriptZigZagGreen, 30, 1, 0}, 
  {scriptZigZagGreen, 20, 1, 0}, 
  {scriptZigZagGreen, 14, 1, 0}, 
  {scriptZigZagGreen, 10, 1, 0}, 
  {scriptZigZagGreen, 8, 2, 0}, 
  {scriptZigZagGreen, 6, 4, 0}, 
  {scriptZigZagGreen, 3, 12, 0}, 
  {scriptZigZagGreen, 2, 24, 0}, 
  {scriptWhiteBoom, 15, 1, 0}, 
  {scriptPause, 10,10,0},
  {0, 15,1,ENDSTORIES}
};
static scriptScript_TypeDef stories_WarpCore[] = {
  {scriptWarpCore, 3, 17, 0}, //script, speed, repeat, pause
  {scriptWarpCore, 2, 30, 0},           
  {0, 15,1,ENDSTORIES}
};
static scriptScript_TypeDef stories_BottomOn[] = {
  {scriptBottomOn, 2, 1, 0},             //script, speed, repeat, pause
 {0, 15,1,ENDSTORIES}
};
static scriptScript_TypeDef stories_TopOn[] = {
  {scriptTopOn, 2, 1, 0},             //script, speed, repeat, pause
 {0, 15,1,ENDSTORIES}
};

static scriptScript_TypeDef* storyPtrs[5] = {
  stories_Drip,
  stories_ZigZagGreenBoom,
  stories_WarpCore,
  stories_TopOn,
  stories_BottomOn
};
