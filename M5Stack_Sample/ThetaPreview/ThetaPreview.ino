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

#include <driver/dac.h>
#include <M5Stack.h>      // M5STACK
#include "Free_Fonts.h"   // Include the header file attached to this sketch

#include "Connect.h"
#include "KeyControl.h"
#include "ThetaControl.h"
#include "theta_images.h"
#include "Display.h"
#include "Preview.h"
#include "Battery.h"
#include "Debug.h"

const char sVersion[] = "v00.00";

void    Core0Task(void * pvParameters);

void setup() {
  Serial.begin(115200);        // SERIAL
  M5.begin();                  // M5STACK INITIALIZE
  
  //Measures against speaker noise
  dac_output_disable(DAC_CHANNEL_1);
  dacWrite(25, 0);

  //Init Screen
  M5.Lcd.setBrightness(iLcdBrightness);
  M5.Lcd.fillScreen(BLACK);    // CLEAR SCREEN
  M5.Lcd.setRotation(1);       // SCREEN ROTATION = 1 : Current horizontal display.

  //Init debug pin's
  pinMode(DEBUG_PIN_1, OUTPUT);
  pinMode(DEBUG_PIN_2, OUTPUT);
  digitalWrite(DEBUG_PIN_1, LOW);
  digitalWrite(DEBUG_PIN_2, LOW);

  //Display THETA Logo & Version
  M5.Lcd.drawJpg(theta_logo, iSize_theta_logo);
  M5.Lcd.setCursor(140, 210);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf(sVersion);
  delay(500);
  M5.Lcd.fillScreen(BLACK);    // CLEAR SCREEN
  delay(500);

  // Define EEPROM SIZE
  EEPROM.begin(EEPROM_SIZE);    //Max 4096Byte

  //Read TEHTA SSID from EEPROM & check
  char SsidBuff[THETA_SSID_BYTE_LEN+1];
  ReadThetaSsid(SsidBuff);  
  if ( CheckThetaSsid( (const char*)SsidBuff ) == 1 ) {
    memcpy( ssid, SsidBuff, THETA_SSID_BYTE_LEN );
    SetThetaSsidToPassword(SsidBuff, password);
    Serial.println("");
    Serial.print("EEPROM SSID=");
    Serial.println(ssid);
    Serial.print("       PASS=");
    Serial.println(password);
  } else {
    iConnectOrScan = WIFI_SCAN_THETA;
  }

  //Initi chack battery
  Wire.begin();
  delay(10);
  CheckBatteryStatus();
  ulCheckBatStart=millis();
  
  //Init fps counter
  ulInFpsCntStart=millis();
  ulOutFpsCntStart=millis();

  //Init Live-Preview Buffer
  static T_LV_BUFF stLvBuff;
  pstLvBuff=&stLvBuff;
  pstLvBuff->iWP=0;
  pstLvBuff->iRP=0;
  pstLvBuff->staLvFrame[0].iSize = 0;
  pstLvBuff->staLvFrame[1].iSize = 0;
  
  // Task Wake Up
  xTaskCreatePinnedToCore(
                  Core0Task,    /* Function to implement the task */
                  "Core0Task",  /* Name of the task */
                  8192,         /* Stack size in words */
                  NULL,         /* Task input parameter */
                  1,            /* Priority of the task */
                  NULL,         /* Task handle. */
                  0);           /* Core where the task should run */
  
  
}

