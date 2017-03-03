#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <time.h>

#include "wifi_secrets.h"
#define JST (3600 * 9)

const char *weekDays[7] = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };

void setup()
{
    //configure Serial for log
    Serial.begin(115200);
    delay(1000);
    Serial.print("\n\nRESET\n\n");

    //configure WiFi
    WiFi.mode(WIFI_STA);
    WiFi.printDiag(Serial);
    WiFi.begin(WiFi_ssid, WiFi_wpaKey);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }
    WiFi.printDiag(Serial);
    Serial.println();
    Serial.printf("Connected, IPADDR = ");
    Serial.println(WiFi.localIP());

    //sync NTP
    configTime(JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
}

void loop()
{
    time_t t;
    struct tm *tm;
    t = time(NULL);
    tm = localtime(&t);
    Serial.printf("%04d-%02d-%02d(%s) %02d:%02d:%02d\n",
                    tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
                    weekDays[tm->tm_wday],
                    tm->tm_hour, tm->tm_min, tm->tm_sec);
    delay(500);
}
