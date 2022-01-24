#include "spotifyui.h"

#include <SPI.h>
#include <SPIFFS.h>
#include <WiFi.h>

SpotifyUI::SpotifyUI()
{
#ifdef TFT_ENABLED
    this->tft = new TFT_eSPI();
#endif
}

void SpotifyUI::Init()
{
#ifdef TFT_ENABLED
    tft->init();
    tft->setRotation(TFT_ROTATION);
    tft->fillScreen(TFT_BLACK);
    tft->setTextColor(TFT_SPT_GREEN);
#endif

    if (!SPIFFS.begin())
    {
        GiveUp("Unable to begin SPIFFS");
    }

    ESP_LOGI(TAG, "initialized");
}

void SpotifyUI::SetState(UIState newState)
{
    this->state = newState;
    switch (this->state)
    {
    case UIState::WifiConnecting:
        //-----------------------------------------------
        // Initialize Wifi
        //-----------------------------------------------
#ifdef TFT_ENABLED
        tft->drawPngFile(SPIFFS, "/logo128.png", (tft->width() - 128) >> 1, (tft->height() - 128) >> 1);

        tft->setFreeFont(&FreeSansBoldOblique12pt7b);
        tft->setTextSize(1);
        tft->setTextDatum(TC_DATUM);
        tft->drawString(CONFIG_DEVICE_NAME, 160, 10);

        tft->setFreeFont(&FreeSans9pt7b);
        tft->setTextSize(1);
        tft->setTextDatum(BC_DATUM);
        tft->drawString("Connecting to WiFi...", 160, 215);
#endif
        ESP_LOGI(TAG, "Connecting to WiFi...");
        break;

    case InfoScreen:
        //-----------------------------------------------
        // Display some infos
        //-----------------------------------------------
#ifdef TFT_ENABLED
        tft->fillScreen(TFT_BLACK);
        tft->drawPngFile(SPIFFS, "/logo128d.png", (tft->width() - 128) >> 1, (tft->height() - 128) >> 1);

        tft->setFreeFont(&FreeSansBoldOblique12pt7b);
        tft->setTextColor(TFT_SPT_GREEN);
        tft->setTextSize(1);
        tft->setTextDatum(TC_DATUM);
        tft->drawString(CONFIG_DEVICE_NAME, 160, 10);

        tft->setFreeFont(&FreeMono9pt7b);
        tft->setTextColor(TFT_WHITE);
        tft->setTextSize(1);
        tft->setCursor(0, 75);
        tft->printf(" SSID:      %s\n", WiFi.SSID().c_str());
        tft->printf(" IP:        %s\n", WiFi.localIP().toString().c_str());
        tft->printf(" STA MAC:   %s\n", WiFi.macAddress().c_str());
        tft->printf(" AP MAC:    %s\n", WiFi.softAPmacAddress().c_str());
        tft->printf(" Chip size: %s\n", prettyBytes(ESP.getFlashChipSize()).c_str());
        tft->printf(" Free heap: %s\n", prettyBytes(ESP.getFreeHeap()).c_str());

        tft->setFreeFont(&FreeSans9pt7b);
        tft->setTextColor(TFT_SPT_GREEN);
        tft->setTextSize(1);
        tft->setTextDatum(BC_DATUM);
        // tft->drawString("Press any button to continue...", 160, 230);
#endif
        ESP_LOGI(TAG, " SSID:      %s", WiFi.SSID().c_str());
        ESP_LOGI(TAG, " IP:        %s", WiFi.localIP().toString().c_str());
        ESP_LOGI(TAG, " STA MAC:   %s", WiFi.macAddress().c_str());
        ESP_LOGI(TAG, " AP MAC:    %s", WiFi.softAPmacAddress().c_str());
        ESP_LOGI(TAG, " Chip size: %s", prettyBytes(ESP.getFlashChipSize()).c_str());
        ESP_LOGI(TAG, " Free heap: %s", prettyBytes(ESP.getFreeHeap()).c_str());

        break;

    case SetupFinished:
        //-----------------------------------------------
        // End of setup
        //-----------------------------------------------
#ifdef TFT_ENABLED
        tft->fillScreen(TFT_BLACK);
        tft->drawPngFile(SPIFFS, "/logo128.png", (tft->width() - 128) >> 1, (tft->height() - 128) >> 1);

        tft->setFreeFont(&FreeSansBoldOblique12pt7b);
        tft->setTextSize(1);
        tft->setTextDatum(TC_DATUM);
        tft->drawString(CONFIG_DEVICE_NAME, 160, 10);
#endif
        break;

    case Ready:
#ifdef TFT_ENABLED
        tft->setTextDatum(BC_DATUM);
        tft->setFreeFont(&FreeSans9pt7b);
        tft->drawString("Ready...", 160, 230);
#endif
        ESP_LOGI(TAG, "Ready");
        break;

    case AuthRequired:
#ifdef TFT_ENABLED
        tft->setTextDatum(BC_DATUM);
        tft->setFreeFont(&FreeSans9pt7b);
        tft->drawString("Point your browser to", 160, 205);

        tft->setFreeFont(&FreeSans12pt7b);
        tft->drawString("http://" CONFIG_DEVICE_NAME ".lan", 160, 235);
#endif
        ESP_LOGW(TAG, "Point your browser to: %s", "http://" CONFIG_DEVICE_NAME ".lan");
        break;
    }
}

