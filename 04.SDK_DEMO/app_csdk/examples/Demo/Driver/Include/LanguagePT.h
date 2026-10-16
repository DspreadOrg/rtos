/**********************************************************************************************************
 * Copyright(C)2011, 
 * Description:
 * Change Logs:
 * Date           Author         Notes
 * 2022-03-04     leeon          first version
 **********************************************************************************************************/

#ifndef _LANGUAGE_PT_H_
#define _LANGUAGE_PT_H_

//
#define SELECT_LANG                             "Selecione idioma"
#define INITIALIZE_LANG                         "linguagem Initialization"
#define SETTING_DONE                            "Configura��o conclu�da"
#define INITIALIZE_FAIL                         "Falha na inicializa��o"
#define USE_DEFAULT_MENU                        "Usar menu padr�o"
#define DEV_MODEL                               DEV_PN
#define UPGRADE_SUCCESS                         "Sucesso de atualiza��o"
#define SYSTEM_INIT                             "Sistema iniciando..."

//Main Menu
#define MENU_MAIN                               "Principal"
#define MENU_PAY                                " Pagar"
#define MENU_REFUND                             " Reembols"
#define MENU_RKI                                " RKI"
#define MENU_MQTT                               " MQTT Info"
#define MENU_SCAN                               " Escanear"
#define MENU_TEST                               " Teste"
#define MENU_MANAGE                             " Admin"
#define MENU_PRINT                              " Imprimir"
#define MENU_SETTING                            " Config"
#define MENU_MORE                               " Mais"
#define MENU_ESIGN                              " E-Sign"


//Test Menu
#define MENU_TEST_TITLE                         "Teste"
#define MENU_ONLINE                             "Teste de Rede"
#define MENU_ONLINESET                          "Config"
#define MENU_CONNECTION_MODE                    "Modo de liga��o"

//Test Submenu
#define MENU_TEST_SUBTITLE                      "Config"
#define MENU_NET_MODE_SETTING                   "Modo Rede"
#define MENU_APN_DNS_SETTING                    "APN&DNS"
#define MENU_OPERATOR_LIST                      "Provedor"
#define MENU_TCP                                "TCP"
#define MENU_SSL                                "SSL"

//Echo Test Menu
#define MENU_NETWORK_TEST                       "Teste de Rede [%d / %d]:"
#define MENU_SINGLE_ECHO                        "1.Unico Echo"
#define MENU_SHORT_LINK                         "2.Conexao curta"
#define MENU_LONG_LINK                          "3.Conexao longa"
#define MENU_RESTART_LINK                       "4.Reiniciar loop"
#define MENU_COMBINATION                        "5.Combinacao"

//GPRS Setting
#define MENU_SET_GPRS_MODE                      "Definir o Modo GPRS"                 //add
#define MENU_GPRS                               "GPRS"
#define MENU_WIFI                               "WiFi"
#define MENU_WIFI_PRIOR                         "WiFi priorit�rio"
#define MENU_MODE                               "Modo"

//Wifi Setting
#define MENU_NETWORK_SETTING                    "Rede"
#define MENU_SELECT_MODE                        "Selecionar Modo"
#define MENU_CONNECT_WIFI                       "Conectar WiFi"

//Admin Menu
#define MENU_MANAGE_TITLE                       "Admin"
#define MENU_LOG_IN                             " Conectar"
#define MENU_LOG_OUT                            " Desconectar"
#define MENU_PAY_QUERY                          " Consulta Pagmnt"
#define MENU_DEV_INFO                           " Info Disp"
#define MENU_VER_INFO                           " Info Versao"
#define MENU_FW_OTA                             " Atualizacao"
#define MENU_TMS_UPGRADE                        " Atualiza��o do TMS"

//imprimir menu
#define MENU_PRINT_TITILE                       "Imprimir"
#define MENU_PRINT_RECEIPT                      "Recibo"
#define MENU_CALIB_SCREEN                       "Calibrar"
#define MENU_PRINT_ESIG                         "e-Sign"
#define MENU_ESIGN_TITILE                       "E-Sign"

//Query transaction history menu
#define MENU_QUERY_TITLE                        "Consulta de pagamento"
#define MENU_QUERY_DETAIL                       "Detalhes"
#define MENU_QUERY_TOTAL                        "Sumario"

//OTA menu
#define MENU_OS_UPDATE                          "SO"
#define MENU_WIFI_UPDATE                        "WiFi Firmware"
#define MENU_SE_UPDATE                          "SE Firmware"
#define MENU_FILE_UPDATE                        "File Firmware"
#define MENU_APP_FW_UPDATE                      "App Firmware"
#define MENU_FIRMWARE_OTA                       "Atualizacao OTA"
#define MENU_OS_FOTA_UPGRADE                    "Atualizacao SO OTA"
#define MENU_APP_FW_UPGRADE                     "Atualizacao firmware App"

