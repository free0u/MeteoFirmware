/*
  MHZ19_uart.h - MH-Z19 CO2 sensor library for ESP-WROOM-02/32(ESP8266/ESP32) or Arduino
  version 0.3

  License MIT
*/

#ifndef MHZ19_uart_h_
#define MHZ19_uart_h_

#include "Arduino.h"
#include "SoftwareSerial.h"

enum MHZ19_DATA { PPM, TEMPERATURE, STAT };

class MHZ19_uart {
  public:
    MHZ19_uart();
    MHZ19_uart(int rx, int tx);
    virtual ~MHZ19_uart();

    void begin(int rx = -1, int tx = -1);
    void setAutoCalibration(boolean autocalib);
    void calibrateZero();
    void calibrateSpan(int ppm);
    void setRange(bool isRange5);
    int getPPM();
    int getTemperature();
    int getStatus();

    boolean isWarming();

  protected:
    void writeCommand(uint8_t com[]);
    void writeCommand(uint8_t com[], uint8_t response[]);

  private:
    SoftwareSerial *hserial;

    uint8_t mhz19_checksum(uint8_t com[]);
    int getSerialData(MHZ19_DATA flg);

    static const int REQUEST_CNT = 8;
    static const int RESPONSE_CNT = 9;

    // serial command
    uint8_t getppm[REQUEST_CNT] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t zerocalib[REQUEST_CNT] = {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t spancalib[REQUEST_CNT] = {0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t autocalib_on[REQUEST_CNT] = {0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00};
    uint8_t autocalib_off[REQUEST_CNT] = {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};

    uint8_t range2[REQUEST_CNT] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x07, 0xD0};
    uint8_t range5[REQUEST_CNT] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88};

    int _rx_pin = -1;
    int _tx_pin = -1;
};

#endif
