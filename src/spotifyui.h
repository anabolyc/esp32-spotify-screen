#ifndef SPOTIFY_UI_H
#define SPOTIFY_UI_H

#ifdef TFT_ENABLED
#include "TFT_eSPI.h"
#define TFT_SPT_GREEN TFT_GREEN ///tft->color565(30, 215, 96);
#endif

enum UIState
{
    WifiConnecting,
    InfoScreen,
    SetupFinished,
    Ready,
    AuthRequired
};

class SpotifyUI
{
public:
    SpotifyUI();
    void Init();
    void SetState(UIState);
    void GiveUp(const char *);

    void SetSongName(String);
    void SetArtistName(String);
    void SetProgress(uint32_t, uint32_t);
    void SetAlbumArt(const char *);

private:
    const char *TAG = "ui";
    
    UIState state;
    TFT_eSPI *tft;

    String prettyBytes(uint32_t bytes);
};

#endif