//Setting menu
#define MENU_SET_TITLE                          "Configuracoes"
#define MENU_NET_SETTING                        "Rede"
#define MENU_BRIGHTNESS                         "Brilho Tela"
#define MENU_SYSTIME_SETTING                    "Tempo do sistema"
#define MENU_BEEP                               "Bip"
#define MENU_THEME_COLOR                        "cor do tema"
#define MENU_LANGUAGE_SETTING                   "Linguagem"
#define MENU_LAYER_DEMO                         "exibi��o de camadas"
#define MENU_POWER_MANAGE                       "Gerenciamento de energia"
#define MENU_BACKLIGHT_TIME                     "tempo de retroilumina??o"
#define MENU_SLEEP_TIME                         "tempo de sono"
#define MENU_POWEROFF_TIME                      "tempo de desativa??o"

//??????
#define MENU_DATE_AND_TIME                      "Date & Time"
#define MENU_TIME_ZONE                          "Fuso- hor�rio"
#define MENU_NTP                                "NTP"

//NTP
#define MENU_NTP_SERVER                         "O servidor NTP"
#define MENU_NTP_SYNC                           "sincroniza��o"
#define MENU_NTP_AUTO_SYNC                      "autom�tica"

#define MENU_NTP_SYNC_SWITCH                    "Mudar"
#define MENU_NTP_SYNC_SETTING                   "Configura��es"

//POSP setting
#define MENU_POSP                               "POSP"
#define MENU_POSP_MODE                          "Acesso POSP"
#define MENU_TMS                                "TMS"
#define MENU_TMS_MODE                           "TMS Modo"
#define MENU_NETWORK                            "Configurar"

//TMS Server setting
#define MENU_TMS_APP_SERVER                     "App Servidor"
#define MENU_TMS_APP_SERVER_MODE                "App Servidor Modo"
#define MENU_TMS_OTHER_SERVER                   "outro Servidor"

//Theme settings
#define MENU_THEME_TITLE                        "cor do tema"
#define MENU_THEME_BLUE                         "tema azul"
#define MENU_THEME_ORANGE                       "tema laranja"
#define MENU_THEME_RED                          "tema vermelho"

//APN setting
#define MENU_APN                                "APN"
#define MENU_DNS                                "DNS"
#define MENU_APN_DNS                            "APN & DNS"
#define MENU_APN_SET                            "Configurando APN"
#define MENU_DNS_SET                            "Configurando DNS"

//RKI
#define MENU_RKI_GET_CERT                       "GetCertificate"
#define MENU_RKI_BIND                           "Bind"
#define MENU_RKI_KEY                            "KEY"
#define MENU_RKI_UNBIND                         "unBind"
#define MENU_RKI_REBIND                         "reBind"


//USB setting
#define MENU_USB_CCID                           "USB CCID"
#define MENU_USB_COM                            "USB COM"

//More
#define MENU_MAINTENANCE                        "Manutencao"
#define MENU_SELF_CHECK                         "Verificacao dispositivo"
#define MENU_DEBUG_SWITCH                       "Depuracao"
#define MENU_USB_MODE                           "Modo USB"
#define MENU_FUNCTION_TEST                      "Function Test"

//Single item self-test menu
#define MENU_DEV_CHECK                          "Verificacao dispositivo"
#define MENU_FEATURE_CHECK                      "Verificacao recursos"
#define MENU_AGING_CHECK                        "Envelhecimento"
#define MENU_CHECK_DISP                         ".LCD & LED"
#define MENU_CHECK_KEYS                         ".Teclas"
#define MENU_CHECK_PICC                         ".Cartao sem contato"
#define MENU_CHECK_MCR                          ".Cartao com tarja magnetica"
#define MENU_CHECK_ICC                          ".Cartao de chip"
#define MENU_CHECK_CHG                          ".Carregamento USB"
#define MENU_CHECK_SCAN                         ".Scanner"
#define MENU_CHECK_PRN                          ".Impressora"
#define MENU_CHECK_NET                          ".Rede"
#define MENU_CHECK_WIFI                         ".WiFi"

//system prompt
#define TIP_SYS_INIT                            "Sistema iniciando..."
#define TIP_BAT_LOW_TO_CHG                      "Bateria baixa!"
#define TIP_BAT_LOW_SHUT_DOWN                   "Desligando!"
#define TIP_IS_SHUT_DOWN                        "Confirmar desligamento?"

//payment prompt
#define TIP_ENTER_AMOUNT                        "Insira o valor:"
#define TIP_SHOP_NAME                           "www.shopname.com"
#define TIP_TRADE_OK                            "Pagamento efetuado!"       //Modify because of too long
#define TIP_AMOUNT                              "Valor:"
#define TIP_PRINTING                            "Imprimindo..."
#define TIP_AMOUNT_OVER                         "Valor excedido"
#define TIP_CONFIRM_AMOUNT                      "Confirmar valor?"
#define TIP_PAY_BY_CARD                         "Por favor, pague com cartao!"
#define TIP_PULL_OUT_CARD                       "Remova o cartao"
#define TIP_RECORDS_FULL                        "Registro esta cheio"
#define TIP_QR_TRADE_SUCC                       "Pagamento efetuado!"       //Modify because of too long

