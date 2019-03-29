/* 
	Smoglist library
   	Written by Blazej Faliszek
   	Based on ESPinfluxdb.h" - https://github.com/hwwong/ESP_influxdb by HW Wong
 */

#ifndef SMOGLISTDB_H
#define SMOGLISTDB_H
#include "Arduino.h"

#if defined(ESP8266)
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <HTTPClient.h>
#endif

enum SmoglistDB_RESPONSE {SmoglistDB_SUCCESS, SmoglistDB_ERROR, SmoglistDB_CONNECT_FAILED};

// Url encode function
String SmoglistURLEncode(String msg);

class SmoglistdbMeasurement
{
public:
SmoglistdbMeasurement(String m);

String measurement;

void addField(String key, float value);
void addTag(String key, String value);
void empty();
String postString();

private:
String _data;
String _tag;
};

class Smoglist
{
public:
Smoglist();

SmoglistDB_RESPONSE opendb();
SmoglistDB_RESPONSE write(SmoglistdbMeasurement data);
SmoglistDB_RESPONSE write(String data);
SmoglistDB_RESPONSE query(String sql);
//uint8_t createDatabase(char *dbname);
SmoglistDB_RESPONSE response();

private:
String _port;
String _host;
String _db;

SmoglistDB_RESPONSE _response = SmoglistDB_ERROR;

};

#endif
