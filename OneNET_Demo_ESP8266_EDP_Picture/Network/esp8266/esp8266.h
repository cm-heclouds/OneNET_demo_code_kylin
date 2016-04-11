#define AT          "AT\r\n"	
#define CWMODE      "AT+CWMODE=3\r\n"		//STA+AP模式
#define RST         "AT+RST\r\n"
#define CIFSR       "AT+CIFSR\r\n"
#define CWJAP       "AT+CWJAP=\"onenet\",\"\"\r\n"	//ssid: onenet 密码：空
#define CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"	//EDP服务器 183.230.40.39/876
#define CIPMODE0    "AT+CIPMODE=0\r\n"		//非透传模式
#define CIPMODE1    "AT+CIPMODE=1\r\n"		//透传模式
#define CIPSEND     "AT+CIPSEND\r\n"
#define CIPSTATUS   "AT+CIPSTATUS\r\n"		//网络状态查询


void ESP8266_Init(void);