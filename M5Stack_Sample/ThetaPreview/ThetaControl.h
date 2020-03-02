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

#ifndef THETA_CONTROL_H
#define THETA_CONTROL_H

#include "Connect.h"
#include <ArduinoJson.h>  // Add JSON Library  https://github.com/bblanchon/ArduinoJson
#include "THETA_API_2_1.h"

String  strLastFileURL = "";

#define   TAKE_PIC_STAT_DONE  0
#define   TAKE_PIC_STAT_BUSY  1

#define   INT_EXP_OFF         0
#define   INT_EXP_ON          1

#define   INT_EXP_STAT_STOP   0
#define   INT_EXP_STAT_RUN    1

#define   MOVE_STAT_STOP      0
#define   MOVE_STAT_REC       1

int     iTakePicStat      = TAKE_PIC_STAT_DONE;
int     iIntExpStat       = INT_EXP_STAT_STOP;    //For expansion
int     iIntExpOnOff      = INT_EXP_OFF;          //For expansion
int     iMoveStat         = MOVE_STAT_STOP;       //For expansion
int     iRecordedTime     = 0;                    //For expansion
int     iRecordableTime   = 0;                    //For expansion
String  strTakePicLastId  = "0";
String  strCaptureStatus  = "unknown";
//String  strBattLevel      = "unknown";
int     iBattLevel        = -1;


#define   HTTP_TIMEOUT_DISABLE  0 // never times out during transfer. 
#define   HTTP_TIMEOUT_NORMAL   1
#define   HTTP_TIMEOUT_STATE    2
#define   HTTP_TIMEOUT_STARTCAP 5
#define   HTTP_TIMEOUT_STOPCAP  70
#define   HTTP_TIMEOUT_CMDSTAT  2
#define   HTTP_TIMEOUT_LV       20  //sec


String  HttpPostGetString(const char* sPostGet, char* sUrl, String strData, unsigned int uiTimeoutSec);
int     HttpPostGet(const char* sPostGet, char* sUrl, String strData, unsigned char* pucaBuff, int iBuffSize, unsigned int uiTimeoutSec);

int     ThetaAPI_Post_State(void);
int     ThetaAPI_Post_takePicture(void);
int     ThetaAPI_Post_commnads_status(void);


#endif	//THETA_CONTROL_H
