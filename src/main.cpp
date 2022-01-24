#include <Arduino.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
#include "main.h"
#include "spotifyapi.h"
#include "spotifyui.h"

const char *TAG = "cspot";

WiFiMulti wifiMulti;
SpotifyUI* ui = new SpotifyUI();
SpotifyApi* api = new SpotifyApi(ui, 80);


/**
 * Setup
 */
void setup()
{
    ESP_LOGI(TAG, "CSPOT starting");

    ui->Init();
    ui->SetState(UIState::WifiConnecting);

    WiFi.setHostname(CONFIG_DEVICE_NAME);
    WiFi.mode(WIFI_STA);
    for (auto i : AP_LIST)
    {
        wifiMulti.addAP(i.ssid, i.passphrase);
    }

    uint8_t count = 20;
    while (count-- && (wifiMulti.run() != WL_CONNECTED))
        delay(500);

    if (!WiFi.isConnected())
        ui->GiveUp("Unable to connect to WiFi");

    // MDNS.addService("http", "tcp", 80);

    ui->SetState(UIState::InfoScreen);
    api->RegisterPaths();
    api->Init();

    if (api->HaveRefreshToken())
    {
        ui->SetState(UIState::Ready);
        api->SetState(SptfActions::CurrentlyPlaying);
    }
    else
    {
        ui->SetState(UIState::AuthRequired);
    }
}

/**
 * Main loop
 */
void loop()
{
    api->Loop();
    
    // #ifdef WITH_APDS9960
    //     // Gesture event handler
    //     if (isr_flag == 1)
    //     {
    //         detachInterrupt(digitalPinToInterrupt(APDS9960_INT_PIN));
    //         handleGesture();
    //         isr_flag = 0;
    //         attachInterrupt(digitalPinToInterrupt(APDS9960_INT_PIN), interruptRoutine, FALLING);
    //     }
    // #endif

    // M5Stack handler
    // m5.update();
    // if (m5.BtnA.wasPressed())
    // {
    //     sptfAction = Previous;
    // }

    // if (m5.BtnB.wasPressed())
    // {
    //     sptfAction = Toggle;
    // }

    // if (m5.BtnC.wasPressed())
    // {
    //     sptfAction = Next;
    // }
}