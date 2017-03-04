#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <time.h>

#include <Wire.h>
#include <SSD1306.h>

#include "wifi_secrets.h"
#define JST (3600 * 9)

const char *weekDays[7] = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };
char ipaddrBuffer[15 + 1];
char dateBuffer[15 + 1];
char clockBuffer[8 + 1];

SSD1306 display(0x3c, 4, 5); //addr, SDA, SCK

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

    byte oct0 = WiFi.localIP()[0];
    byte oct1 = WiFi.localIP()[1];
    byte oct2 = WiFi.localIP()[2];
    byte oct3 = WiFi.localIP()[3];
    sprintf(ipaddrBuffer, "IPADDR:%d.%d.%d.%d", oct0, oct1, oct2, oct3);

    //sync NTP
    configTime(JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");

    //configure SSD1306 i2c display
    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
    display.displayOn();
}

void loop()
{
    time_t t;
    struct tm *tm;
    t = time(NULL);
    tm = localtime(&t);
    sprintf(dateBuffer, "%04d-%02d-%02d(%s)",
                    tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
                    weekDays[tm->tm_wday]);
    sprintf(clockBuffer, "%02d:%02d:%02d",
                    tm->tm_hour, tm->tm_min, tm->tm_sec);

    Serial.printf("%s %s\n", dateBuffer, clockBuffer);

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 0, String(ipaddrBuffer));
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 16, String(dateBuffer));
    display.setFont(ArialMT_Plain_24);
    display.drawString(64, 32 + 8, String(clockBuffer));
    display.display();
    delay(250);
}
