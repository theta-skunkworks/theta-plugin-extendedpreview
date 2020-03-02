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

#ifndef BATTERY_H
#define BATTERY_H


#define I2CADR_IP5306       0x75
#define IP5306_BAT_GAUGE    0x78
#define IP5306_REG_READ0    0x70
#define IP5306_REG_READ1    0x71
#define MASK_BAT_GAUGE      0xF0
#define MASK_BIT3           0x08

#define CHECK_BAT_CYCLE_NORM      1000  //ms
#define CHECK_BAT_CYCLE_CONNECT   5000  //ms

unsigned char ucBatGauge = 0xFF;
bool bCharging = false;
bool bFullCharge = false;

unsigned long ulCheckBatStart;


uint8_t getIP5306Reg(uint8_t readAddr, bool* pbfailure);
uint8_t getBatteryGauge(void);
void  CheckBatteryStatus(void);
void CheckBatteryTimming(void);


#endif  //BATTERY_H
