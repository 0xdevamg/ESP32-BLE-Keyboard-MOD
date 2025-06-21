#ifndef ESP32_BLE_KEYBOARD_MOD_H
#define ESP32_BLE_KEYBOARD_MOD_H

#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

// Uncomment to use NimBLE (recomendado para mejor performance)
//#define USE_NIMBLE

#if defined(USE_NIMBLE)
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>
#define BLEHIDDevice NimBLEHIDDevice
#define BLECharacteristic NimBLECharacteristic
#define BLEServer NimBLEServer
#define BLEAdvertising NimBLEAdvertising
#else
#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#endif

#include "Print.h"
#include "esp_gap_ble_api.h"  // Necesario para tipos de seguridad

// Definiciones de estructuras faltantes
typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReportBLE;

typedef uint8_t MediaKeyReport[2];

// Constantes de teclado (se mantienen igual)
const uint8_t KEY_LEFT_CTRL = 0x80;
const uint8_t KEY_LEFT_SHIFT = 0x81;
const uint8_t KEY_LEFT_ALT = 0x82;

class BleKeyboard : public Print,
#if defined(USE_NIMBLE)
                   public NimBLEServerCallbacks,
                   public NimBLECharacteristicCallbacks
#else
                   public BLEServerCallbacks,
                   public BLECharacteristicCallbacks
#endif
{
private:
    BLEHIDDevice* hid;
    BLECharacteristic* inputKeyboard;
    BLECharacteristic* outputKeyboard;
    BLECharacteristic* inputMediaKeys;
    BLEAdvertising*    advertising;
    
    // Report structures
    KeyReportBLE   _keyReport;
    MediaKeyReport _mediaKeyReport;
    
    std::string    deviceName;
    std::string    deviceManufacturer;
    uint8_t        batteryLevel;
    bool           connected = false;
    uint32_t       _delay_ms = 7;
    void delay_ms(uint64_t ms);

    uint16_t vid     = 0x05ac;
    uint16_t pid     = 0x820a;
    uint16_t version = 0x0210;

public:
    BleKeyboard(std::string deviceName = "ESP32 Keyboard",
                std::string deviceManufacturer = "Espressif",
                uint8_t batteryLevel = 100);
    
    void begin();
    void end();
    void sendReport(KeyReportBLE* keys);
    void sendReport(MediaKeyReport* keys);
    size_t press(uint8_t k);
    size_t press(const MediaKeyReport k);
    size_t release(uint8_t k);
    size_t release(const MediaKeyReport k);
    void releaseAll(void);
    size_t write(uint8_t c);
    size_t write(const MediaKeyReport c);
    size_t write(const uint8_t *buffer, size_t size);
    bool isConnected(void);
    void setBatteryLevel(uint8_t level);
    void setName(std::string deviceName);
    void setDelay(uint32_t ms);

    void set_vendor_id(uint16_t vid);
    void set_product_id(uint16_t pid);
    void set_version(uint16_t version);
    
    // Métodos de seguridad
    void enableBonding(bool enable = true);
    void setSecurityLevel(esp_ble_auth_req_t level);
    void setIOcapabilities(esp_ble_io_cap_t iocap);
    void clearBondedDevices();

protected:
    virtual void onStarted(BLEServer *pServer) { };
    
#if defined(USE_NIMBLE)
    void onConnect(NimBLEServer* pServer) override;
    void onDisconnect(NimBLEServer* pServer) override;
    void onWrite(NimBLECharacteristic* characteristic) override;
#else
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
    void onWrite(BLECharacteristic* characteristic) override;
#endif
};

#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_KEYBOARD_MOD_H