/**
 * Display error message and stop execution
 *
 * @param errMsg
 */
void SpotifyUI::GiveUp(const char *errMsg)
{
    ///ESP_LOGE(TAG, errMsg);

#ifdef TFT_ENABLED
    tft->setFreeFont(&FreeSans12pt7b);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(1);
    tft->setTextDatum(CC_DATUM);
    tft->fillScreen(TFT_RED);
    tft->drawString(errMsg, tft->width() >> 1, tft->height() >> 1);
#endif

    while (true)
    {
        yield();
    }
}

/**
 * Display bytes in a pretty format
 *
 * @param Bytes value
 * @return Bytes value prettified
 */
String SpotifyUI::prettyBytes(uint32_t bytes)
{

    const char *suffixes[7] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
    uint8_t s = 0;
    double count = bytes;

    while (count >= 1024 && s < 7)
    {
        s++;
        count /= 1024;
    }
    if (count - floor(count) == 0.0)
    {
        return String((int)count) + suffixes[s];
    }
    else
    {
        return String(round(count * 10.0) / 10.0, 1) + suffixes[s];
    };
}

void SpotifyUI::SetSongName(String name)
{
// Display song name
#ifdef TFT_ENABLED
    tft->fillRect(0, 0, 320, 30, 0xffffff);
    tft->setTextColor(TFT_BLACK);
    tft->setTextFont(2);
    tft->setTextSize(1);
    tft->setTextDatum(TC_DATUM);
    tft->drawString(name, 160, 2);
#endif

    ESP_LOGI(TAG, "Song name: %s", name);
}

void SpotifyUI::SetArtistName(String name)
{
#ifdef TFT_ENABLED
    // Display artists names
    tft->setTextFont(1);
    tft->setTextSize(1);
    tft->setTextDatum(BC_DATUM);
    tft->drawString(name, 160, 28);
#endif

    ESP_LOGI(TAG, "Artist name: %s", name);
}

void SpotifyUI::SetProgress(uint32_t progress, uint32_t duration)
{
#ifdef TFT_ENABLED
    // Display progress bar background
    tft->fillRect(0, 235, 320, 5, TFT_WHITE); // TODO: only when changed
    tft->fillRect(0, 235, ceil((float)320 * ((float)progress / duration)), 5, TFT_SPT_GREEN);
#endif

    ESP_LOGI(TAG, "Progress: %d / %d", progress, duration);
}

void SpotifyUI::SetAlbumArt(const char *filename)
{
#ifdef TFT_ENABLED
    // Display album art
    tft->fillScreen(TFT_WHITE);
    tft->drawJpgFile(SPIFFS, filename, 10, 30);
#endif

    ESP_LOGI(TAG, "Album art: %s", filename);
}