//Signing order prompt
#define TIP_RECEIPT                             "Comprovante"
#define TIP_MERCHANT_ID                         "ID estabelecimento:"
#define TIP_MERCHANT_NO                         "Nr estabelecimento:"
#define TIP_TERMINAL_ID                         "ID Terminal:"
#define TIP_CASHIER_NO                          "Nr Caixa:"
#define TIP_TOTAL                               "Total:"
#define TIP_PAY_TYPE                            "Tipo:"
#define TIP_DATE_TIME                           "Data/Hora:"
#define TIP_PAY_CARD                            "Cartao"                    //Modify because of not simplified enough
#define TIP_PAY_SCAN                            "QR"                        //Modify because of not simplified enough
#define TIP_PAY_UNKNOW                          "Outros"                    //Modify because of not simplified enough
#define TIP_PURCHASE                            "OBRIGADO PELA COMPRA!"

//network prompt
#define TIP_SEARCH_NET_OPER                     "Procurando redes"
#define TIP_SEARCH_SUCC                         "Sucesso na busca"
#define TIP_SEARCH_FAIL                         "Falha na busca"
#define TIP_NO_SIM_CARD                         "Cartao SIM nao encontrado"
#define TIP_NET_SEND_DATA                       "Enviando dados..."
#define TIP_NET_RECV_DATA                       "Recebendo dados..."
#define TIP_NET_INIT                            "Inicializando Rede"
#define TIP_NET_INIT_TIMEOUT                    "Tempo de inicio esgotado"     //Modify because of too long
#define TIP_NET_CONNECT                         "Conectando"
#define TIP_NET_CONNECT_TIMEOUT                 "Tempo limite de conexao!"
#define TIP_NET_CONNECT_FAIL                    "Falha na conexao"
#define TIP_NET_RESTART                         "Reiniciando rede..."
#define TIP_NET_READ_DATA                       "Ler dados"
#define TIP_NET_READ_TIMEOUT                    "Leia o tempo limite!"      //add
#define TIP_NET_WIFI_ABNORMAL                   "WiFi est?anormal"
#define TIP_NET_SWITCH_GPRS                     "A rede mudar?para o modo GPRS"

//Wifi prompt
#define TIP_WIFI_INFO                           "WiFi"
#define TIP_WIFI_INFO_GET_FAIL                  "Falha ao buscar info WiFi"
#define TIP_WIFI_STATIC_IP                      "IP est�tico"                       //add
#define TIP_WIFI_DYNAMIC_IP                     "IP Dinamico"
#define TIP_WIFI_GATEWAY                        "Gateway"
#define TIP_WIFI_DNS1                           "DNS1"
#define TIP_WIFI_IP                             "IP"
#define TIP_WIFI_SUBWAY                         "Subnet"
#define TIP_WIFI_DNS2                           "DNS2"
#define TIP_WIFI_MAC                            "MAC"
#define TIP_WIFI_BID                            "BID"
#define TIP_WIFI_SEARCH                         "Procurando redes"
#define TIP_WIFI_GET_LIST                       "Buscando lista WiFi"
#define TIP_WIFI_NO_AVAILABLE                   "Sem redes WiFi disponiveis"
#define TIP_WIFI_NAME_UNAVAILABLE               "Nome de WiFi indisponivel"       //Modify because of too long
#define TIP_WIFI_AP_LIST                        "Lista WiFi"
#define TIP_WIFI_FORGET_NETWORK                 "Deletar este WiFi?"
#define TIP_WIFI_DEL_SUCC                       "WiFi deletado!"
#define TIP_WIFI_INPUT_SSID                     "SSID"
#define TIP_WIFI_INPUT_PASS                     "Senha"
#define TIP_WIFI_SSID                           "SSID"
#define TIP_WIFI_PASS                           "Senha"
#define TIP_WIFI_CONNECT_SUCC                   "Conexao com sucesso!"
#define TIP_WIFI_CONNECT_FAIL                   "Falha na conexao!"
#define TIP_WIFI_PLEASE_CHECK                   "Por favor, verifique a rede"
#define TIP_WIFI_UNABLE_CONNECT                 "Conexao WiFi s/ sucesso"               //Modify because of too long
#define TIP_WIFI_CONNECTIONG                    "Conectando..."

