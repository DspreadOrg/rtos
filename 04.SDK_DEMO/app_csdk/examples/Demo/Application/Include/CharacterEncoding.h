/**********************************************************************************************************
* Copyright(C)2011, 
* File Name     : Encoding.h
*  Version      :
*  Author       :
* Description   : Encoding Definition
**********************************************************************************************************/

#ifndef __CHARACTER_ENCODING_H__
#define __CHARACTER_ENCODING_H__


//---------------------------------------------------------------------------
// Definition
//---------------------------------------------------------------------------
typedef enum
{
    IBM037          = 37,       // IBM EBCDIC (US Canada)
    IBM437	        = 437,      // OEM USA
    IBM500          = 500,      // IBM EBCDIC (International)
    ASMO_708        = 708,      // Arabic characters (ASMO 708)
    DOS_720         = 720,      // Arabic characters (DOS)
    ibm737          = 737,      // Greek characters (DOS)
    ibm775          = 775,      // Baltic Characters (DOS)
    ibm850          = 850,      // Western European Characters (DOS)
    ibm852          = 852,      // Central European Characters (DOS)
    IBM855          = 855,      // OEM Cyrillic
    ibm857          = 857,      // Turkey character (DOS)
    IBM00858        = 858,      // OEM Multilingual Latin I
    IBM860          = 860,      // Portuguese (DOS)
    ibm861          = 861,      // Icelandic (DOS)
    DOS_862         = 862,      // Hebrew characters (DOS)
    IBM863          = 863,      // Canadian French (DOS)
    IBM864          = 864,      // Arabic characters (864)
    IBM865          = 865,      // Scandinavian characters (DOS)
    cp866           = 866,      // Cyrillic characters (DOS)
    ibm869          = 869,      // Modern Greek Characters (DOS)
    IBM870          = 870,      // IBM EBCDIC (Multilingual Latin 2)
    windows_874     = 874,      // Thai (Windows)
    cp875           = 875,      // IBM EBCDIC (Modern Greek)
    shift_jis       = 932,      // Japanese (Shift JIS)
    gb2312          = 936,      // Simplified Chinese (GB2312)
    ks_c_5601_1987  = 949,      // Korean
    big5            = 950,      // Traditional Chinese (Big5)
    IBM1026         = 1026,     // IBM EBCDIC (Turkey Latin 5)
    IBM01047        = 1047,     // IBM Latin 1
    IBM01140        = 1140,     // IBM EBCDIC (US - Canada - Europe)
    IBM01141        = 1141,     // IBM EBCDIC (Germany - Europe)
    IBM01142        = 1142,     // IBM EBCDIC (Denmark Norway Europe)
    IBM01143        = 1143,     // IBM EBCDIC (Finland Sweden Europe)
    IBM01144        = 1144,     // IBM EBCDIC (Italy Europe)
    IBM01145        = 1145,     // IBM EBCDIC (Spain Europe)
    IBM01146        = 1146,     // IBM EBCDIC (UK Europe)
    IBM01147        = 1147,     // IBM EBCDIC (France Europe)
    IBM01148        = 1148,     // IBM EBCDIC (International Europe)
    IBM01149        = 1149,     // IBM EBCDIC (Icelandic European)
    utf_16          = 1200,     // Unicode
    UnicodeFFFE     = 1201,     // Unicode (Big-Endian)
    windows_1250    = 1250,     // Central European characters (Windows)
    windows_1251    = 1251,     // Cyrillic characters (Windows)
    Windows_1252    = 1252,     // Western European characters (Windows)
    windows_1253    = 1253,     // Greek characters (Windows)
    windows_1254    = 1254,     // Turkey character (Windows)
    windows_1255    = 1255,     // Hebrew characters (Windows)
    windows_1256    = 1256,     // Arabic characters (Windows)
    windows_1257    = 1257,     // Baltic characters (Windows)
    windows_1258    = 1258,     // Vietnamese characters (Windows)
    Johab           = 1361,     // Korean (Johab)
    macintosh       = 10000,    // Western European characters (Mac)
    x_mac_japanese  = 10001,    // Japanese (Mac)
    x_mac_chinesetrad= 10002,   // Traditional Chinese (Mac)
    x_mac_korean    = 10003,    // Korean (Mac)
    x_mac_arabic    = 10004,    // Arabic characters (Mac)
    x_mac_hebrew    = 10005,    // Hebrew characters (Mac)
    x_mac_greek     = 10006,    // Greek characters (Mac)
    x_mac_cyrillic  = 10007,    // Cyrillic characters (Mac)
    x_mac_chinesesimp = 10008,  // Simplified Chinese (Mac)
    x_mac_romanian  = 10010,    // Romanian (Mac)
    x_mac_ukrainian = 10017,    // Ukrainian (Mac)
    x_mac_thai      = 10021,    // Thai (Mac)
    x_mac_ce        = 10029,    // Central European characters (Mac)
    x_mac_icelandic = 10079,    // Icelandic (Mac)
    x_mac_turkish   = 10081,    // Turkey characters (Mac)
    x_mac_croatian  = 10082,    // Croatian (Mac)
    x_Chinese_CNS   = 20000,    // Traditional Chinese (CNS)
    x_cp20001       = 20001,    // TCA Taiwan
    x_Chinese_Eten  = 20002,    // Traditional Chinese (Eten)
    x_cp20003       = 20003,    // IBM5550 Taiwan
    x_cp20004       = 20004,    // TeleText Taiwan
    x_cp20005       = 20005,    // Wang Taiwan
    x_IA5           = 20105,    // Western European characters (IA5)
    x_IA5_German    = 20106,    // German (IA5)
    x_IA5_Swedish   = 20107,    // Swedish (IA5)
    x_IA5_Norwegian = 20108,    // Norwegian (IA5)
    us_ascii        = 20127,    // US-ASCII
    x_cp20261       = 20261,    // T.61
    x_cp20269       = 20269,    // ISO-6937
    IBM273          = 20273,    // IBM EBCDIC (Germany)
    IBM277          = 20277,    // IBM EBCDIC (Denmark Norway)
    IBM278          = 20278,    // IBM EBCDIC (Finland Sweden)
    IBM280          = 20280,    // IBM EBCDIC (Italy)
    IBM284          = 20284,    // IBM EBCDIC (Spain)
    IBM285          = 20285,    // IBM EBCDIC (UK)
    IBM290          = 20290,    // IBM EBCDIC (Japanese Katakana)
    IBM297          = 20297,    // IBM EBCDIC (France)
    IBM420          = 20420,    // IBM EBCDIC (Arabic)
    IBM423          = 20423,    // IBM EBCDIC (Greek)
    IBM424          = 20424,    // IBM EBCDIC (Hebrew)
    x_EBCDIC_KoreanExtended = 20833,    // IBM EBCDIC (Korean extension)
    IBM_Thai        = 20838,    // IBM EBCDIC (Thai)
    koi8_r          = 20866,    // Cyrillic characters (KOI8-R)
    IBM871          = 20871,    // IBM EBCDIC (Icelandic)
    IBM880          = 20880,    // IBM EBCDIC (Cyrillic Russian)
    IBM905          = 20905,    // IBM EBCDIC (Turkish)
    IBM00924        = 20924,    // IBM Latin 1
    EUC_JP          = 20932,    // Japanese (JIS 0208-1990 and 0212-1990)
    x_cp20936       = 20936,    // Simplified Chinese (GB2312-80)
    x_cp20949       = 20949,    // Korean Wansung
    cp1025          = 21025,    // IBM EBCDIC (Cyrillic Serbian Bulgarian)
    koi8_u          = 21866,    // Cyrillic characters (KOI8-U)
    iso_8859_1      = 28591,    // Western European characters (ISO)
    iso_8859_2      = 28592,    // Central European Characters (ISO)
    iso_8859_3      = 28593,    // Latin 3 (ISO)
    iso_8859_4      = 28594,    // Baltic characters (ISO)
    iso_8859_5      = 28595,    // Cyrillic characters (ISO)
    iso_8859_6      = 28596,    // Arabic characters (ISO)
    iso_8859_7      = 28597,    // Greek characters (ISO)
    iso_8859_8      = 28598,    // Hebrew characters (ISO Visual)
    iso_8859_9      = 28599,    // Turkey character (ISO)
    iso_8859_13     = 28603,    // Estonian (ISO)
    iso_8859_15     = 28605,    // Latin 9 (ISO)
    x_Europa        = 29001,    // Europa
    iso_8859_8_i    = 38598,    // Hebrew characters (ISO Logical)
    iso_2022_jp     = 50220,    // Japanese (JIS)
    csISO2022JP     = 50221,    // Japanese (JIS allows 1-byte kana)
    iso_2022_jp_2   = 50222,    // Japanese (JIS allows 1-byte kana - SO/SI)
    iso_2022_kr     = 50225,    // Korean (ISO)
    x_cp50227       = 50227,    // Simplified Chinese (ISO-2022)
    euc_jp          = 51932,    // Japanese (EUC)
    EUC_CN          = 51936,    // Simplified Chinese (EUC)
    euc_kr          = 51949,    // Korean (EUC)
    hz_gb_2312      = 52936,    // Simplified Chinese (HZ)
    GB18030         = 54936,    // Simplified Chinese (GB18030)
    x_iscii_de      = 57002,    // ISCII Sanskrit
    x_iscii_be      = 57003,    // ISCII Bengali
    x_iscii_ta      = 57004,    // ISCII Tamil
    x_iscii_te      = 57005,    // ISCII Telugu
    x_iscii_as      = 57006,    // ISCII Assamese
    x_iscii_or      = 57007,    // ISCII Oriya
    x_iscii_ka      = 57008,    // ISCII Kannada
    x_iscii_ma      = 57009,    // ISCII Malayalam
    x_iscii_gu      = 57010,    // ISCII Gujarati
    x_iscii_pa      = 57011,    // ISCII Punjabi
    utf_7           = 65000,    // Unicode (UTF-7)
    utf_8           = 65001,    // Unicode (UTF-8)
    utf_32          = 65005,    // Unicode (UTF-32)
    utf_32BE        = 65006,    //
}E_CHARACTER_ENCODING_COLLECTION;
#endif /*__CHARACTER_ENCODING_H__*/

