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

#ifndef DISPLAY_H
#define DISPLAY_H


// BRIGHTNESS = MIN 0 - MAX 255
#define   LCD_BRIGHT_LOW    32
#define   LCD_BRIGHT_NORM   64
#define   LCD_BRIGHT_HIGH1  128
#define   LCD_BRIGHT_HIGH2  255
int iLcdBrightness = LCD_BRIGHT_NORM;


#define DISP_MODE_CNCT      0
#define DISP_MODE_PORT      1
#define DISP_MODE_SCAN      2
#define DISP_MODE_NORM      3
int iDispMode = DISP_MODE_CNCT;

#define DISP_BAT_POS_X  300
#define DISP_BAT_POS_Y  225


void  Disp_StatusArea(void);
void  Disp_TakePicture(int iX, int iY);
void  Disp_Quality(int iX, int iY);
void  Disp_LcdBrightness(int iX, int iY);

void  Disp_ConnectTheta(void);
void  Disp_SearchTheta(void);

void  Disp_M5BAT(int iX, int iY, unsigned char ucBatGauge, bool bCharging, bool bFullCharge);


#endif	//DISPLAY_H