//Echo Test Prompt
#define TIP_ONCE_LINK                           "unico echo"
#define TIP_SHORT_LINK                          "conexao curta"
#define TIP_LONG_LINK                           "conexao longa"
#define TIP_RESTART_LINK                        "reiniciar loop"
#define TIP_COMBINATION_TEST                    "combinacao"
#define TIP_SET_TIMES                           "Configurar vezes:"
#define TIP_RESTART_FAIL                        "Falha na reinicializacao"
#define TIP_INIT_FAIL                           "Falha na inicializacao"
#define TIP_CONNECT_FAIL                        "Falha na conexao"
#define TIP_SEND_FAIL                           "Falha no envio"
#define TIP_LINK_FAIL                           "Falha na conexao"
#define TIP_RECV_FAIL                           "Falha no recebimento"
#define TIP_ECHO_SUCC                           "Sucesso no echo"
#define TIP_ECHO_FAIL                           "Falha no echo"
#define TIP_TIMES_AND_LEN                       "Vezes:[%d/%d],Tamanho[%d]"
#define TIP_PASS_AND_FAIL                       "Sucesso:%d,Falha:%d"

//Echo test result prompt
#define TIP_DEVICE_PN                           "PN:"
#define TIP_OS_VER                              "SO:"
#define TIP_SDK_VER                             "SDK:"
#define TIP_APP_VER                             "APP:"
#define TIP_SE_VER                              "SE:"
#define TIP_START_TIME                          "Inicio:"
#define TIP_END_TIME                            "Fim:"
#define TIP_NET_OPERATOR                        "Provedor:"
#define TIP_NET_MODE                            "Modo Rede:"
#define TIP_NET_MODE_AUTO                       "2G/4G"
#define TIP_NET_MODE_2G                         "2G"
#define TIP_NET_MODE_4G                         "4G"
#define TIP_NUM_OF_TEST                         "Tempos teste:"
#define TIP_VARIABLE_LEN                        "Comprim dados:"
#define TIP_VARIABLE_RANGE                      "Intervalo dados:"
#define TIP_SIGNAL_RANGE                        "Faixa sinal:"
#define TIP_SIGNAL_STRENGTH                     "Forca sinal:"
#define TIP_NUM_OF_PASS                         "Tempos sucessos:"                      //add
#define TIP_NUM_OF_INIT_FAIL                    "Inicializando c/ falha:"
#define TIP_NUM_OF_LINK_FAIL                    "Conectar c/ falha:"                    //add
#define TIP_NUM_OF_SEND_FAIL                    "Envio c/ falha:"
#define TIP_NUM_OF_RECV_FAIL                    "Recib c/ falha:"
#define TIP_NUM_OF_COMP_FAIL                    "Compare Fail:"                         //add
#define TIP_TEST_DURATION                       "Duracao:"
#define TIP_TEST_CITY                           "Localizacao:"

//Manage prompts
#define TIP_LOG_IN_SUCC                         "Sucesso ao conectar-se!"
#define TIP_NO_RECORD                           "Sem registro!"
#define TIP_MONEY                               "Quantidade"
#define TIP_STATUS                              "Estado"
#define TIP_PAY_OK                              "Pagamento efetuado!"                 //Modify because of too long
#define TIP_OPCODE                              "Tp pgto"                               //Modify because of too long
#define TIP_TRANS_NO                            "Nr pgto"                                    //Modify because of too long
#define TIP_TIME                                "Hora pgto"                                  //Modify because of too long
#define TIP_OTHERS                              "Outros"

//Device information prompt
#define TIP_DEV_INFO                            "Dispositivo"
#define TIP_MODEL                               "Modelo"
#define TIP_SN                                  "Nr Serie"
#define TIP_IMEI                                "IMEI"
#define TIP_POWER                               "Energia"

//Version information prompt
#define TIP_OS                                  "SO"
#define TIP_SDK                                 "SDK"
#define TIP_APP                                 "APP"
#define TIP_SE                                  "SE"
#define TIP_HW                                  "HW"
#define TIP_WIFI                                "WiFi"

//upgrade prompt
#define TIP_OTA_FAIL                            "Falha na atualizacao OTA"
#define TIP_OTA_CMD_VER_SEND_FAIL               "Falha na solicita��o de vers�o"
#define TIP_OTA_CMD_VER_RECV_TIMEOUT            "Tempo esgotado de recebnto"         //can't display
#define TIP_OTA_VER_CHECK_FAIL                  "Falha na verificacao de versao"
#define TIP_FS_FREE_SIZE_NOT_ENOUGH             "Sem espaco suficiente"
#define TIP_FW_FILE_OPEN_FAIL                   "Falha ao construir caminho"
#define TIP_OTA_CMD_DOWNLOAD_FAIL               "Erro de formato de solicitacao"
#define TIP_CMD_DOWNLOAD_SEND_FAIL              "Falha na solicitacao de dw"       //can't display
#define TIP_CMD_RECV_DOWNLOAD_TIMEOUT           "S/ resposta de sol. de dw"   //can't display
#define TIP_CMD_DEPACAGE_DOWNLOAD_FAIL          "Falha na verif. do pacote"          //can't display
#define TIP_RECV_FILE_OFFSET_ERR                "Pacote receb. nao corresp."             //can't display
#define TIP_PACK_SIZE_ERR                       "Erro de tamanho do pacote"
#define TIP_FW_FILE_WRITE_FAIL                  "Falha ao gravar o arquivo"
#define TIP_OTA_EXIT                            "Sair da atualiza��o"
#define TIP_OTA_CMD_FINISH_PACK_FAIL            "Erro de formato de resp."      //can't display
#define TIP_OTA_CMD_FINISH_SEND_FAIL            "Falha ao enviar resp."         //can't display
#define TIP_DOWNLOAD_FINISH                     "Download concluido"
#define TIP_SETUP_AND_WAIT                      "Instalando..."
#define TIP_SETUP_FAIL                          "Falha na instalacao"
#define TIP_TOTAL_SIZE                          "Progresso total"                               //add
#define TIP_DOWNLOAD_SIZE                       "Progresso do download"                         //add

