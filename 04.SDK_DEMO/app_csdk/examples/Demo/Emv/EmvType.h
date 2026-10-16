/**********************************************************************************************************
 * Copyright(C)2018, 
 * File Name     : EmvType.h
 * Version       : v1.0
 * Author        :
 * Description   : EMV kernel type definition
 **********************************************************************************************************/

#ifndef _EMV_TYPE_H_
#define _EMV_TYPE_H_

#ifndef NULL
#define NULL      (0)
#endif

#ifndef TRUE
#define TRUE      (1)
#endif

#ifndef FALSE
#define FALSE     (0)
#endif

#ifndef bool
//#define BOOL   unsigned char
typedef unsigned char bool;
#endif

#ifndef BOOL
//#define bool   BOOL
typedef unsigned char BOOL;
#endif

#ifndef uint8
#define uint8   unsigned char
#endif

#ifndef uint16
#define uint16  unsigned short
#endif

#ifndef uint32
#define uint32  unsigned int
#endif

#ifndef int8
#define int8   signed char
#endif

#ifndef int16
#define int16  short
#endif

#ifndef int32
#define int32  int
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef HIBYTE
#define HIBYTE(X) ((unsigned char)((X) >> 8))
#endif
#ifndef LOBYTE
#define LOBYTE(X) ((unsigned char)((X)&0xFF))
#endif
#define HIWORD(l) ((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))
#define LOWORD(l) ((unsigned short)(l))

#ifndef MAKEWORD
#define MAKEWORD(HiByte, LoByte) ((((unsigned short)(HiByte) << 8) & 0xFF00) | ((unsigned short)(LoByte)&0x00FF))
#endif

#ifndef WORD2BUF
#define WORD2BUF(l, buf)                \
    {                                   \
        *(buf) = (unsigned char)((l) >> 8) & 0xFF; \
        *((buf) + 1) = (unsigned char)(l)&0xFF;    \
    }
#endif


#define TAG_NULL            0xFFFFFFFF
#define LENGTH_NULL         0xFFFFFFFF


// Tags in CDOL
//8A 02     Authorization Response Code
#define TAG_008A            0x008A
//95 05     Terminal verification result TVR
#define TAG_0095            0x0095
//9F 02 06  Credit limit
#define TAG_9F02            0x9F02
//9F 03 06  Other amounts
#define TAG_9F03            0x9F03
//9F 1A 02  terminal country code
#define TAG_9F1A            0x9F1A
//5F 2A 02  transaction Currency Code
#define TAG_5F2A            0x5F2A
//9A 03     transaction Date
#define TAG_009A            0x009A
//9C 01     transaction type
#define TAG_009C            0x009C
//9F 37 04  Tags for unpredictable data at the terminal
#define TAG_9F37            0x9F37
//9F 21 03  transaction time
#define TAG_9F21            0x9F21
//9F 4E 14  MERCHANT NAME
#define TAG_9F4E            0x9F4E


// Tags in CDOL
//generate ac Return label
// Issuer bank Application Data
#define TAG_9F10            0x9F10
//AC Application Cryptogram 8BYTE
#define TAG_9F26            0x9F26
// CID cryptogram information data 1BYTE
#define TAG_9F27            0x9F27
// ATC Application transaction records 2BYTE
#define TAG_9F36            0x9F36
// Signed Dynamic Application Data
#define TAG_9F4B            0x9F4B
// Application version number
#define TAG_9F09            0x9F09
// Terminal Type
#define TAG_9F35            0x9F35
// Terminal limit
#define TAG_9F1B            0x9F1B
// Additional performance terminal
#define TAG_9F40            0x9F40
// terminal capability
#define TAG_9F33            0x9F33
// TSI
#define TAG_009B            0x009B


// PDOL Value
#define TAG_0083            0x0083
// Industry identification code
#define TAG_0042            0x0042
// ADF name
#define TAG_004F            0x004F
// Application label
#define TAG_0050            0x0050
// Equivalent data of track 2
#define TAG_0057            0x0057
// Application primary Account (PAN)
#define TAG_005A            0x005A
// Identifier
#define TAG_0061            0x0061
// FCI template
#define TAG_006F            0x006F
// lable
#define TAG_0070            0x0070
// Customized by the issuing bank
#define TAG_0073            0x0073
// DF name
#define TAG_0084            0x0084
// Application priority indicator
#define TAG_0087            0x0087
// Basic directory files SFI
#define TAG_0088            0x0088
// CDOL1
#define TAG_008C            0x008C
// CDOL2
#define TAG_008D            0x008D
// Cardholder Verification Method (CVM) List
#define TAG_008E            0x008E
// CA Public Key Index (PKI)
#define TAG_008F            0x008F
// NCA issuing bank public key certificate NI
#define TAG_0090            0x0090
// Remainder of issuer's public key
#define TAG_0092            0x0092
// application for Static signature data NI
#define TAG_0093            0x0093
// DDF name
#define TAG_009D            0x009D
// FCI Data Specific Template
#define TAG_00A5            0x00A5
// Cardholder Name
#define TAG_5F20            0x5F20
// Application expiration date YYMMDD
#define TAG_5F24            0x5F24
// Application effective date
#define TAG_5F25            0x5F25
// Country code of issuing bank
#define TAG_5F28            0x5F28
//Language selection
#define TAG_5F2D            0x5F2D
//Service code
#define TAG_5F30            0x5F30
// Application primary account serial number
#define TAG_5F34            0x5F34
// Issuing Bank URL
#define TAG_5F50            0x5F50
// International Bank Account Number
#define TAG_5F53            0x5F53
// Bank identifier code
#define TAG_5F54            0x5F54
// Country code of issuing bank
#define TAG_5F55            0x5F55
// Country code of issuing bank
#define TAG_5F56            0x5F56
// Terminal application identifier
#define TAG_9F06            0x9F06
// Application Usage Control
#define TAG_9F07            0x9F07
// Application version number
#define TAG_9F08            0x9F08
// ISSUER ACT CODE (IAC) - Default
#define TAG_9F0D            0x9F0D
// Issuing Bank Action Code (IAC) - Reject
#define TAG_9F0E            0x9F0E
// Issuing Bank Action Code (IAC) - Online
#define TAG_9F0F            0x9F0F
// Index of issuing bank codes
#define TAG_9F11            0x9F11
// Application Priority Name
#define TAG_9F12            0x9F12
// Continuous offline transaction lower limit
#define TAG_9F14            0x9F14
// Magnetic Stripe 1 Custom Data
#define TAG_9F1F            0x9F1F
// Continuous offline transaction limit
#define TAG_9F23            0x9F23
// Issuer's public key exponent
#define TAG_9F32            0x9F32
// PDOL
#define TAG_9F38            0x9F38
// Application currency code                     & APPLICATION CURRENCY CODE (MASTER)
#define TAG_9F42            0x9F42
// Data of Static Applications Signed (SAD) NI
#define TAG_9F46            0X9F46
// IC Card Public Key exponent
#define TAG_9F47            0x9F47
// DDOL
#define TAG_9F49            0x9F49
// Record entry
#define TAG_9F4D            0x9F4D
// Electronic cash terminal indicator
#define TAG_9F7A            0x9F7A
// Customized by the issuing bank
#define TAG_BF0C            0xBF0C
// Cardholder ID number       -Master CVC3(Track2)
#define TAG_9F61            0x9F61
// Cardholder ID type     -Master PCVC3(Track1)
#define TAG_9F62            0x9F62
// EMV Application Reference Currency n3
#define TAG_9F3B            0x9F3B
// EMV Application Reference Currency exponent n1
#define TAG_9F43            0x9F43

