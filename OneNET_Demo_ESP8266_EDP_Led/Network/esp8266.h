#ifndef   ESP8266_H_H
#define   ESP8266_H_H

#define   DEVICEID   "771323"
#define   APIKEY     "0f3FXdmZ7od0iuVYVZBkiJeaJbg="

#define   AT          "AT\r\n"	
#define   CWMODE      "AT+CWMODE=1\r\n"
#define   RST         "AT+RST\r\n"
#define   CIFSR       "AT+CIFSR\r\n"
#define   CWJAP       "AT+CWJAP=\"ONENET\",\"IOT@Chinamobile\"\r\n"
#define   CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"
#define   CIPMODE_0   "AT+CIPMODE=0\r\n"
#define   CIPSEND     "AT+CIPSEND\r\n"
#define   CIPSTATUS   "AT+CIPSTATUS\r\n"

#define   MAX_SEND_BUF_LEN  1024

extern    EdpPacket* send_pkg;
extern    char send_buf[MAX_SEND_BUF_LEN];

extern  void ESP8266_Init(void);
extern  void GetSendBuf(void);
extern  void SendCmd(char* cmd, char* result, int timeOut);
extern  void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut);
extern   int ESP8266_CheckStatus(int timeOut);
extern  void ESP8266_SendDat(void);

#endif


