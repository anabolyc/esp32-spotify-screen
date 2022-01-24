#ifndef M5SPOT_MAIN_H
#define M5SPOT_MAIN_H

typedef struct {
    const char *ssid;
    const char *passphrase;
} APlist_t;

/*
 * WiFi access points list
 */
const APlist_t AP_LIST[] = {
      {CONFIG_WIFI_SSID, CONFIG_WIFI_PASS}
};

#endif // M5SPOT_MAIN_H
