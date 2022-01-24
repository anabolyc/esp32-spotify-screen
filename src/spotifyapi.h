#ifndef SPOTIFY_API_H
#define SPOTIFY_API_H

#include "spotifyui.h"
#include <WString.h>
#include <ESPAsyncWebServer.h>

#define min(X, Y) (((X)<(Y))?(X):(Y))
#define startsWith(STR, SEARCH) (strncmp(STR, SEARCH, strlen(SEARCH)) == 0)

enum EventsLogTypes
{
    log_line,
    log_raw
};

enum GrantTypes
{
    gt_authorization_code,
    gt_refresh_token
};

enum SptfActions
{
    Idle,
    GetToken,
    TokenRequestFailed,
    CurrentlyPlaying,
    Next,
    Previous,
    Toggle
};

typedef struct
{
    int httpCode;
    String payload;
} HTTP_response_t;

/*
 * Spotify settings
 *
 * Don't forget to declare http://m5spot.local/callback/
 * as the Redirect URI in your Spotify App settings
 * See https://developer.spotify.com/dashboard/applications
 */
const char SPTF_CLIENT_ID[] = CONFIG_SPTF_CLIENT_ID;
const char SPTF_CLIENT_SECRET[] = CONFIG_SPTF_CLIENT_SECRET;
const uint16_t SPTF_POLLING_DELAY = 5000;

class SpotifyApi
{
public:
    SpotifyApi(SpotifyUI* ui, uint8_t port);
    // webserver part
    void RegisterPaths();

    void eventsSendLog(const char *logData, EventsLogTypes type = log_line);
    void eventsSendInfo(const char *msg, const char *payload = "");
    void eventsSendError(int code, const char *msg, const char *payload = "");
    
    // API part
    void Init();
    bool HaveRefreshToken();
    void SetState(SptfActions);
    void Loop();

    void Previous();
    void Next();
    void Toggle();
    void CurrentlyPlaying();
    void DisplayAlbumArt(String);

private:
    const char *TAG = "api";
    bool send_events = true;

    SpotifyUI* ui;
    SpotifyApi *api;
    AsyncWebServer *server;
    AsyncEventSource *events;

    char auth_state[17];
    String auth_state_returned;
    String auth_code;
    String access_token;
    String refresh_token;

    uint32_t token_lifetime_ms = 0;
    uint32_t token_millis = 0;
    uint32_t last_curplay_millis = 0;
    uint32_t next_curplay_millis = 0;

    bool getting_token = false;
    bool ota_in_progress = false;
    bool sptf_is_playing = true;

    SptfActions State = Idle;

    void GetToken(const String &code, GrantTypes grant_type = GrantTypes::gt_refresh_token);
    HTTP_response_t ApiRequest(const char *method, const char *endpoint, const char *content = "");

    void deleteRefreshToken();
    String readRefreshToken();
    void writeRefreshToken();
    HTTP_response_t httpRequest(const char *host, uint16_t port, const char *headers, const char *content = "");
};

#endif