// Terminal transaction attributes   -Master PUNATC(Track2)
#define TAG_9F66            0x9F66
// lable
#define TAG_0080            0x0080
// lable
#define TAG_0077            0x0077
// AFL
#define TAG_0094            0x0094
// AIP
#define TAG_0082            0x0082
// Available offline consumption amount-Master Application Capabilities Information
#define TAG_9F5D            0x9F5D
// Card transaction attributes
#define TAG_9F6C            0x9F6C
// EC Issuer Authorization Code
#define TAG_9F74            0x9F74
// List of static data authentication labels
#define TAG_9F4A            0x9F4A
// IC card public key remainder
#define TAG_9F48            0x9F48
// Continuous offline transaction limit
#define TAG_9F59            0x9F59
// Product Card Identification Information - Master PUNATC (Track1)
#define TAG_9F63            0x9F63
//Card authentication related data indicates FDDA 00/01   //In the master, it refers to UDOL
#define TAG_9F69            0x9F69
//Second currency code
#define TAG_9F76            0x9F76
// Unprotected Data Envelope 3
#define TAG_9F77            0x9F77
// Unprotected Data Envelope 4
#define TAG_9F78            0x9F78
// PIN attempt counter
#define TAG_9F17            0x9F17
// Continuous offline transaction lower limit
#define TAG_9F58            0x9F58
// Transaction log format
#define TAG_9F4F            0x9F4F
// Last Online ATC Register
#define TAG_9F13            0x9F13
// Issuing bank URL2
#define TAG_9F5A            0x9F5A
// Country code of issuing bank
#define TAG_9F57            0x9F57
// Issuing bank authentication indicator bit
#define TAG_9F56            0x9F56
// IC card dynamic number
#define TAG_9F4C            0x9F4C
// data authentication code  Part of 93
#define TAG_9F45            0x9F45
// Maximum cumulative offline transaction amount
#define TAG_9F5C            0x9F5C
// Currency conversion factor
#define TAG_9F73            0x9F73
// Transaction sequence counter n 4-8, 4 bytes, 1 increment per transaction, with terminal maintenance
#define TAG_9F41            0x9F41
// Transaction reference currency index
#define TAG_9F3D            0x9F3D
// Transaction Reference Currency Code
#define TAG_9F3C            0x9F3C
// Terminal identification
#define TAG_9F1C            0x9F1C
// Transaction currency index
#define TAG_5F36            0x5F36
// Terminal sales point input mode
#define TAG_9F39            0x9F39
// Merchant identification ans15
#define TAG_9F16            0x9F16
// Merchant classification code n4
#define TAG_9F15            0x9F15
// Interface Device (IFD)
#define TAG_9F1E            0x9F1E
// Certification Center Public Key Index
#define TAG_9F22            0x9F22
// Certification data from issuing bank
#define TAG_0091            0x0091
// Issuing Bank Script Command
#define TAG_0086            0x0086
// TDOL
#define TAG_0097            0x0097
// Transaction amount HEX format 4 bytes
#define TAG_0081            0x0081
// Transaction certificate
#define TAG_0098            0x0098
// Online PIN
#define TAG_0099            0x0099
// Application Currency Code   &   SUPERT RECOVER AC DATA (MASTAR)
#define TAG_9F51            0x9F51
// default behavior of the application
#define TAG_9F52            0x9F52
// Customized data for applications 1-32
#define TAG_9F05            0x9F05
// Application currency exponent   &    Application Currency Exponent   (MASTER)
#define TAG_9F44            0x9F44
// Cardholder Name Extension
#define TAG_9F0B            0x9F0B
// Continuous Offline Transaction Restrictions (International-Currency)
#define TAG_9F53            0x9F53
// Accumulated offline transaction amount limit
#define TAG_9F54            0x9F54
// Continuous offline transaction limit (International-country)
#define TAG_9F72            0x9F72
// Accumulated offline transaction amount limit (dual currency)
#define TAG_9F75            0x9F75
// Acquiring bank identifier n6-11
#define TAG_9F01            0x9F01
// Other amount HEX, 4 bytes
#define TAG_9F04            0x9F04
// Reference currency amount HEX, 4 bytes
#define TAG_9F3A            0x9F3A
// Cardholder Verification Method (CVM) Result B3
#define TAG_9F34            0x9F34
// Issuing Bank Country Code   &   Account Type (MASTER)
#define TAG_5F57            0x5F57
// Issuing Bank URL   &   OFF LINE BALANCE (MASTER)
#define TAG_9F50            0x9F50
#define TAG_9F18            0x9F18
// MASTER refers to the proprietary data in magnetic stripe 2
#define TAG_9F20            0x9F20
// Trading currency index
#define TAG_9F1D            0x9F1D
// support china national security algorithm or not
#define TAG_DF69            0xDF69
// Electronic cash balance
#define TAG_9F79            0x9F79
// Electronic cash reset threshold
#define TAG_9F6D            0x9F6D
// DS Slot Management Control
#define TAG_9F6F            0x9F6F
// Protected Data Envelope 1  (AXP: Card Interface and Payment Capabilities)
#define TAG_9F70            0x9F70
// Protected Data Envelope 2  (AXP: Mobile CVM Results)
#define TAG_9F71            0x9F71
// Issuing Bank Script 71
#define TAG_0071            0x0071
// Issuing bank script 72
#define TAG_0072            0x0072
// Authorization code
#define TAG_0089            0x0089
// DS Summary 1
#define TAG_9F7D            0x9F7D
// DS Unpredictable Number
#define TAG_9F7F            0x9F7F
//Second application currency code
#define TAG_DF71            0xDF71
//Second electronic cash balance
#define TAG_DF79            0xDF79

