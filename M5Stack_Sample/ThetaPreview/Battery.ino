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

int getIP5306Reg(uint8_t readAddr, int iReadSize, uint8_t* pucaResult)
{
  int   iReadCnt=0;
  
  Wire.beginTransmission(I2CADR_IP5306);
  Wire.write(readAddr);
  if (Wire.endTransmission(false)==0) {
    Wire.requestFrom(I2CADR_IP5306,iReadSize);
    while(Wire.available()){
      if (iReadCnt<iReadSize) {
        *(pucaResult+iReadCnt) = Wire.read();
        iReadCnt++;
      }
    }
  } else {
    iReadCnt=-1;
  }
  return iReadCnt;
}

uint8_t getBatteryGauge(void)
{
  int iSize;
  uint8_t ucResult;
  
  iSize = getIP5306Reg(IP5306_BAT_GAUGE, 1, &ucResult);
  if (iSize==1) {
    ucResult = ucResult & MASK_BAT_GAUGE;
    switch(ucResult){
      case 0xF0 : //empty (3.2v or less)
        ucResult=0;
        break;
      case 0xE0 : //Less than 25%
        ucResult=25;
        break;
      case 0xC0 : //Less than 50%
        ucResult=50;
        break;
      case 0x80 : //Less than 75%
        ucResult=75;
        break;
      case 0x00 : //Full
        ucResult=100;
        break;
      default :   //Undefined
        ucResult=0xFE;
    }
  } else {
    ucResult = 0xFF;
  }
  return ucResult;
}

void  CheckBatteryStatus(void)
{
  uint8_t ucaREG_READ[2] = {0x00, 0x00};
  int iSize=0;
  
  ucBatGauge = getBatteryGauge();
  if ( (0<=ucBatGauge) && (ucBatGauge<=100) ) {
    iSize = getIP5306Reg(IP5306_REG_READ0, 2, ucaREG_READ);
    if (iSize==2) {
      if ( (ucaREG_READ[0]&MASK_BIT3) != 0 ) {
        bCharging=true;
      } else {
        bCharging=false;
      }
      if ( (ucaREG_READ[1]&MASK_BIT3) != 0 ) {
        bFullCharge=true;
      } else {
        bFullCharge=false;
      }
    }
  } else {
    bCharging=false;
    bFullCharge=false;
  }
  Serial.printf("\n");
  Serial.printf("battery=%d \n", ucBatGauge);
  Serial.printf("REG_READ0=0x%02X, REG_READ1=0x%02X, result=%d \n", ucaREG_READ[0], ucaREG_READ[1], iSize);
  Serial.printf("\n");
}

void CheckBatteryTimming(unsigned long ulCycle)
{
  unsigned long ulCurMs = millis();
  unsigned long ulElapsedMs = calcDiffTime(ulCheckBatStart, ulCurMs);
  if ( ulElapsedMs >= ulCycle ) {
    CheckBatteryStatus();
    ulCheckBatStart = millis();
  }
}