//Dicas relacionadas ao TMS
#define TIP_TMS_TASK_REQUEST                    "Solicita�ao de tarefa TMS"
#define TIP_TMS_SOFT_DOWNLOAD                   "Download de firmware TMS"
#define TIP_TMS_DOWNLOAD_DATA_ERR               "erro de download de dados"
#define TIP_TMS_NO_SOFT_TO_DOWNLOAD             "Sem firmware para download"
#define TIP_TMS_CHECK_SOFT                      "Confira o firmware "
#define TIP_TMS_FIRM_APP_DATA_ERR               "Erro de dados de firmware"
#define TIP_TMS_PARA_DATA_ERR                   "Erro de dados de par�metro"
#define TIP_TMS_DOWNLOAD_CONFIRM                "Download do TMS confirmar"
#define TIP_TMS_DOWNLOAD_PROGRESS               "Progresso:"
#define TIP_TMS_DOWNLOAD_DONE                   "Download feito, validando"
#define TIP_TMS_SOFT_UP_TO_DATE                 "Firmware TMS atualizado"
#define TIP_TMS_PARA_DOWNLOAD                   "Download de par�metros TMS"
#define TIP_TMS_PARA_UP_TO_DATE                 "Par�metros TMS atualizados"
#define TIP_TMS_PARA_DOWNLOAD_DONE              "download de parametros feito"
#define TIP_TMS_READY_STORAGE_SPACE             "pronto para armazenar"
#define TIP_TMS_KEEP_UPGRADE                    "Continue atualizando..."
#define TIP_TMS_WAIT_REBOOT                     "Aguarde para reiniciar"
#define TIP_TMS_NONE_MODULE_INFO                "Sem informa��es do m�dulo"
#define TIP_TMS_GET_MODULE_INFO                 "Obter informa��es do m�dulo"
#define TIP_TMS_MODULE_DOWNLOAD                 "Baixe o firmware do m�dulo"
#define TIP_TMS_MODULE_UP_TO_DATE               "j?a vers�o mais recente"
#define TIP_TMS_MODULE_VER_NO_MATCH             "vers�o incompat�vel"
#define TIP_TMS_MODULE_DATA_ERR                 "Erro de dados de firmware"