// PURE increase
// The value of AID supported by the terminal
#define TAG_DF7F            0xDF7F
//Terminal Execution Options
#define TAG_FF23            0xFF23
// Current process indicator, 0 represents NEW, 1 represents Online Response
#define TAG_FF24            0xFF24
// AID contactless preprocessing indicators
#define TAG_FF25            0xFF25
// contactless application capability
#define TAG_FF26            0xFF26
// The length of TAG in GDDOL Resulting Buffer
#define TAG_FF27            0xFF27
// Autorun
#define TAG_FF28            0xFF28
// GDDOL Resulting Buffer
#define TAG_9F71            0x9F71
// Memory Slot Update Template
#define TAG_BF70            0xBF70
// Memory Slot Read Template
#define TAG_BF71            0xBF71
// TTPI
#define TAG_00C7            0x00C7
// CCID
#define TAG_00C5            0x00C5
//Application currency code
#define TAG_00CD            0x00CD
// MTOL
#define TAG_DF8400          0xDF8400

// DISCVER increase
// ADF extension name
#define TAG_9F29            0x9F29
// Offline Allow Zero Amount Flag
#define TAG_FF62            0xFF62
// Allow zero amount flag
#define TAG_FF63            0xFF63
// Data Storage
#define TAG_FF64            0xFF64
// Extended Logging
#define TAG_FF65            0xFF65
// Tearing Recovery
#define TAG_FF66            0xFF66
// Save Script Processing SW
#define TAG_FF67            0xFF67
// Save GPO response data
#define TAG_FF68            0xFF68
// Request the length of random number
#define TAG_FF69            0xFF69
// SEL AID
#define TAG_FF70            0xFF70
// Data Storage Update Template
#define TAG_BF10            0xBF10
// Write Data Storage Template
#define TAG_BF11            0xBF11
#define TAG_DF3A            0xDF3A
#define TAG_DF3B            0xDF3B
// Extended log
#define TAG_DF3C            0xDF3C
// Data Storage Directory
#define TAG_DF3D            0xDF3D
#define TAG_DF3E            0xDF3E
// Data Store
#define TAG_DF3F            0xDF3F
// Offline balance
#define TAG_00D1            0x00D1

// JCB increase
#define TAG_9F25            0x9F25  // Last 4 digits of PAN
#define TAG_9F19            0x9F19  // Token Requestor ID
#define TAG_DF39            0xDF39  // Transaction Mode: 00 UNKNOWN,01 MAG_STRIPE,02 EMV,03 TRANS_MODE_LEGACY
#define TAG_DF40            0xDF40  // USER DEFINE TAG : Implementation Options               1B
#define TAG_DF41            0xDF41  // USER DEFINE TAG : Combination Options                  2B
#define TAG_DF42            0xDF42  // USER DEFINE TAG : Online Transaction Context           VAR
#define TAG_DF43            0xDF43  // USER DEFINE TAG : 'Recovering from Torn EMV Transaction' Flag   1B
#define TAG_DF44            0xDF44  // USER DEFINE TAG : Recovery Context
#define TAG_DF45            0xDF45  // USER DEFINE TAG : Terminal Interchange Profile (static )   3B
#define TAG_DF46            0xDF46  // USER DEFINE TAG : Torn CDA Hash Data Buffer            VAR UP TO 507
#define TAG_DF47            0xDF47  // USER DEFINE TAG : Torn Track 2 Data                    VAR UP TO 19
#define TAG_DF48            0xDF48  // USER DEFINE TAG : Removal Timeout                      4n
#define TAG_DF49            0xDF49  // USER DEFINE TAG : transaction type
#define TAG_FF71            0xFF71  // USER DEFINE TAG : 71 TAG Summary
#define TAG_FF72            0xFF72  // USER DEFINE TAG : 72 TAG Summary

