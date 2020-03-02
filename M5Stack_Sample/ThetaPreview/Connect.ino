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


//-------------------------------------------
// Wi-Fi Connect functions
//-------------------------------------------
int ConnectTHETA(void)
{
  int iRet = WIFI_CONNECT_THETA;
  int iButtonState = 0;
  int iButtonCnt = 0;
  int iBeginRetryCnt = 0;
  int iConnectRet=0;

  Serial.println("");
  Serial.println("WiFi disconnected");

  Serial.print("Connecting to ");
  Serial.println(ssid);

  iButtonCnt=0;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //Disp_ConnectTheta();
    Serial.print(".");
    iButtonState = M5.BtnB.read();
    if ( iButtonState == HIGH ) {
      iButtonCnt++;
      if ( iButtonCnt >= 5 ) {
        iRet=WIFI_SCAN_THETA;
        WiFi.disconnect(true);
        delay(10);
        break;
      }
    } else {
      iButtonCnt=0;
    }
    
    iBeginRetryCnt++;
    if ( iBeginRetryCnt >= 10 ) {
      iBeginRetryCnt=0;
      WiFi.disconnect(true);
      delay(10);
      WiFi.begin(ssid, password);
    }
    
    delay(500);
    CheckBatteryTimming(CHECK_BAT_CYCLE_CONNECT);
  }

  if ( iRet != WIFI_SCAN_THETA ) {
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); //Own IP address. -> if THETA AP mode = 192.168.1.5

    //debug print 
    WiFi.printDiag(Serial);

    iRet = WIFI_CHECK_PORT;
  }
  
  return iRet;
}

int CheckPort(int iRetryCnt, int iRetryDelayMs)
{
  int iRet = WIFI_CHECK_PORT;
  int iConnectRet;

  Serial.printf("\n");
  for (int iCnt=0; iCnt<iRetryCnt; iCnt++) {
    iConnectRet = client.connect(sHost, iHttpPort);
    if ( iConnectRet != 0 ) {
      Serial.printf("*** connect ret=%d\n",iConnectRet);
      iRet = WIFI_CONNECTED;
      break;
    }
    //Disp_ConnectTheta();
    Serial.printf("wait %d msec\n", iRetryDelayMs);
    delay(iRetryDelayMs);
  }
  Serial.printf("\n");
  
  return iRet;
}

int SearchAndEnterTHETA(void)
{
  int iRet = 0;
  int iThetaCnt=0;
  int aiSsidPosList[20];
  char ssidbuf[256];
  
  Serial.println("");
  Serial.println("Search THETA");
  
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(") ");
      //Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*"); //for ESP8266 
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*"); //for ESP32 
      
      if( WiFi.RSSI(i) >= NEAR_RSSI_THRESHOLD ) {
        WiFi.SSID(i).toCharArray(ssidbuf, sizeof(ssidbuf));
        if ( CheckThetaSsid(ssidbuf) == 1 ) {
          aiSsidPosList[iThetaCnt]=i;
          iThetaCnt++;
        }
      }
      delay(10);
    }
    
    if (iThetaCnt>0) {
      iRet = 1;
      
      //Select Max RSSI THETA
      int iRssiMaxPos=0;
      for (int i = 0; i <iThetaCnt; i++) {
        if( WiFi.RSSI(aiSsidPosList[iRssiMaxPos]) < WiFi.RSSI(aiSsidPosList[i]) ) {
          iRssiMaxPos = i;
        }
      }
      //Enter THETA SSID to EEPROM
      WiFi.SSID(aiSsidPosList[iRssiMaxPos]).toCharArray(ssidbuf, sizeof(ssidbuf));
      SaveThetaSsid(ssidbuf);
      Serial.println("");
      Serial.println("--- Detected TEHTA ---");
      Serial.print("SSID=");
      Serial.print(WiFi.SSID(aiSsidPosList[iRssiMaxPos]));
      Serial.print(", RSSI=");
      Serial.print(WiFi.RSSI(aiSsidPosList[iRssiMaxPos]));
      Serial.println("[db]");
    }
  }
  Serial.println("");
  
  return iRet;
}

int CheckThetaSsid( const char* pcSsid )
{
  int iRet=1;

  String strSsid = String(pcSsid);
  if ( strSsid.length() == THETA_SSID_BYTE_LEN ) {
    if ( ( strSsid.startsWith("THETA") == true ) &&
         ( strSsid.endsWith(".OSC") == true )      ){
      //Serial.print("pass=");
      for (int j=THETA_PASS_OFFSET; j<(THETA_PASS_OFFSET+THETA_PASS_BYTE_LEN); j++) {
        //Serial.print( *( pcSsid + j ) );
        if (  (*( pcSsid + j )<0x30) || (0x39<*( pcSsid + j ))  ) {
          iRet=0;
        }
      }
      Serial.println("");
    } else {
      iRet = 0;
    }
  } else {
    iRet = 0;
  }

  return iRet;  
}

void SaveThetaSsid(char* pcSsid)
{
  for (int iCnt=0; iCnt<THETA_SSID_BYTE_LEN; iCnt++) {
     EEPROM.write( (EEPROM_THETA_SSID_TOP + iCnt), *(pcSsid+iCnt) );
  }
  EEPROM.commit();
  delay(100);
  
  return ;
}

void ReadThetaSsid(char* pcSsid)
{
  for (int iCnt=0; iCnt<THETA_SSID_BYTE_LEN; iCnt++) {
    //Read EEPROM
    *(pcSsid+iCnt) = EEPROM.read(EEPROM_THETA_SSID_TOP + iCnt);
  }
  *(pcSsid+THETA_SSID_BYTE_LEN) = 0x00;

  return ;
}

void SetThetaSsidToPassword(char* pcSsid, char* pcPass)
{
  for (int iCnt=0; iCnt<THETA_PASS_BYTE_LEN; iCnt++) {
    //Read EEPROM
    *(pcPass+iCnt) = *(pcSsid+THETA_PASS_OFFSET+iCnt);
  }
  *(pcPass+THETA_PASS_BYTE_LEN) = 0x00;
  
  return ;
}
