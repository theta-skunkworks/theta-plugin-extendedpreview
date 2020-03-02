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

#define CURSOR_Y              170
#define CURSOR_LINE1          (CURSOR_Y+15)
#define CURSOR_LINE2          (CURSOR_Y+35)
#define CURSOR_LINE3          (CURSOR_Y+55)

//-------------------------------------------
// Display functions
//-------------------------------------------
void  Disp_StatusArea(void)
{
  M5.Lcd.drawLine(  0, 161, 320, 161, WHITE);
  
  int iSCursorY = 170;
  int iSCursorX1 = 30;
  int iSCursorX2 = 135;
  int iSCursorX3 = 225;

  Disp_Quality(iSCursorX1, iSCursorY);
  Disp_TakePicture(iSCursorX2, iSCursorY);
  Disp_LcdBrightness(iSCursorX3, iSCursorY);
  
  return ;    
}

void  Disp_TakePicture(int iX, int iY)
{
  M5.Lcd.setCursor(iX, CURSOR_LINE1);
  M5.Lcd.printf("Take");
  M5.Lcd.setCursor(iX, CURSOR_LINE2);
  M5.Lcd.printf("Picture");
  
  return ;    
}

void  Disp_Quality(int iX, int iY)
{
  M5.Lcd.setCursor(iX, CURSOR_LINE1);
  M5.Lcd.printf("Preview");
  M5.Lcd.setCursor(iX, CURSOR_LINE2);
  M5.Lcd.printf("Quality");
  
  M5.Lcd.fillRect(iX, CURSOR_LINE3-14, 30, 18, BLACK);
  M5.Lcd.setCursor(iX, CURSOR_LINE3);
  M5.Lcd.printf("%d", iPreviewQuality);
  
  return ;    
}

void  Disp_LcdBrightness(int iX, int iY)
{
  M5.Lcd.setCursor(iX+2, CURSOR_LINE1);
  M5.Lcd.printf("LCD");
  M5.Lcd.setCursor(iX+2, CURSOR_LINE2);
  M5.Lcd.printf("Brightness");
  
  M5.Lcd.fillRect(iX, CURSOR_LINE3-14, 40, 18, BLACK);
  M5.Lcd.setCursor(iX, CURSOR_LINE3);
  M5.Lcd.printf(" %d", iLcdBrightness);
  
  return ;    
}


#define CONNECT_CIRCLE_NUM          5
#define CONNECT_CIRCLE_COLOR_OFF    0x39E7
#define CONNECT_CIRCLE_COLOR_ON     0xFFFF
int     iDispCnt_ConnectTheta = (CONNECT_CIRCLE_NUM-1);
void  Disp_ConnectTheta(void)
{
  int       iCnt;
  int       iDispX = 103;
  int       iDispY = 110;
  int       iDispR = 5;
  int       iDispX_Offset = iDispR * 2 + 8;
  uint16_t  ausColor[CONNECT_CIRCLE_NUM];
  
  M5.Lcd.drawJpg(icon_m5stack, iSize_icon_m5stack,  200, 70, 80, 80, 0, 0, JPEG_DIV_NONE);
  M5.Lcd.drawJpg(icon_THETA,   iSize_icon_THETA,     45, 76, 36, 72, 0, 0, JPEG_DIV_NONE);

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setFreeFont(FSS12);
  M5.Lcd.setTextSize(1);
  
  if ( iDispMode == DISP_MODE_CNCT ) {
    M5.Lcd.setCursor(10, 200);
    M5.Lcd.printf("Connecting to");
    M5.Lcd.setCursor(10, 225);
    M5.Lcd.printf("%s", ssid);
  } else if ( iDispMode == DISP_MODE_PORT ) {
    M5.Lcd.setCursor(15, 30);
    M5.Lcd.printf("Waiting for plug-in response");
    M5.Lcd.setCursor(10, 200);
    M5.Lcd.printf("Connected to");
    M5.Lcd.setCursor(10, 225);
    M5.Lcd.printf("%s", ssid);
  }
  M5.Lcd.setFreeFont(FSS9);

  

  for (iCnt=0; iCnt<CONNECT_CIRCLE_NUM; iCnt++){
    if ( iCnt == iDispCnt_ConnectTheta ) {
      ausColor[iCnt] = CONNECT_CIRCLE_COLOR_ON;
    } else {
      ausColor[iCnt] = CONNECT_CIRCLE_COLOR_OFF;
    }
    M5.Lcd.fillCircle( iDispX+iDispX_Offset*iCnt, iDispY, iDispR, ausColor[iCnt]);
  }
  iDispCnt_ConnectTheta--;
  if ( iDispCnt_ConnectTheta < 0) {
    iDispCnt_ConnectTheta = CONNECT_CIRCLE_NUM;
  }
  
  return ;  
}