// Custom TAG
// Default TDOL
#define TAG_FF01            0xFF01
// Issuing bank script
#define TAG_FF02            0xFF02
// SDA related static application data
#define TAG_FF03            0xFF03
// Terminal status check support bit (QPBOC)
#define TAG_FF04            0xFF04
// Security Control Information (8583 F53)
#define TAG_FF05            0xFF05
// PIN contains identifier (8583 F22 3rd digit)
#define TAG_FF06            0xFF06
// Flashcard identifier N1 0: Not flashed 1: Flashcard+voucher number
#define TAG_FF07            0xFF07
// Obtain the read record instruction for TAG:005A
#define TAG_FF08            0xFF08
// Get the read record instruction for TAG:0057
#define TAG_FF09            0xFF09
// ARQC, 9F26 saves TC during printing, FF0A saves ARQC
#define TAG_FF0A            0xFF0A
// Authorization amount of 0 check
#define TAG_FF0B            0xFF0B
//#define TAG_FF0C            0xFF0C  // Not Used
// Current read record instruction, 5B fixed length
#define TAG_FF0D            0xFF0D
// Warning status word processing identifier  0 or does not exist, processing warning status word; 1: not processing warning status word
#define TAG_FF0E            0xFF0E
// Save time when flashing card (relative to the number of seconds on January 1, 1970, 4 byte int)
#define TAG_FF0F            0xFF0F
// Last read record command during card flashing
#define TAG_FF10            0xFF10
// Is the static authentication data valid during card flashing (are all read data labels 70)
#define TAG_FF11            0xFF11
// Track 3 data
#define TAG_FF12            0xFF12
// CA certificate serial number
#define TAG_FF13            0xFF13
// Switch information
#define TAG_FF14            0xFF14
// Data capture methods
#define TAG_FF15            0xFF15
// TAG to be read before storing GAC
#define TAG_FF16            0xFF16

//  support UnionPay AID priority or not, change it to support any AID priority, see FF61
#define TAG_FF20            0xFF20
// Will only card supported AIDs be added to the candidate list?

#define TAG_FF21            0xFF21
// Does it support null online PIN? 0- null online PIN is considered as entered 1- null online PIN is considered as PIN byPass
#define TAG_FF22            0xFF22
// AXP PROFILE configuration information
#define TAG_FF23            0xFF23

// Candidate list parameter, application priority processing identifier bin 1B, Bit0: China UnionPay application, Bit1: Terminal support
#define TAG_FF30            0xFF30
// Candidate list parameters, terminal AID parameter index corresponding to ADF
#define TAG_FF31            0xFF31
// Default Dynamic Reader Limits American DRL configuration parameter set, with value ranges including DF19, DF20, and DF21
#define TAG_FF32            0xFF32
// Sets of Dynamic Reader Limits, The American DRL configuration parameter set contains a maximum of 16 configurations, with TAGs ranging from 0x01 to 0x10, and each TAG value range containing DF19, DF20, and DF21
#define TAG_FF33            0xFF33

// Issuer's public key module
#define TAG_FF40            0xFF40
// IC card public key module
#define TAG_FF41            0xFF41
// PDOL Package Data
#define TAG_FF43            0xFF43
// CDOL1 Package Data
#define TAG_FF44            0xFF44
// CDOL2 Package Data
#define TAG_FF45            0xFF45
// Offline PIN encryption public key
#define TAG_FF46            0xFF46
// AXP Kernel Pseudo Track Data Pseudo Track 1
#define TAG_FF47            0xFF47
// AXP kernel pseudo two track data Pseudo Track 2
#define TAG_FF48            0xFF48

// Transaction parameters
// Force online
#define TAG_FF50            0xFF50
// Mandatory online PIN, only applicable to EMV standard mode
#define TAG_FF51            0xFF51
// Transaction serial number
#define TAG_FF52            0xFF52
// Additional data
#define TAG_FF53            0xFF53

// configuration parameter
// Cardholder application selection method 1- automatic selection, 0 or non-existent - manual selection
#define TAG_FF60            0xFF60
// Priority AID selection
#define TAG_FF61            0xFF61
// Whether to execute card number confirmation 0- do not execute card number confirmation,
// 1 or do not exist - default to execute card number confirmation
#define TAG_FF62            0xFF62


// UnionPay TAG Definition
// Does it support partial AID matching?
#define TAG_DF01            0xDF01
// CA Public key module
#define TAG_DF02            0xDF02
// CA Public key verification value
#define TAG_DF03            0xDF03
// CA Public key exponent
#define TAG_DF04            0xDF04
// CA Public key validity period
#define TAG_DF05            0xDF05
// CA Public Key HASH Algorithm Identification
#define TAG_DF06            0xDF06
// CA Public key (signature) algorithm identification
#define TAG_DF07            0xDF07
// TAC-default
#define TAG_DF11            0xDF11
// TAC-online
#define TAG_DF12            0xDF12
// TAC-decline
#define TAG_DF13            0xDF13
// Default DDOL
#define TAG_DF14            0xDF14
// Bias random selection threshold
#define TAG_DF15            0xDF15
// Bias random selection of maximum target percentage
#define TAG_DF16            0xDF16
// Randomly select target percentage
#define TAG_DF17            0xDF17
// Terminal online PIN support capability
#define TAG_DF18            0xDF18
// Minimum limit for terminals contactless and offline
#define TAG_DF19            0xDF19
// Terminal contactless transaction limit, Master -Reader Contactless Transaction Limit
#define TAG_DF20            0xDF20
// Terminal contactless CVM Limit
#define TAG_DF21            0xDF21
// Issuing bank script result
#define TAG_DF31            0xDF31
// Terminal SM algorithm support indicator
#define TAG_DF69            0xDF69
// Offline PIN encryption public key certificate
#define TAG_9F2D            0x9F2D
// Offline PIN encryption public key exponent
#define TAG_9F2E            0x9F2E
// Offline PIN encryption public key remainder
#define TAG_9F2F            0x9F2F

// MASTER newly added
// Application selection registration proprietary data
#define TAG_9F0A            0x9F0A
// extend selection
#define TAG_9F29            0x9F29
// Kernel identifier
#define TAG_9F2A            0x9F2A
// Device support indicator
#define TAG_9F7E			0x9F7E

#define TAG_9F5E			0x9F5E	// DS ID
#define TAG_9F5F			0x9F5F	// DS Slot Availability
#define TAG_9F60			0x9F60	//CVC3 (Track1)
#define TAG_9F61			0x9F61	//CVC3 (Track2)
#define TAG_9F64			0x9F64	//NATC(Track1)
#define TAG_9F65			0x9F65  //PCVC3(Track2)
#define TAG_9F67			0x9F67	//NATC(Track2)

#define TAG_9F6A			0x9F6A	// Unpredictable Number (Numeric)
#define TAG_9F6B            0x9F6B  //Track 2 Data