//setting prompt
#define TIP_DATE_AND_TIME                       "Data & Hora"
#define TIP_APN_SET                             "Configurando APN:"
#define TIP_APN_USERNAME_SET                    "Nome de usuario:"
#define TIP_APN_PASSWORD_SET                    "Senha:"
#define TIP_DNS_MAIN                            "DNS principal:"
#define TIP_DNS_BACK                            "DNS Secundario:"
#define TIP_SERVER_IP_MAIN                      "Host1 IP:"
#define TIP_SERVER_PORT_MAIN                    "Host1 porta:"
#define TIP_SERVER_IP_BACK                      "Host2 IP:"
#define TIP_SERVER_PORT_BACK                    "Host2 porta:"
#define TIP_DOMAIN_NAME_MAIN                    "Dominio 1:"
#define TIP_DOMAIN_PORT_MAIN                    "Dominio 1 porta:"
#define TIP_DOMAIN_NAME_BACK                    "Dominio 2:"
#define TIP_DOMAIN_PORT_BACK                    "Dominio 2 porta:"
#define TIP_POSP_CFG                            "Config POSP"
#define TIP_TMS_SERVER_IP_MAIN                  "TMS Host1 IP:"
#define TIP_TMS_SERVER_PORT_MAIN                "TMS Host1 porta:"
#define TIP_TMS_SERVER_IP_BACK                  "TMS Host2 IP:"
#define TIP_TMS_SERVER_PORT_BACK                "TMS Host2 porta:"
#define TIP_TMS_DOMAIN_NAME_MAIN                "TMS Domain1:"
#define TIP_TMS_DOMAIN_PORT_MAIN                "TMS Domain1 porta:"
#define TIP_TMS_DOMAIN_NAME_BACK                "TMS Domain2:"
#define TIP_TMS_DOMAIN_PORT_BACK                "TMS Domain2 porta:"
#define TIP_TMS_OS_DOMAIN_NAME_MAIN             "OS Domain1:"
#define TIP_TMS_OS_DOMAIN_PORT_MAIN             "OS Domain1 porta:"
#define TIP_TMS_CFG                             "Config TMS"
#define TIP_CURRENT_RESULT                      "Resultado atual"
#define TIP_POSP_MODE_SETTING                   "Configura��o de acesso POSP"
#define TIP_TMS_MODE_SETTING                    "Configura��o de acesso TMS"
#define TIP_IS_USE_DOMAIN                       "Access with domain?"
#define TIP_BEEP_SETTING                        "Configuracao Bip"
#define TIP_EN_TO_DEBUG_TOOLS                   "[Ligado] debug"                     //Modify can't display
#define TIP_EN_TO_FILE                          "[Ligado] para arquivos"
#define TIP_EN_TO_UPLOAD_FILE                   "[Ligado] upload de arqs"                      //Modify can't display
#define TIP_GPRS_IS_SELECT                      "Selecionar GPRS?"
#define TIP_WIFI_IS_SELECT                      "Selecionar WiFi?"
#define TIP_GPRS_AND_WIFI_SELECT                "Selecionar GPRS+WiFi?"
#define TIP_WIFI_PRIOR                          "Selecionar WiFi priorit�rio?"
#define TIP_UNIT_S                              "segundo"
#define TIP_UNIT_MIN                            "minuto"
#define TIP_POWER_CURRENT_TIME                  "hora atual:"
#define TIP_POWER_BACKLIGHT_TIME_SETTING        "Defini??o do tempo de retroilumina??o"
#define TIP_POWER_SLEEP_TIME_SETTING            "Defini??o do tempo de sono"
#define TIP_POWER_POWEROFF_TIME_SETTING         "Defini??o do tempo de desativa??o"
#define TIP_GPRS_APN_INFO                       "APN Informa??es"
#define TIP_GPRS_APN_AUTH_MATCH                 "APN correspond��ncia autom��tica"
#define TIP_GPRS_APN_IP                         "IP"
#define TIP_GPRS_APN_IPV6                       "IPV6"
#define TIP_GPRS_APN_IPV4V6                     "IPV4V6"
#define TIP_GPRS_APN_NONE                       "None"
#define TIP_GPRS_APN_PAP                        "PAP"
#define TIP_GPRS_APN_CHAP                       "CHAP"
#define TIP_GPRS_APN_PROCOTOL                   "Procotol:"
#define TIP_GPRS_APN_AUTH_TYPE                  "Tipos de autentica??o:"
#define TIP_GPRS_APN_LOADING                    "carregamento... Por favor, aguarde."
#define TIP_GPRS_APN_FUN                        "FUN"
#define TIP_GPRS_APN_MAN                        "MAN"
#define TIP_GPRS_APN_LIST                       "APN listagens"
#define TIP_APN_PROTOCOL_SET                    "Configura??o do protocolo:"
#define TIP_APN_AUTH_TYPE_SET                   "Definir o tipo de autentica??o:"
#define TIP_APN_ENTE_NUM_SELECT                 "Introduzir um n��mero para selecionar"
#define TIP_SIM_INIT_SUCC                       "Cart?o SIM inicializado com ��xito"
#define TIP_INSERT_SIM_CARD                     "Introduzir o cart?o SIM"

//LCD and LED self-test prompts
#define TIP_CHECK_FLASH_LAMP                    "Verificar a lanterna?"
#define TIP_ENTER_CONTINUE                      "Aperte [O] para continuar"
#define TIP_CANCEL_EXIT                         "Aperte [X] para sair"
#define TIP_COLOR_BLUE                          "Azul"
#define TIP_COLOR_YELLOW                        "Amarelo"
#define TIP_COLOR_GREEN                         "Verde"
#define TIP_COLOR_RED                           "Vermelho"
#define TIP_CUR_LED_COLOR                       "Cor LED atual:"
#define TIP_TURN_ON_LED_2S                      "Ligar LED, Intervalo 2s"
#define TIP_LED_TEST_END                        "Fim do teste de LED!"

//Key self-test prompt
#define TIP_KB_UP                               "CIMA"
#define TIP_KB_DOWN                             "BAIXO"
#define TIP_KB_MENU                             "MENU"
#define TIP_KB_FUN                              "FUNC"
#define TIP_KB_CLEAR                            "<"
#define TIP_KB_ENTER                            "O"
#define TIP_KB_CANCEL                           "X"
#define TIP_KB_ZM                               "ALPHA"
#define TIP_PRESS                               "Aperte a tecla "
#define TIP_KEY                                 " "
#define TIP_IS_BEEP_OK                          "O bip esta funcionando?"              //Modify because of too long
#define TIP_ENTER_CANCLE                        "[O]Sim, [X]N�o"

