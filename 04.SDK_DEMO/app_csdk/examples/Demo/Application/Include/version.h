#ifndef __SVN_VERSION_TMPL
#define __SVN_VERSION_TMPL


#define SVN_VER       "R19792"


#define SVN_MODIFIED  ""
#define SVN_DATE      "2026/10/16 13:29:28"
#define SVN_RANGE     "19792"
#define SVN_MIXED     "Not mixed"


#if defined(SKD_PROD)
#define INFO_DEV_SUB_VER1     "97"
#else
#define INFO_DEV_SUB_VER1     "00"
#endif

#if defined(HW_D70R) 
#define INFO_DEV_SUB_VER2	"03" 
#elif defined(HW_A30R) 
#define INFO_DEV_SUB_VER2	"02" 
#elif defined(HW_A75R) 
#define INFO_DEV_SUB_VER2	"04" 
#endif

// APP version (SVN version)
#define INFO_DEV_SVER_MAIN          "V1.xx.xx"
#undef  INFO_DEV_SVER_MAIN
#define INFO_DEV_SVER_MAIN          "V1."INFO_DEV_SUB_VER1"."INFO_DEV_SUB_VER2

#define INFO_DEV_SVER               INFO_DEV_SVER_MAIN"."SVN_VER

// Software version number
#define APP_VERSION                 INFO_DEV_SVER

#if 0
#warning Source is modified!
#endif

#if 0
#warning Source is mixed!
#endif


#endif