void  Disp_SearchTheta(void)
{
  //M5.Lcd.fillScreen(BLACK);     // CLEAR SCREEN
  
  M5.Lcd.drawJpg(icon_m5stack, iSize_icon_m5stack,  200, 70, 80, 80, 0, 0, JPEG_DIV_NONE);
  M5.Lcd.drawJpg(icon_THETA,   iSize_icon_THETA,     45, 76, 36, 72, 0, 0, JPEG_DIV_NONE);

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setFreeFont(FSS12);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(80, 225);
  M5.Lcd.printf("Search THETA");
  M5.Lcd.setFreeFont(FSS9);
  
  int iEntCnt=0;
  int iWp=0;
  //int iAftImgNum=60;
  int iAftImgNum=40;
  float fAfterimage[iAftImgNum][2];
  unsigned short usColor[iAftImgNum];

  //Set gradation
  unsigned short usGreen=0x3F;
  for (int i=0; i<iAftImgNum; i++) {
    usColor[i]= 0x07E0&(usGreen<<5);
    usGreen -= 1;
  }
  
  float R = 110.0;
  for (float fDeg=60.0; fDeg>-60.0; fDeg-=0.5) {
    float fX = R*cos( (fDeg/180)*PI );
    float fY = R*sin( (fDeg/180)*PI );
    
    fAfterimage[iWp][0] = fX;
    fAfterimage[iWp][1] = fY;
    iWp++; if (iWp>iAftImgNum){iWp=0;}
    iEntCnt++; if (iEntCnt>iAftImgNum) {iEntCnt=iAftImgNum;}
    
    int iDrawPos = iWp;
    for (int iAftImg=0; iAftImg<iEntCnt; iAftImg++) {
      iDrawPos--;
      if ( iDrawPos<0 ){ iDrawPos=(iAftImgNum-1); }
      M5.Lcd.drawLine( 195-(int)(fAfterimage[iDrawPos][0]+0.5), 110-(int)(fAfterimage[iDrawPos][1]+0.5),  195, 110,  usColor[iAftImg] );
    }
    M5.Lcd.drawLine( 195-(int)(fAfterimage[iDrawPos][0]+0.5), 110-(int)(fAfterimage[iDrawPos][1]+0.5),  195, 110,  0x0000 );
    //delay(1);
  }
  int iAftColorPos=0;
  while(iEntCnt>0) {
    int iDrawPos = iWp;
    for (int iAftImg=0; iAftImg<iEntCnt; iAftImg++) {
      iDrawPos--;
      if ( iDrawPos<0 ){ iDrawPos=(iAftImgNum-1); }
      M5.Lcd.drawLine( 195-(int)(fAfterimage[iDrawPos][0]+0.5), 110-(int)(fAfterimage[iDrawPos][1]+0.5),  195, 110,  usColor[iAftColorPos+iAftImg] );
    }
    M5.Lcd.drawLine( 195-(int)(fAfterimage[iDrawPos][0]+0.5), 110-(int)(fAfterimage[iDrawPos][1]+0.5),  195, 110,  0x0000 );
    iEntCnt--;
    iAftColorPos++;
  }
  
  return ;  
}


#define DISP_BAT_WIDTH    16
#define DISP_BAT_HEIGHT   11
#define DISP_BAT_G_WIDTH  (DISP_BAT_WIDTH-6)
#define DISP_BAT_G_HEIGHT (DISP_BAT_HEIGHT-4)
void  Disp_M5BAT(int iX, int iY, unsigned char ucBatGauge, bool bCharging, bool bFullCharge)
{
  uint32_t color1;
  uint32_t color2;

  if ( 0<=ucBatGauge && ucBatGauge<=100 ) {
    if ( 0<=ucBatGauge && ucBatGauge<25 ) {
      color2 = RED;
      color1 = RED;
    } else if ( 25<=ucBatGauge && ucBatGauge<50 ) {
      color2 = ORANGE;
      color1 = WHITE;
    } else if ( 50<=ucBatGauge && ucBatGauge<75 ) {
      color2 = YELLOW;
      color1 = WHITE;
    } else if ( 75<=ucBatGauge && ucBatGauge<100 ) {
      color2 = GREEN;
      color1 = WHITE;
    } else {
      if (bFullCharge) {
        color2 = BLUE;
        color1 = WHITE;
      } else {
        color2 = GREEN;
        color1 = WHITE;
      }
    }
  } else {
    if ( ucBatGauge==0xFF ) {   //Unavailable due to old model(IP5306 does not support I2C)
      color1 = DARKGREY;
    } else {                    //Undefined
      color1 = YELLOW;
    }
    ucBatGauge=100;
    color2 = BLACK;
  }
  //Frame
  M5.Lcd.drawRect(iX+2, iY  , (DISP_BAT_WIDTH-2) , DISP_BAT_HEIGHT       , color1);
  M5.Lcd.drawLine(iX+1, iY+3, iX+1               , (iY+DISP_BAT_HEIGHT-4), color1);
  M5.Lcd.drawLine(iX  , iY+3, iX                 , (iY+DISP_BAT_HEIGHT-4), color1);
  //Gap
  M5.Lcd.drawRect(iX+3, iY+1, (DISP_BAT_WIDTH-4) , (DISP_BAT_HEIGHT-2) , BLACK);
  //Gauge
  int iGaugeWidth = (ucBatGauge*DISP_BAT_G_WIDTH)/100;
  M5.Lcd.fillRect((iX+4)                               , iY+2, (DISP_BAT_G_WIDTH-iGaugeWidth), DISP_BAT_HEIGHT-4, BLACK );
  M5.Lcd.fillRect((iX+4)+(DISP_BAT_G_WIDTH-iGaugeWidth), iY+2, iGaugeWidth                   , DISP_BAT_HEIGHT-4, color2);
  //Lightning sign
  if (bCharging) {
    M5.Lcd.fillTriangle( iX+4, iY+(DISP_BAT_HEIGHT/2), iX+2+((DISP_BAT_WIDTH-2)/2), iY+(DISP_BAT_HEIGHT/2), iX+2+((DISP_BAT_WIDTH-2)/2), iY+2, WHITE);
    M5.Lcd.fillTriangle( iX+DISP_BAT_WIDTH-3, iY+(DISP_BAT_HEIGHT/2), iX+1+((DISP_BAT_WIDTH-2)/2), iY+(DISP_BAT_HEIGHT/2), iX+1+((DISP_BAT_WIDTH-2)/2), iY+DISP_BAT_HEIGHT-3, WHITE);
  }
  
  return ;    
}