#define TAG_0056			0x0056	//Track 1 Data
#define TAG_DF4B			0xDF4B	//PCII
#define TAG_DF60			0xDF60	// DS Input (Card)
#define TAG_DF61			0xDF61	// DS Digest H
#define TAG_DF62			0xDF62	// DS ODS Info DS ODS Term
#define TAG_DF63			0xDF63	// DS ODS Term


// Master does not define actual meaning, TAG used for testing, unknown Tag
#define TAG_00C7            0x00C7
// Master does not define actual meaning, TAG used for testing
#define TAG_00D7            0x00D7

#define TAG_DF8101          0xDF8101    //DS Summary 2
#define TAG_DF8102          0xDF8102    //DS Summary 3
#define TAG_DF8104          0xDF8104    //Balance Read Before Gen AC
#define TAG_DF8105          0xDF8105    //Balance Read After Gen AC
#define TAG_DF8106          0xDF8106    // Data Needed

#define TAG_DF8108          0xDF8108    // DS AC Type
#define TAG_DF8109          0xDF8109    // DS Input (Term)
#define TAG_DF810A          0xDF810A    // DS ODS Info For Reader
#define TAG_DF810B          0xDF810B    // DS Summary Status
#define TAG_DF810C          0xDF810C    // Kernel ID
#define TAG_DF810D          0xDF810D    // DSVN Term
#define TAG_DF810E          0xDF810E    // Post-Gen AC Put Data Status
#define TAG_DF810F          0xDF810F    // Pre-Gen AC Put Data Status
#define TAG_DF8110          0xDF8110    // Proceed To First Write Flag

#define TAG_DF8112          0xDF8112    // Tags To Read

#define TAG_DF8115          0xDF8115    // Error type indicator
#define TAG_DF8116          0xDF8116    // user interface
#define TAG_DF8117          0xDF8117    //Card Data Input Capability            9F33 BYTE1
#define TAG_DF8118          0xDF8118    //CVM Required                          9F33 BYTE2 BIT4
#define TAG_DF8119          0xDF8119    //CVM NO Required                       9F33 BYTE2 BIT4
#define TAG_DF811A          0xDF811A    //Default UDOL
#define TAG_DF811B          0xDF811B    //Kernel Configuration
#define TAG_DF811C          0xDF811C    // Max Lifetime of Torn Transaction Log Record
// The maximum number of saved flash card transaction records
#define TAG_DF811D          0xDF811D
// Magnetic strip card CVM Required
#define TAG_DF811E          0xDF811E
#define TAG_DF811F          0xDF811F    //Security Capability                   9F33 BYTE3
#define TAG_DF8120          0xDF8120    // Terminal Action Code - Default
#define TAG_DF8121          0xDF8121    // Terminal Action Code - Denial
#define TAG_DF8122          0xDF8122    // Terminal Action Code - Online
// Minimum limit for non connected card readers
#define TAG_DF8123			0xDF8123
// Card reader contactless transaction limit (NO ON-DEVICE CVM)
#define TAG_DF8124			0xDF8124
// Card reader contactless transaction limit (ON-DEVICE CVM)
#define TAG_DF8125			0xDF8125
// Card reader CVM limit
#define TAG_DF8126			0xDF8126
#define TAG_DF8127			0xDF8127	// Time Out Value
#define TAG_DF8128			0xDF8128	// IDS Status
// Result parameter set
#define TAG_DF8129			0xDF8129
#define TAG_DF812A			0xDF812A	// DD Card (Track1)
#define TAG_DF812B			0xDF812B	// DD Card (Track2)
#define TAG_DF812C			0xDF812C	// Magnetic strip card CVM NO Required
#define TAG_DF812D			0xDF812D	//Message Hold Time
#define TAG_DF8130          0xDF8130    //Hold Time Value
#define TAG_DF8131			0xDF8131	//Phone Message Table
#define TAG_DF8132			0xDF8132	// Minimum delay blocking grace period
#define TAG_DF8133			0xDF8133	// Maximum delay blocking grace period
#define TAG_DF8134			0xDF8134	// Terminal expected transmission time for delayed blocking of C-APDU
#define TAG_DF8135			0xDF8135	// Terminal expected transmission time for delayed blocking of R-APDU
#define TAG_DF8136			0xDF8136	// Delay blocking accuracy threshold
#define TAG_DF8137			0xDF8137	// Delay blocking transmission time mismatch threshold
#define TAG_DF8301			0xDF8301	// Terminal delay blocking entropy
#define TAG_DF8302			0xDF8302	// Device delay blocking entropy
#define TAG_DF8303			0xDF8303	// Minimum processing time for delayed blocking of APDU
#define TAG_DF8304			0xDF8304	// Maximum processing time for delayed blocking of APDU
#define TAG_DF8305			0xDF8305	// Expected transmission time of devices used to delay blocking R-APDU
#define TAG_DF8306			0xDF8306	// Delay blocking processing time
#define TAG_DF8307			0xDF8307	//RRP Counter
#define TAG_DF8107          0xDF8107    //CDOL1 Related Data
#define TAG_DF8128          0xDF8128    //IDS Status
#define TAG_DF8111          0xDF8111    //PDOL Related Data, Include TAG 83
#define TAG_DF8113          0xDF8113    //DRDOL Related Data
#define TAG_DF8114          0xDF8114    //Reference Control Parameter

#define TAG_FF8101          0xFF8101    // Torn Record
#define TAG_FF8102          0xFF8102    // Tags To Write Before Gen AC
#define TAG_FF8103          0xFF8103    // Tags To Write After Gen AC
#define TAG_FF8104          0xFF8104    // Data To Send
#define TAG_FF8105          0xFF8105    // Data Record
#define TAG_FF8106          0xFF8106    // Discretionary Data

