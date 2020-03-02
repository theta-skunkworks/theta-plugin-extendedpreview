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

#ifndef CONNECT_H
#define CONNECT_H


#include <EEPROM.h>
#include <WiFi.h>
#include <HTTPClient.h>

//-------------------------------------------
// Wi-Fi Connect functions
//-------------------------------------------
//--- HTTP  ---
const char sHost[] = "192.168.1.1";   // AP Mode
//const int iHttpPort = 80;
const int iHttpPort = 8888;

//--- EEPROM ---
#define   EEPROM_SIZE           512
#define   EEPROM_THETA_SSID_TOP 0
#define   EEPROM_BOOTMODE_TOP   128

//--- Wi-Fi Connect ---
#define   THETA_SSID_BYTE_LEN   19
#define   THETA_PASS_OFFSET     7
#define   THETA_PASS_BYTE_LEN   8
char ssid[THETA_SSID_BYTE_LEN+1] = "THETAXS00000000.OSC";   //This initial value is not used.
char password[THETA_PASS_BYTE_LEN+1] = "00000000";          //This initial value is not used.

#define   NEAR_RSSI_THRESHOLD   -45   //[db]

#define   WIFI_CONNECT_THETA    0
#define   WIFI_CHECK_PORT       1
#define   WIFI_SCAN_THETA       2
#define   WIFI_CONNECTED        3
int iConnectOrScan = WIFI_CONNECT_THETA;

// Use WiFiClient class to create TCP connections
WiFiClient client;


int     ConnectTHETA(void);
int     CheckPort(int iRetryCnt, int iRetryDelayMs);
int     SearchAndEnterTHETA(void);
int     CheckThetaSsid( const char* pcSsid );
void    SaveThetaSsid(char* pcSsid);
void    ReadThetaSsid(char* pcSsid);
void    SetThetaSsidToPassword(char* pcSsid, char* pcPass);


#endif	//CONNECT_H