//Key self-test prompt
#define TIP_NFC_TEST                            "Teste NFC:"
#define TIP_M_TIMES_N_SUCC                      "%d teste, %d sucessos"
#define TIP_SWIPE_PICC_CARD                     "Aproxime o cart�o NFC..."
#define TIP_MOVE_PICC_CARD                      "Remover cart�o sem contato..."
#define TIP_CANCLE_TO_NEXT                      "Cancelar:Proximo"

//Prompt for contactless card self-check
#define TIP_MSC_TEST                            "Teste cartao com tarja mag:"     //Modify because of too long
#define TIP_MSC_N1_N2_N3                        "1MSC %d, 2MSC %d, 3MSC %d"
#define TIP_MSC_TEST_3_TIMES                    "MSC precisa completar 3 testes."
#define TIP_MSC_TRACK2_FAIL                     "Falha na faixa 2"
#define TIP_MSC_TRACK3_FAIL                     "Falha na faixa 3"
#define TIP_MSC_TIMEOUT                         "Tempo limite de MSC"
#define TIP_TEST_N_TIMES                        "Testar %d vezes"
#define TIP_REPEAT_NEXT                         "Enter:Repeat, Cancel:Next"

//Contact card self-check prompt
#define TIP_ICC_TEST                            "Teste cart�o com chip:"
#define TIP_TOUCH_ICC                           "Inserir cartao IC..."
#define TIP_MOVE_ICC                            "Remover cartao IC..."

//USB detection prompt
#define TIP_INSERT_USB                          "Por favor, insira cabo USB..."

//Scan code prompt
#define TIP_QR_OPEN_FAIL                        "Falha ao abrir o QR"
#define TIP_QR_PLEASE_SCAN                      "Por Favor escaneie"
#define TIP_QR_M_SCAN_N_SUCC                    "Cod leitura, %d leit, %d ok"              //Modify because of too long
#define TIP_QR_RESULT_RET                       "Result cod de leitura[%d]:"                        //Modify because of too long
#define TIP_QR_CODE_SUCC                        "Sucesso ao ler codigo"
#define TIP_QR_CODE_FAIL                        "Falha ao ler codigo"

//printer prompt
#define TIP_PRN_EXIT                            "Sair do teste de impressao"
#define TIP_PRN_TESTING                         "Testando..."
#define TIP_PRN_PAPER_OUT                       "Sem papel"
#define TIP_PRN_PAPER_ADD                       "Adicionar papel[Enter]"
#define TIP_PRN_IS_CLEAR                        "A nitidez da impressao esta boa?"
#define TIP_PRN_CHECK_SUCC                      "Verif. de imp. OK"                //Modify because of too long
#define TIP_PRN_CHECK_FAIL                      "Falha na verif. de imp"                  //Modify because of too long
#define TIP_PRN_OPEN_FAIL                       "Falha ao ligar a impressora"
#define TIP_PRN_PRINTING                        "Impress�o..."
#define TIP_PRN_ERR                             "Erro da impressora"

//Prompt de sinal eletr�nico
#define TIP_TOUCH_SCR_CALIB                     "Calibra��o da tela de toque"
#define TIP_SIGN_ERR                            "Erro de assinatura"
#define TIP_SIGN_FAIL                           "Falha na assinatura"
#define TIP_SIGN_SUCC                           "Sucesso de assinatura"

//GPRS prompt
#define TIP_GPRS_CHECK                          "Teste GPRS:"
#define TIP_GPRS_TEST_N_TIMES                   "Teste forca sinal %d Vzs"                        //Modify because of too long
#define TIP_GPRS_TEST_STRENGTH                  "Teste de forca de sinal GPRS"
#define TIP_GPRS_DEF_N_DBM                      "Padrao -113dBm(-113 to -51)"
#define TIP_GPRS_TEST_FAIL                      "Teste falhou"
#define TIP_GPRS_TEST_SUCC                      "Teste passou"
#define TIP_GPRS_SIGNAL_N_DBM                   "Forca do sinal atual: %ddBm"

//Wifi prompt
#define TIP_WIFI_TEST                           "Teste WiFi:"
#define TIP_WIFI_DISCONNECTED                   "WiFi desconectado..."
#define TIP_WIFI_CONNECTING                     "Conectando ao WiFi..."
#define TIP_WIFI_CONN_SUCC                      "WiFi conectado"
#define TIP_WIFI_CONN_FAIL                      "Falha ao conectar WiFi"
#define TIP_NET_TIMEOUT                         "Tempo limite de rede"
#define TIP_WIFI_SIGNAL_N_DBM                   "Forca de sinal:%ddBm"
#define TIP_WIFI_SIGNAL_TEST_SUCC               "Teste de sinal passou"
#define TIP_WIFI_CONN_TCP_SERVER                "Conectando ao servidor TCP..."
#define TIP_CONN_FAIL                           "Conexao falhou"
#define TIP_SEND_DATA_FAIL                      "Falha ao enviar dados"
#define TIP_TEST_FAIL                           "Teste falhou"
#define TIP_TEST_SUCC                           "Teste com sucesso"
#define TIP_ERR_CODE_FAIL                       "Codigo de erro Falha"
#define TIP_ERR_CODE_SUCC                       "Codigo de erro Sucesso"

