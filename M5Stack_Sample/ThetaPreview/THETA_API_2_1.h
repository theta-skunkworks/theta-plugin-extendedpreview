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

#ifndef THETA_API_2_1_H
#define THETA_API_2_1_H


//--- THETA API URLs ---
const char  sUrlInfo[]        = "/osc/info" ;
const char  sUrlState[]       = "/osc/state" ;
const char  sUrlChkForUp[]    = "/osc/checkForUpdates" ;
const char  sUrlCmdExe[]      = "/osc/commands/execute" ;
const char  sUrlCmdStat[]     = "/osc/commands/status" ;


//--- THETA API param lists ---

#define   LIST_MAXSTRLEN_CAPMODE  7
#define   LIST_NUM_CAPMODE        2
char  sList_CaptureMode[LIST_NUM_CAPMODE][LIST_MAXSTRLEN_CAPMODE] = {
                "image", 
                "_video"
    };

#define   LIST_MAXSTRLEN_EXPPRG   2
#define   LIST_NUM_EXPPRG         4
char  sList_ExpProg[LIST_NUM_EXPPRG][LIST_MAXSTRLEN_EXPPRG] = {
                "2",  // AUTO : Normal program
                "9",  // ISO  : ISO priority
                "4",  // SS   : Shutter priority
                "1"   // MANU : Manual
    };

#define   LIST_MAXSTRLEN_EV       5
#define   LIST_NUM_EV             13
char  sList_Ev[LIST_NUM_EV][LIST_MAXSTRLEN_EV] = {
                "-2.0",
                "-1.7",
                "-1.3",
                "-1.0",
                "-0.7",
                "-0.3",
                "0.0",
                "0.3",
                "0.7",
                "1.0",
                "1.3",
                "1.7",
                "2.0"
    };
char  sList_Ev_Disp[LIST_NUM_EV][LIST_MAXSTRLEN_EV] = {
                "-2.0",
                "-1.7",
                "-1.3",
                "-1.0",
                "-0.7",
                "-0.3",
                " 0.0",
                " 0.3",
                " 0.7",
                " 1.0",
                " 1.3",
                " 1.7",
                " 2.0"
    };

#define   LIST_MAXSTRLEN_ISO      5
#define   LIST_NUM_ISO            18
char  sList_ISO[LIST_NUM_ISO][LIST_MAXSTRLEN_ISO] = {
                "64",
                "80",
                "100",
                "125",
                "160",
                "200",
                "250",
                "320",
                "400",
                "500",
                "640",
                "800",
                "1000",
                "1250",
                "1600",
                "2000",
                "2500",
                "3200",
    };
char  sList_ISO_Disp[LIST_NUM_ISO][LIST_MAXSTRLEN_ISO] = {
                " 64 ",
                " 80 ",
                " 100",
                " 125",
                " 160",
                " 200",
                " 250",
                " 320",
                " 400",
                " 500",
                " 640",
                " 800",
                "1000",
                "1250",
                "1600",
                "2000",
                "2500",
                "3200",
    };

#define   LIST_MAXSTRLEN_SS       11
#define   LIST_NUM_SS_SSMODE      30
#define   LIST_NUM_SS             61
char  sList_SS[LIST_NUM_SS][LIST_MAXSTRLEN_SS] = {
                "0.00004","0.00005","0.0000625",
                "0.00008","0.0001","0.000125",
                "0.00015625", "0.0002", "0.00025",
                "0.0003125", "0.0004", "0.0005",
                "0.000625", "0.0008", "0.001",
                "0.00125", "0.0015625", "0.002",
                "0.0025", "0.003125", "0.004",
                "0.005", "0.00625", "0.008",
                "0.01", "0.0125", "0.01666666",
                "0.02", "0.025", "0.03333333",
                "0.04", "0.05", "0.06666666",
                "0.07692307", "0.1", "0.125",
                "0.16666666", "0.2", "0.25",
                "0.33333333", "0.4", "0.5",
                "0.625", "0.76923076", "1",
                "1.3", "1.6", "2",
                "2.5", "3.2", "4", 
                "5", "6", "8", 
                "10", "13", "15", 
                "20","25", "30", 
                "60"
    };
char  sList_SS_Disp[LIST_NUM_SS][LIST_MAXSTRLEN_SS] = {
                "1/25000", "1/20000", "1/16000",
                "1/12500", "1/10000", " 1/8000",
                " 1/6400", " 1/5000", " 1/4000",
                " 1/3200", " 1/2500", " 1/2000",
                " 1/1600", " 1/1250", " 1/1000",
                "  1/800", "  1/640", "  1/500",
                "  1/400", "  1/320", "  1/250",
                "  1/200", "  1/160", "  1/125",
                "  1/100", "   1/80", "   1/60",
                "   1/50", "   1/40", "   1/30",
                "   1/25", "   1/20", "   1/15",
                "   1/13", "   1/10", "   1/8 ",
                "   1/6 ", "   1/5 ", "   1/4 ",
                "   1/3 ", "  1/2.5", "   1/2 ",
                "  1/1.6", "  1/1.3", "     1\"",
                "   1.3\"","   1.6\"","     2\"",
                "   2.5\"","   3.2\"","     4\"",
                "     5\"","     6\"","     8\"",
                "    10\"","    13\"","    15\"",
                "    20\"","    25\"","    30\"",
                "    60\""
    };

#define   LIST_MAXSTRLEN_WB       22
#define   LIST_NUM_WB             12
char  sList_WB[LIST_NUM_WB][LIST_MAXSTRLEN_WB] = {
                "auto",
                "daylight",
                "shade",
                "cloudy-daylight",
                "incandescent",
                "_warmWhiteFluorescent",
                "_dayLightFluorescent",
                "_dayWhiteFluorescent",
                "fluorescent", 
                "_bulbFluorescent",
                "_underwater",
                "_colorTemperature"
    };
char  sList_WB_Disp[LIST_NUM_WB][5] = {
                "auto",
                "dayl",
                "shad",
                "clou",
                "lb 1",
                "lb 2",
                "fl D",
                "fl N",
                "fl W",
                "fl L",
                "u-wa",
                " CT ",
    };

#define WB_COLORTEMP      (LIST_NUM_WB-1)
#define WB_COLORTEMP_MIN  2500
#define WB_COLORTEMP_MAX  10000
#define WB_COLORTEMP_STEP 100



#define   LIST_MAXSTRLEN_OPT      16
#define   LIST_NUM_OPT            4
char  sList_Opt[LIST_NUM_OPT][LIST_MAXSTRLEN_OPT] = {
                "off",
                "DR Comp", 
                "Noise Reduction",
                "hdr"
    };
char  sList_Opt_Disp[LIST_NUM_OPT][4] = {
                "OFF",
                " DR", 
                " NR",
                "HDR"
    };



#endif	//THETA_API_2_1_H
