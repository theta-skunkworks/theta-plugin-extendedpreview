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

#ifndef THETA_IMAGES_H
#define THETA_IMAGES_H

extern uint8_t theta_logo[];
extern uint8_t Lv_Non[];
extern uint8_t icon_THETA[];
extern uint8_t icon_m5stack[];
extern uint8_t icon_Lv_Off[];
extern uint8_t icon_Lv_On[];
extern uint8_t icon_Move_REC[];
extern uint8_t icon_Setting[];
extern uint8_t icon_SMStat_Move[];
extern uint8_t icon_SMStat_Still[];
extern uint8_t icon_Still_Mode0[];
extern uint8_t icon_Still_Mode1[];
extern uint8_t icon_Still_Mode2[];
extern uint8_t icon_Still_Mode3[];
extern uint8_t WB_00[];
extern uint8_t WB_01[];
extern uint8_t WB_02[];
extern uint8_t WB_03[];
extern uint8_t WB_04[];
extern uint8_t WB_05[];
extern uint8_t WB_06[];
extern uint8_t WB_07[];
extern uint8_t WB_08[];
extern uint8_t WB_09[];
extern uint8_t WB_10[];


const int iSize_theta_logo       = 9091;
const int iSize_Lv_Non           = 3652;
const int iSize_icon_THETA       = 1849;
const int iSize_icon_m5stack     = 2526;
const int iSize_icon_Lv_Off      = 2302;
const int iSize_icon_Lv_On       = 2110;
const int iSize_icon_Move_REC    = 3277;
const int iSize_icon_Setting     = 1878;
const int iSize_icon_SMStat_Move = 1425;
const int iSize_icon_SMStat_Still= 1538;
const int iSize_icon_Still_Mode0 = 1846;
const int iSize_icon_Still_Mode1 = 1746;
const int iSize_icon_Still_Mode2 = 1662;
const int iSize_icon_Still_Mode3 = 1635;
const int iSize_WB_00            = 1640;
const int iSize_WB_01            = 1800;
const int iSize_WB_02            = 1675;
const int iSize_WB_03            = 1705;
const int iSize_WB_04            = 1611;
const int iSize_WB_05            = 1670;
const int iSize_WB_06            = 1693;
const int iSize_WB_07            = 1692;
const int iSize_WB_08            = 1695;
const int iSize_WB_09            = 1651;
const int iSize_WB_10            = 1790;


#include "THETA_API_2_1.h"

unsigned char* pucList_WB_Image[LIST_NUM_WB] = {
                WB_00,
                WB_01,
                WB_02,
                WB_03,
                WB_04,
                WB_05,
                WB_06,
                WB_07,
                WB_08,
                WB_09,
                WB_10
    };

unsigned int  iList_WB_ImageSize[LIST_NUM_WB] = {
                iSize_WB_00,
                iSize_WB_01,
                iSize_WB_02,
                iSize_WB_03,
                iSize_WB_04,
                iSize_WB_05,
                iSize_WB_06,
                iSize_WB_07,
                iSize_WB_08,
                iSize_WB_09,
                iSize_WB_10
    };



#endif	//THETA_IMAGES_H
