#pragma once

typedef enum WiFiMode 
{
    WIFI_OFF = 0,
    WIFI_STA = 1,
    WIFI_AP = 2,
    WIFI_AP_STA = 3,
} WiFiMode_t;

struct WiFiState
{
    uint32_t crc;
    struct
    {
        // station_config fwconfig;
        // ip_info ip;
        // ip_addr_t dns[2];
        // ip_addr_t ntp[2];
        // WiFiMode_t mode;
        uint8_t channel;
        bool persistent;
    } state;
};
