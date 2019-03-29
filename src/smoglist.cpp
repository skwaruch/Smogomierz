#include "Arduino.h"
#include "smoglist.h"


//#define DEBUG_PRINT // comment this line to disable debug print

#ifndef DEBUG_PRINT
#define DEBUG_PRINT(a)
#else
#define DEBUG_PRINT(a) (Serial.println(String(F("[Debug]: "))+(a)))
#define _DEBUG
#endif

const char* host = "51.83.73.195";
uint16_t port = 8086;

String smoglistdb = "smoglistDB";
String smoglistuser = "smoglist";
String smoglistpassword = "4A8R=A9A=5y@dUpN";

Smoglist::Smoglist() {
        _port = String(port);
        _host = String(host);
}

SmoglistDB_RESPONSE Smoglist::opendb() {
        _db = smoglistdb + "&u=" + smoglistuser + "&p=" + smoglistpassword;
}

SmoglistDB_RESPONSE Smoglist::write(SmoglistdbMeasurement data) {
        return write(data.postString());
}

SmoglistDB_RESPONSE Smoglist::write(String data) {

        HTTPClient http;

        DEBUG_PRINT("HTTP post begin...");

        http.begin("http://" + _host + ":" + _port + "/write?db=" + _db); //HTTP
        http.addHeader("Content-Type", "text/plain");

        int httpResponseCode = http.POST(data);

        if (httpResponseCode == 204) {
                _response = SmoglistDB_SUCCESS;
                String response = http.getString();    //Get the response to the request
                DEBUG_PRINT(String(httpResponseCode)); //Print return code
                DEBUG_PRINT(response);                 //Print request answer

        } else {
                DEBUG_PRINT("Error on sending POST:");
                DEBUG_PRINT(String(httpResponseCode));
                _response=SmoglistDB_ERROR;
        }

        http.end();
        return _response;
}

SmoglistDB_RESPONSE Smoglist::query(String sql) {

        String url = "/query?";
        url += "pretty=true&";
        url += "db=" + _db;
        url += "&q=" + SmoglistURLEncode(sql);
        DEBUG_PRINT("Requesting URL: ");
        DEBUG_PRINT(url);

        HTTPClient http;

        http.begin("http://" + _host + ":" + _port + url); //HTTP

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode == 200) {
                // HTTP header has been send and Server response header has been handled
                _response = SmoglistDB_SUCCESS;
                String reply = http.getString();
                Serial.println(reply);

        } else {
                _response = SmoglistDB_ERROR;
                DEBUG_PRINT("[HTTP] GET... failed, error: " + httpCode);
        }

        http.end();
        return _response;
}

SmoglistDB_RESPONSE Smoglist::response() {
        return _response;
}

/* -----------------------------------------------*/
//        Field object
/* -----------------------------------------------*/
SmoglistdbMeasurement::SmoglistdbMeasurement(String m) {
        measurement = m;
}

void SmoglistdbMeasurement::empty() {
        _data = "";
        _tag = "";
}

void SmoglistdbMeasurement::addTag(String key, String value) {
        _tag += "," + key + "=" + value;
}

void SmoglistdbMeasurement::addField(String key, float value) {
        _data = (_data == "") ? (" ") : (_data += ",");
        _data += key + "=" + String(value);
}

String SmoglistdbMeasurement::postString() {
        //  uint32_t utc = 1448114561 + millis() /1000;
        return measurement + _tag + _data;
}

// URL Encode with Arduino String object
String SmoglistURLEncode(String msg) {
        const char *hex = "0123456789abcdef";
        String encodedMsg = "";

        uint16_t i;
        for (i = 0; i < msg.length(); i++) {
                if (('a' <= msg.charAt(i) && msg.charAt(i) <= 'z') ||
                    ('A' <= msg.charAt(i) && msg.charAt(i) <= 'Z') ||
                    ('0' <= msg.charAt(i) && msg.charAt(i) <= '9')) {
                        encodedMsg += msg.charAt(i);
                } else {
                        encodedMsg += '%';
                        encodedMsg += hex[msg.charAt(i) >> 4];
                        encodedMsg += hex[msg.charAt(i) & 15];
                }
        }
        return encodedMsg;
}
