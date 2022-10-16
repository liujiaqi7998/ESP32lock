#include "webserver.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);

void int_web()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hi! This is a EN Door Web Page. V5.0.6"); });

    
    AsyncElegantOTA.begin(&server); // Start AsyncElegantOTA
    server.begin();
}