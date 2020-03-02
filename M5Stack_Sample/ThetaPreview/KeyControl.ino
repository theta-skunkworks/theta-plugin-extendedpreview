/**
 * Copyright 2018 Ricoh Company, Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

int  LognPressCntA = 0;
int  LognPressCntB = 0;
int  LognPressCntC = 0;

bool bLongPressFlagA = false;
bool bLongPressFlagB = false;
bool bLongPressFlagC = false;
bool bLongPressOneTimeA = false;
bool bLongPressOneTimeB = false;
bool bLongPressOneTimeC = false;

#define KEY_LONGPRESS_MS    1000
#define KEY_RET_NOP         0
#define KEY_RET_SHORT       1
#define KEY_RET_LONG        2

//-------------------------------------------
// Bottun Read functions
//-------------------------------------------
int  KeyReadA(void)
{
  int iRet = KEY_RET_NOP;
  
  //--- BtnA Read ---
  if ( M5.BtnA.wasPressed() ) {
    bLongPressFlagA=false;
    bLongPressOneTimeA=false;
    LognPressCntA=0;
  }
  if ( M5.BtnA.wasReleased() ) {
    if (bLongPressFlagA == false) {
      iRet = KEY_RET_SHORT;
    }
  } else {
    if ( M5.BtnA.pressedFor(KEY_LONGPRESS_MS) ) {   // Pressed for more than the specified time[ms]
      bLongPressFlagA=true;
      if ( !bLongPressOneTimeA ) {
        bLongPressOneTimeA=true;
        iRet = KEY_RET_LONG;
      }
    } else {
      //NOP
    }
  }
  
  return (iRet);
}

int  KeyReadB(void)
{
  int iRet = KEY_RET_NOP;
  
  //--- BtnB Read ---
  if ( M5.BtnB.wasPressed() ) {
    bLongPressFlagB=false;
    bLongPressOneTimeB=false;
    LognPressCntB=0;
  }
  if ( M5.BtnB.wasReleased() ) {
    if (bLongPressFlagB == false) {
      iRet = KEY_RET_SHORT;
    }
  } else {
    if ( M5.BtnB.pressedFor(KEY_LONGPRESS_MS) ) {   // Pressed for more than the specified time[ms] 
      bLongPressFlagB=true;
      if ( !bLongPressOneTimeB ) {
        bLongPressOneTimeB=true;
        iRet = KEY_RET_LONG;
      }
    } else {
      //NOP
    }
  }
  
  return (iRet);
}

int  KeyReadC(void)
{
  int iRet = KEY_RET_NOP;
  
  //--- BtnC Read ---
  if ( M5.BtnC.wasPressed() ) {
    bLongPressFlagC=false;
    bLongPressOneTimeC=false;
    LognPressCntC=0;
  }
  if ( M5.BtnC.wasReleased() ) {
    if (bLongPressFlagC == false) {
      iRet = KEY_RET_SHORT;
    }
  } else {
    if ( M5.BtnC.pressedFor(KEY_LONGPRESS_MS) ) {   // Pressed for more than the specified time[ms]
      bLongPressFlagC=true;
      if ( !bLongPressOneTimeC ) {
        bLongPressOneTimeC=true;
        iRet = KEY_RET_LONG;
      }
    } else {
      //NOP
    }
  }
  
  return (iRet);
}

int  KeyControl(void)
{
  int iRet = KEYCTRL_DISP_NOP;
  int iKeyVal;
  String Result ;
  
  //--- Center Button ---
  iKeyVal = KeyReadB();
  if (iKeyVal == KEY_RET_SHORT) {
    Serial.print("Btn B Short\n");

    bReqTp=1;
    iRet = KEYCTRL_DISP_CHG;
    
  } else if (iKeyVal == KEY_RET_LONG) {
    Serial.print("Btn B Long\n");
    //NOP
  }

  //--- Left Button ---
  iKeyVal = KeyReadA();
  if (iKeyVal == KEY_RET_SHORT) {
    Serial.print("Btn A Short\n");

    if ( iPreviewQuality == LV_QUALITY_LOW ) {
      iPreviewQuality = LV_QUALITY_MID;
    } else if (iPreviewQuality == LV_QUALITY_MID) {
      iPreviewQuality = LV_QUALITY_HIGH;
    } else {
      iPreviewQuality = LV_QUALITY_LOW;
    }
    Serial.printf("\nPreviewQuality = %d\n\n", iPreviewQuality);
    iRet = KEYCTRL_DISP_CHG;
    
  } else if (iKeyVal == KEY_RET_LONG) {
    Serial.print("Btn A Long\n");
    //NOP
  }

  //--- Right Button ---
  iKeyVal = KeyReadC();
  if (iKeyVal == KEY_RET_SHORT) {
    Serial.print("Btn C Short\n");

    if ( iLcdBrightness == LCD_BRIGHT_LOW ) {
      iLcdBrightness = LCD_BRIGHT_NORM;
    } else if (iLcdBrightness == LCD_BRIGHT_NORM) {
      iLcdBrightness = LCD_BRIGHT_HIGH1;
    } else if (iLcdBrightness == LCD_BRIGHT_HIGH1) {
      iLcdBrightness = LCD_BRIGHT_HIGH2;
    } else {
      iLcdBrightness = LCD_BRIGHT_LOW;
    }
    M5.Lcd.setBrightness(iLcdBrightness);
    Serial.printf("\nLcdBrightness = %d\n\n", iLcdBrightness);
    iRet = KEYCTRL_DISP_CHG;

  } else if (iKeyVal == KEY_RET_LONG) {
    Serial.print("Btn C Long\n");
    //NOP
  }
  
  return (iRet);
}