//The whole machine self-check prompt
#define TIP_SELF_CHECK_MODE                     "Modo autoverificacao"
#define TIP_PLEASE_RETURN                       "Por favor retorne..."
#define TIP_DEV_CHECK_PASS                      "Verificacao de disp passou"
#define TIP_DEV_CHECK_FAIL                      "Falha na verificacao disp"                    //Modify because of too long
#define TIP_PLEASE_SHUT_DOWN                    "Por favor desligue..."

//Others
#define TIP_PLEASE_WAIT                         "Aguardando..."
#define TIP_WAIT_3MINS                          "Espera (MAX 3mins)!"               //add
#define TIP_WAITTING_CHA                        "Espera"                            //add
#define TIP_OK_CHA                              "OK"
#define TIP_CFG_CHA                             "CFG"
#define TIP_DEL_CHA                             "DEL"
#define TIP_SET_CHA                             "Definir"
#define TIP_CLR_CHA                             "CLR"
#define TIP_CON_CHA                             "CON"
#define TIP_UD_CHA                              "U/D"
#define TIP_OK                                  "Ok!"
#define TIP_ERROR                               "Erro!"
#define TIP_YES                                 "Sim"
#define TIP_NO                                  "N�o"
#define TIP_ON                                  "Ligado"
#define TIP_OFF                                 "Desligado"
#define TIP_ON_EN                               "[Ligado]"
#define TIP_OFF_EN                              "[Desligado]"
#define TIP_NONE                                "Nenhum"
#define TIP_NUM                                 "NUM"
#define TIP_CHA                                 "CHA"
#define TIP_CHA_NUM                             "CHA/NUM"
#define TIP_ALPHA_INPUT                         "Entrada alfabetica"
#define TIP_DIGIT_INPUT                         "Entrada numerica"
#define TIP_CURRENT                             "Atual"
#define TIP_DATA_ERR                            "Erro de dados!"
#define TIP_SET_SUCC                            "Definido com sucesso"
#define TIP_SET_FAIL                            "Falha na configuracao"
#define TIP_WAIT_TIMEOUT                        "Tempo limite de espera"
#define TIP_X_TO_EXIT_OTHERS_CONTINUE           "[X]:Sair, outros para continuar"
#define TIP_ANY_TO_END_TEST                     "Aperte qualquer tecla para sair"
#define TIP_ANY_TO_CONTINUE                     "Qualquer tecla para continuar"
#define TIP_ANY_TO_EXIT                         "Qualquer tecla para sair"
#define TIP_CANCEL_TO_STOP                      "Aperte [X] para sair!"
#define TIP_ENTER_TO_EDIT                       "Aperte [O] para editar"
#define TIP_O_CONFIRM_X_CANCEL                  "[O]:confirmar, [X]:cancelar"
#define TIP_ARR_NUM_TO_SET                      "Aperte F1/F2 ou num p/ selecionar"        //Modify because of too long
#define TIP_NOT_SUPPORT                         "Ainda nao suportado!"
#define TIP_PLEASE_INPUT                        "Entrada de par�metro:"
#define TIP_PARA_CANNOT_EMPTY                   "Parametro vazio!"
#define TIP_PARA_CANNOT_MORE                    "Valor excede"
#define TIP_PARA_LEN_LESS                       "Comprimento inferior a"
#define TIP_PARA_CANNOT_ZERO                    "Parametro zero!"
#define TIP_PARA_LEN_ERR                        "Erro de comprimento"
#define TIP_PARA_INPUT_ERR                      "Erro de dados"
#define TIP_PASS                                "PASSAR:"                               //add
#define TIP_UNKNOWN                             "desconhecido"                          //add
#define TIP_USER_CANCEL                         "Cancelar usu�rio"                      //add
#define TIP_UNLOCKING                           "Confirmar desbloqueio?"
#define TIP_SET_AUTOMODE                        "Definir o modo autom�tico"
#define TIP_XOR_CHECK_ERR                       "Erro de verifica�ao XOR"
#define TIP_UPGRADING                           "Fazendo upgrade..."
#define TIP_UPGRADE_FAIL                        "Falha no upgrade"
#define TIP_PLEASE_REBOOT_LATER                 "Reinicie mais tarde"
#define TIP_CHARGING                            "cobrando"
#define TIP_ARROW_TO_SET                        "Modificado por F1/F2"
#define TIP_SYNC_PERIOD                         "Per�odo"
#endif