//Master Custom TAG
#define TAG_FFF001          0xFFF001    // TEI SIGNAL ACK: ResponseCode
#define TAG_FFF002          0xFFF002    // TEI CONFIG SIGNAL: CONF_NAME
#define TAG_FFF003          0xFFF003    // TEI DEK SIGNAL: DEK
#define TAG_FFF004          0xFFF004    // TEI DET SIGNAL: DET
#define TAG_FFF005          0xFFF005    // TEI TEST_INFO SIGNAL: InterfaceVersion
#define TAG_FFF006          0xFFF006    // TEI TEST_INFO SIGNAL: SessionId
#define TAG_FFF007          0xFFF007    // TEI TEST_INFO SIGNAL: TestId
#define TAG_FFF008          0xFFF008    // TEI TEST_INFO SIGNAL: DekDetId
#define TAG_FFF009          0xFFF009    // AC Type
#define TAG_FFF00A          0xFFF00A    // CDA DSV Related Data
#define TAG_FFF00B          0xFFF00B    // Tags of DET Download
#define TAG_FFF00C          0xFFF00C    // Tags To Read Yet
#define TAG_FFF00D          0xFFF00D    // Tags To Write Yet After Gen AC
#define TAG_FFF00E          0xFFF00E    // Tags To Write Yet Before Gen AC
#define TAG_FFF00F          0xFFF00F    // Active AFL
#define TAG_FFF010          0xFFF010    // ODA Status  (MasterCard: Bit7 -CDA), (AXP: Bit7 -CDA, Bit5 -SDA)
#define TAG_FFF011          0xFFF011    // Static Data To Be Authenticated
#define TAG_FFF012          0xFFF012    // Torn Entry  bin 1B
#define TAG_FFF013          0xFFF013    // nUN
#define TAG_FFF014          0xFFF014    // Torn Temp Record
#define TAG_FFF015          0xFFF015    // Final Select AID Response
#define TAG_FFF016          0xFFF016    // DSDOL Related Data
#define TAG_FFF017          0xFFF017    // LOG submission label
#define TAG_FFF018          0xFFF018    // Card APDU instruction in LOG
#define TAG_FFF019          0xFFF019    // Card APDU response in LOG

#define TAG_FFF020          0xFFF020    // DET end identifier

#define TAG_FFF030          0xFFF030    // ICC LOG 1
#define TAG_FFF031          0xFFF031    // ICC LOG 2
#define TAG_FFF032          0xFFF032    // ICC LOG 3
#define TAG_FFF033          0xFFF033    // ICC LOG 4
#define TAG_FFF034          0xFFF034    // ICC LOG 5
#define TAG_FFF035          0xFFF035    // ICC LOG 6
#define TAG_FFF036          0xFFF036    // ICC LOG 7
#define TAG_FFF037          0xFFF037    // ICC LOG 8
#define TAG_FFF038          0xFFF038    // ICC LOG 9
#define TAG_FFF039          0xFFF039    // ICC LOG 10




// @Ljj 2017.03.22 qUICS newly added
// RAR payment account reference number
#define TAG_9F24            0x9F24
// Card Additional Processing
#define TAG_9F68            0x9F68

//2017 06 08 qVSDC newly added
// Issuer Script Results                       &    DSDOL  (MASTER)
#define TAG_9F5B            0x9F5B
// Form Factor Indicator ,return from gpo     Master -Third Party Data
#define TAG_9F6E            0x9F6E
// Electronic cash transaction limit
#define TAG_9F7B            0x9F7B
// Customer Exclusive Data
#define TAG_9F7C            0x9F7C

// 1th track data
#define TAG_DF8129                      0xDF8129
// When downgrading with magnetic stripe card, POSEntryMode value
#define TAG_DF8137                      0xDF8137
// Terminal Entry Capability :support connect 5;no support connect 8
#define TAG_DF8138                      0xDF8138
// 2th track data (obtained from 57)
#define TAG_DF8139                      0xDF8139

// DRL switch information
#define TAG_DF8140                      0xDF8140
//BYTE 1  BIT 1~2: ONLINE ODA   : 00 DEACTIVED;  01 ACTIVE
//BYTE 1  BIT 3~4: STATUS CHECK : 00 DEACTIVED;  01 ACTIVE
//BYTE 1  BIT 5~6: ZERO CHECK   : 00 DEACTIVED;  01 ONLINE;   10 NOT ALLOWED
//BYTE 1  BIT 7~8: DRL          : 00 NO       ;  01 YES
//BYTE 2  BIT 1~2: EXCEPTION FILE : 00 DEACTIVED;   01 ACTIVE
//BYTE 2  BIT 3~4: CL SWITCH        :    00 DEACTIVED;  01    ACTIVE
//BYTE 3  BIT 1~2: TERMINAL FLOOR LIMIT : 00 NOT PRESENT; 01 PRESENT
//BYTE 3  BIT 3~4: CL OFFLINE LIMIT     : 00 DEACTIVED  ; 01 ACTIVE & EXIST ; 10 ACTIVE & NOT EXIST
//BYTE 3  BIT 5~6: CL TRANS LIMIT       : 00 DEACTIVED  ; 01 ACTIVE & EXIST ; 10 ACTIVE & NOT EXIST
//BYTE 3  BIT 7~8: CL CVM LIMIT         : 00 DEACTIVED  ; 01 ACTIVE & EXIST ; 10 ACTIVE & NOT EXIST

// The switch information DRL and DF8140 are basically the same
#define TAG_DF8141                      0xDF8141
// DRL Parameter template
#define TAG_DF8142                      0xDF8142
// 3th track data
#define TAG_DF8149                      0xDF8149

// script TAG 20170509
#define TAG_DF8170                      0xDF8170
// Transaction result indicates that 0x01 application has expired,
// 0x02 online ODA failed, and 0x03 blacklisted card
#define TAG_DF8171                      0xDF8171
// Other switch information besides DRL
#define TAG_DF8172                      0xDF8172


// Online ODA execution status BIT0,0,sda,1,fdda BIT1,0,fail,1,success
#define TAG_DF8173                      0xDF8173
// need signature or not? add at 2018 01 07
#define TAG_DF8174                      0xDF8174