void loop() {
  //bool  bDispChg = false;
  int   iRet;

  if ( WiFi.status() != WL_CONNECTED ) {
    
    if (iConnectOrScan == WIFI_CONNECT_THETA) {
      iDispMode = DISP_MODE_CNCT;
      CheckBatteryStatus();
      
      iConnectOrScan = ConnectTHETA();
    } else if (iConnectOrScan == WIFI_SCAN_THETA) {
      iDispMode = DISP_MODE_SCAN;
      CheckBatteryStatus();
      
      if( SearchAndEnterTHETA() == 1 ) {
        ReadThetaSsid(ssid);
        SetThetaSsidToPassword(ssid, password);
        iConnectOrScan = WIFI_CONNECT_THETA;
      }
      delay(1000);
    } else {
      iDispMode = DISP_MODE_CNCT;
      CheckBatteryStatus();
      
      Serial.print("\n");
      Serial.println("Force Disconnect. : " + String(iConnectOrScan));
      Serial.print("\n");
      
      WiFi.disconnect(true);
      delay(1000);
      iConnectOrScan = WIFI_CONNECT_THETA;
    }
  } else {
    if (iConnectOrScan == WIFI_CHECK_PORT) {
      iDispMode = DISP_MODE_PORT;
      CheckBatteryStatus();
      
      Serial.print("\n");
      Serial.print("Check Port\n");
      Serial.print("\n");
      iConnectOrScan = CheckPort(10, 1000);
      if (iConnectOrScan==WIFI_CONNECTED) {
        delay(100);
        iRet = ExtAPI_Post_startPreview();        
        if (iRet<0) {
          iConnectOrScan = WIFI_CHECK_PORT;
          Serial.print("\'LivePreview plugin\' is not running\n");
          delay(500);
        }
      } else {
        Serial.print("Waiting for \'LivePreview plugin\' to start.\n");
        delay(500);
      }
    } else {
      
      digitalWrite(DEBUG_PIN_1, HIGH); //debug
      iDispMode = DISP_MODE_NORM;

      //-----------------------------------------------------
      int iUpdateLvRet = updateLivePreview();
      //-----------------------------------------------------
      if (iUpdateLvRet==1) {
        iInFps++;
      }
      
      // Output debug info
      Debug_OutputInFps();
      
      // Camera condition monitoring
      if ( 0<=iUpdateLvRet ) {
        if (bReqTp) {
          ThetaAPI_Post_State();
          if ( (iTakePicStat == TAKE_PIC_STAT_DONE) && strCaptureStatus.equals("idle") ) {
            ThetaAPI_Post_takePicture();
          }
          bReqTp=false;
        }
        if (iTakePicStat != TAKE_PIC_STAT_DONE) {
          ThetaAPI_Post_commnads_status();
          if ( iTakePicStat == TAKE_PIC_STAT_DONE ) {
              delay(10);
              ExtAPI_Post_startPreview();
          }
        }
      }
      
      CheckBatteryTimming(CHECK_BAT_CYCLE_NORM);
      
      digitalWrite(DEBUG_PIN_1, LOW); //debug
      delay(1); //Give a gap to update watchdog timer
    }
  }
}

void    Core0Task(void * pvParameters)
{
  int iLoopDelay=0;
  int iKeyRet;
  int iDispRet;
  int iBeforeDispMode = DISP_MODE_CNCT;
  bool bDispChgFlag = false;
  
  unsigned long ulBeforeMs;
  unsigned long ulCurMs;
  unsigned long ulElapsedMs;

  for(;;) {
    if (iDispMode!=iBeforeDispMode) {
      M5.Lcd.fillScreen(BLACK);    // CLEAR SCREEN
      bDispChgFlag = true;
    } else {
      bDispChgFlag = false;
    }
    iBeforeDispMode = iDispMode;
    
    if ( iDispMode == DISP_MODE_CNCT ) {
      Disp_ConnectTheta();
      iLoopDelay=500;
    } else if ( iDispMode == DISP_MODE_PORT ) {
      Disp_ConnectTheta();
      iLoopDelay=100;
    } else if ( iDispMode == DISP_MODE_SCAN ) {
      Disp_M5BAT(DISP_BAT_POS_X, DISP_BAT_POS_Y, ucBatGauge, bCharging, bFullCharge);
      Disp_SearchTheta();
      iLoopDelay=500;
    } else if ( iDispMode == DISP_MODE_NORM ) {
      
      digitalWrite(DEBUG_PIN_2, HIGH); //debug
      
      // Key operation monitoring
      iKeyRet = KeyControl();

      // Dislpay Live-Preview Frame
      ulBeforeMs = millis();
      iDispRet = DisplayLivePreview();
      if ( iDispRet == 1 ) {
        iOutFps++;
        ulCurMs = millis();
        ulElapsedMs = calcDiffTime(ulBeforeMs, ulCurMs);
        Serial.println( "JPEG Disp time=" + String(ulElapsedMs) + "[ms]");
      }
      
      // Display Status Area
      if ( (iKeyRet==KEYCTRL_DISP_CHG) || bDispChgFlag ) {
        Disp_StatusArea();
        Disp_M5BAT(300, 225, ucBatGauge, bCharging, bFullCharge);
      }
      
      // Output debug info
      Debug_OutputOutFps();

      digitalWrite(DEBUG_PIN_2, LOW); //debug
      iLoopDelay=1; //Give a gap to update watchdog timer
    }
    
    Disp_M5BAT(DISP_BAT_POS_X, DISP_BAT_POS_Y, ucBatGauge, bCharging, bFullCharge);
    delay(iLoopDelay);
    M5.update();
  }
}
