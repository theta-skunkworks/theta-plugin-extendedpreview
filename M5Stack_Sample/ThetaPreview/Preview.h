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

#ifndef PREVIEW_H
#define PREVIEW_H


//-------------------------------------------
// Live-Preview functions
//-------------------------------------------
const char  sUrlExtCmdExe[]      = "/preview/commands/execute" ;

#define   LV_QUALITY_LOW    50  //  8KB -  9KB : 9.5 fps
#define   LV_QUALITY_MID    80  // 13KB - 14KB : 8   fps
#define   LV_QUALITY_HIGH   95  // 25KB - 26KB : 6.5 fps
//#define   LV_QUALITY_HIGH   100 // 45KB - 46KB : Buffer OVF 
int iPreviewQuality = LV_QUALITY_LOW;

//#define   LV_BSIZE_MAX   30720   // 30KB = 30*1024
#define   LV_BSIZE_MAX   35840   // 35KB = 35*1024

struct T_LvFrame {
  int           iSize;
  unsigned char ucaBuff[LV_BSIZE_MAX];
} ;
#define   LV_FRAME_BUF_NUM   2
struct T_LV_BUFF {
  int       iWP;
  int       iRP;
  T_LvFrame staLvFrame[LV_FRAME_BUF_NUM];
}  ;

T_LV_BUFF *pstLvBuff=0;


int     updateLivePreview(void);
int     DisplayLivePreview(void);

int     ExtAPI_Post_startPreview(void);
int     ExtAPI_Post_stopPreview(void);
int     ExtAPI_Post_getPreview(int previewQuality);


#endif	//PREVIEW_H