// TAG length limit
// Merchant name, this TAG has no maximum length. This is the maximum
// length customized for opening BUF, and the recommended length in the card transaction log is 40B
#define TAG_9F4E_MAX                        42
// Equivalent 2th-track data
#define TAG_0057_MAX                        19
// Minimum length of the cardholder's name
#define TAG_5F20_MIN                        2
// Maximum length of cardholder name
#define TAG_5F20_MAX                        26
// Custom data of track 1
#define TAG_9F1F_MAX                        256
// Application primary account serial number
#define TAG_5F34_MAX                        1
// NCA Issuing Bank Public Key Certificate NI
#define TAG_0090_MAX                        256
// Issuer's public key exponent
#define TAG_9F32_MIN                        1
// Issuer's public key index 1 or 3
#define TAG_9F32_MAX                        3
// Remainder of issuer's public key
#define TAG_0092_MAX                        252
// IC Card Public Key exponent
#define TAG_9F47_MIN                        1
// IC card public key index 1 or 3
#define TAG_9F47_MAX                        3
// Static application data of signature NI
#define TAG_0093_MAX                        256
// Number of Static Applications Signed (SAD) NI
#define TAG_9F46_MAX                        256
// CA Public Key Index (PKI)
#define TAG_008F_MAX                        1
// Cardholder Verification Method (CVM) List
#define TAG_008E_MAX                        252
// ISSUER ACT CODE (IAC) - Default
#define TAG_9F0D_MAX                        5
// Issuing Bank Action Code (IAC) - Reject
#define TAG_9F0E_MAX                        5
// Issuing Bank Action Code (IAC) - Online
#define TAG_9F0F_MAX                        5
// CDOL1
#define TAG_008C_MAX                        252
// CDOL2
#define TAG_008D_MAX                        252
// DDOL
#define TAG_9F49_MAX                        252
// Continuous offline transaction limit
#define TAG_9F23_MAX                        1
// Continuous offline transaction lower limit
#define TAG_9F14_MAX                        1
// IC card public key remainder
#define TAG_9F48_MAX                        100
// List of static data authentication labels
#define TAG_9F4A_MAX                        256
// Cardholder ID number
#define TAG_9F61_MAX                        40
#define TAG_9F61_MIN                        1
// Cardholder ID type
#define TAG_9F62_MAX                        1


// Application Primary Account (PAN)
#define TAG_005A_MAX                        10
// Application expiration date YYMMDD
#define TAG_5F24_MAX                        3
// Country code of issuing bank
#define TAG_5F28_MAX                        2
// Application Usage Control
#define TAG_9F07_MAX                        2
// Application effective date
#define TAG_5F25_MAX                        3
// Application version number
#define TAG_9F08_MAX                        2
// service code
#define TAG_5F30_MAX                        2
// Application currency code
#define TAG_9F42_MAX                        2

#define TAG_0082_MAX                        2
#define TAG_0094_MAX                        252
#define TAG_9F36_MAX                        2
#define TAG_9F26_MAX                        8
#define TAG_9F10_MAX                        32
#define TAG_9F6C_MAX                        2
#define TAG_9F5D_MAX                        6
#define TAG_9F4B_MAX                        252
#define TAG_9F74_MAX                        6
#define TAG_0097_MAX                        252
#define TAG_9F68_MAX                        4
#define TAG_9F24_MAX                        29

#define TAG_0084_MIN                        5
#define TAG_0084_MAX                        16   //5~16
#define TAG_0050_MIN                        1
#define TAG_0050_MAX                        16   //1~16
#define TAG_9F38_MIN                        1
#define TAG_9F38_MAX                        256  //1~256
#define TAG_5F2D_MIN                        2
#define TAG_5F2D_MAX                        8    //2~8
#define TAG_9F06_MAX                        16
#define TAG_9F11_MIN                        1
#define TAG_9F12_MIN                        1
#define TAG_9F12_MAX                        20   // May exceed 16 bytes
#define TAG_0073_MIN                        1
#define TAG_0073_MAX                        252  //1~252
#define TAG_BF0C_MIN                        1
#define TAG_BF0C_MAX                        222
#define TAG_9F27_MAX                        1
#define TAG_FF02_MAX                        256

#define	TAG_9F01_LEN						6
#define TAG_9F02_LEN						6
#define TAG_9F03_LEN						6
#define TAG_9F04_LEN						4
#define TAG_9F08_LEN                        2
#define TAG_9F09_LEN                        2
#define TAG_9F36_LEN                        2
#define TAG_9F37_LEN                        4
// Terminal capability parameter length
#define TAG_9F33_LEN                        3

// MASTER
#define TAG_9F60_LEN						2
#define TAG_9F61_LEN						2
#define TAG_9F62_LEN						6
#define TAG_9F64_LEN						1
#define	TAG_9F60_LEN						2
#define TAG_9F65_LEN						2
#define TAG_9F67_LEN						1
#define	TAG_DF4B_LEN						3
#define TAG_9F20_MAX						16
// DS Summary 1
#define TAG_9F7D_MAX                        16
#define TAG_DF8126_LEN                      6

#define TSI_LEN                             2
#define TVR_LEN                             5
#define TRANS_SUM_LEN                       6

// date length
#define DATE_LEN                            3
// time length
#define TIME_LEN                            3
// Date Time Length
#define DATE_TIME_LEN                       6
#define AUTH_RES_CODE_LEN                   2
#define DDOL_LEN                            252
#define TAC_LEN                             5
#define RID_LEN                             5
#define COUNTRY_CODE_LEN                    2
#define TERMINAL_NO_MAX_LEN                 8
#define BUSINESSMAN_NO_MAX_LEN              15
#define BUSINESSMAN_NAME_MAX_LEN            20
#define BUSINESSMAN_TYPE_LEN                2
#define HASH_RESULT_LEN                     20
#define CAPK_HASH_LEN                       20
#define IFD_LEN                             8
#define MAC_LEN_MAX                         16
#define LEN_MAC                             8

// Error indicator length
#define TAG_DF8115_MAX                      6
//UI Request Data
#define TAG_DF8116_MAX                      22
//OUT PARAM length
#define TAG_DF8129_MAX                      8


#define SW1SW2_6283                     0x6283
#define SW1SW2_6983                     0X6983
#define SW1SW2_6984                     0X6984
#define SW1SW2_6985                     0X6985
#define SW1SW2_6986                     0X6986
#define SW1SW2_6A81                     0x6A81
#define SW1SW2_6A82                     0x6A82
#define SW1SW2_6A83                     0x6A83
#define SW1SW2_SENDCMD_SUCC             0x9000

//---------------------------------------------------------------------------
// Data authentication type
//---------------------------------------------------------------------------
#define    TerIC_FDDA           4
#define    TerIC_CDA            3
#define    TerIC_DDA            2
#define    TerIC_SDA            1

#define    ICC_NO_IDS_NO_RRP    0
#define    ICC_NO_IDS_RRP       1
#define    ICC_IDS_NO_RRP       2
#define    ICC_IDS_RRP          3


// cash
#define TRANS_TYPE_CASH                     0x8000
// goods
#define TRANS_TYPE_GOODS                    0x4000
// service
#define TRANS_TYPE_SERVICES                 0x2000
// Cashback
#define TRANS_TYPE_CASHBACK                 0x1000
// query
#define TRANS_TYPE_QUERY                    0x0800
// transfer accounts
#define TRANS_TYPE_TRANSFER_ACCOUNTS        0x0400
// pay a sum of money
#define TRANS_TYPE_PAY_MONEY                0x0200
// manage
#define TRANS_TYPE_MANAGE                   0x0100
// Deposit transactions
#define TRANS_TYPE_DEPOSIT_MONEY            0x0080

// Payment consumption
#define TRANS_9C_PAYMENT                    0x00
// cash
#define TRANS_9C_CASH                       0x01
// Cashback purchase
#define TRANS_9C_PURCHASE_CASHBACK          0x09
//cash disbursement
#define TRANS_9C_CASH_DISBURSEMENT          0x17
// refund
#define TRANS_9C_REFUND                     0x20



//-----------------------------------------------------------------------------
// Macro Definition, Status Code
//-----------------------------------------------------------------------------
// <all> Command execution successful
#define SW9000            0x9000
// <AB><PCU> No information provided
#define SW6200            0x6200
// <AB><GR><IA><PCU><RB><RR> The data sent back may be incorrect
#define SW6281            0x6281
// <RB> file length < Le
#define SW6282            0x6282
// <AB><S> The selected file is invalid
#define SW6283            0x6283
// <S> FCI format does not match the one specified in P2
#define SW6284            0x6284
#define SW6300            0x6300                //
// <AB><AU><CB><IA><PCU><RR><S><V> The flag status bit not changed
#define SW6400            0x6400
// <AB><AU><CB><CFL><CP><DFPCW><DFU><GB><GTP><IFCW><IFL><IFP><IFU><IFUP><PCU><RP><UB><UOL><UR> Memory error (modification failed)
#define SW6581            0x6581
// <CFL><DFPCW><DFU><EA><GR><IA><IFL><IFU><IFUP><RB><RP><RR><S><UB><UOL><UR> Length error (Lc or Le incorrect)
#define SW6700            0x6700
// <IA> Security message not supported
#define SW6882            0x6882
// <UOL> Unable to process
#define SW6900            0x6900
// <CFL><DFPCW><DFU><UOL> Command not accepted (invalid state)
#define SW6901            0x6901
// <RB><RR><UB><UR> Command & file structure incompatibility
#define SW6981            0x6981
// <AB><AU><PCU><RB><UB><UR> Security status not met
#define SW6982            0x6982
// <CP><EA><V> Verification method (personal password) lock
#define SW6983            0x6983
// <AB><PCU><V> Invalid reference data
#define SW6984            0x6984
// <CFL><CP><DFPCW><GB><GTP><IA><IFCW><IFL><IFP><IFU><IFUP><RP><UOL> The usage conditions are not met
#define SW6985            0x6985
// <RB><UB><UR> The conditions for command execution are not met (not the current EF)
#define SW6986            0x6986
// <AB><AU><CB><PCU> Security message data item loss
#define SW6987            0x6987
// <AB><AU><CB><PCU><RP> security message data item is incorrect
#define SW6988            0x6988
// <CP><IA> Incorrect data field parameters
#define SW6A80            0x6A80
// <GC><IFL><RB><RR><S><UB><UR> Feature not supported
#define SW6A81            0x6A81
// <RB><RR><S><UB><UR> file not found
#define SW6A82            0x6A82
// <RR><UR> Record not found
#define SW6A83            0x6A83
// <UR> Insufficient file storage space
#define SW6A84            0x6A84
// <AB><CP><EA><GB><GC><GR><IA><IFL><IFU><IFUP><PCU><RP><S><V> Parameter P1 and P2 are incorrect
#define SW6A86            0x6A86
// <AB><PCU><RP><V> Reference data not found
#define SW6A88            0x6A88
// <RB><UB> Parameter error (offset address exceeds EF)
#define SW6B00            0x6B00
// Extended status code: INS incorrect
#define SW6D00            0x6D00
// Extended status code: CLA incorrect
#define SW6E00            0x6E00
// <GR> Invalid data
#define SW6F00            0x6F00
// <CFL><DFPCW><DFU><GB><UOL> MAC invalid
#define SW9302            0x9302
// <AU><PCU><RP> application has been permanently locked
#define SW9303            0x9303
// <IFCW><IFP><IFU> Insufficient amount
#define SW9401            0x9401
// <IFCW><IFL><IFP><IFU><IFUP> Key index not supported
#define SW9403            0x9403
// <GTP> Required MAC and TAC not available
#define SW9406            0x9406
#define SW6989            0x6989
// Extended status code: Status code does not return
#define SW6EFE            0x6EFE
// Extended status code: Status code does not return
#define SW0000            0x0000

#endif


