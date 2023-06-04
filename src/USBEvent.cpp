#include "Include.h"

void usbEventCallback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
  arduino_usb_cdc_event_data_t * data = (arduino_usb_cdc_event_data_t*)event_data;
  if(event_id == ARDUINO_USB_CDC_RX_EVENT){
    //Serial.printf("CDC RX [%u]:", data->rx.len);
    uint8_t buf[data->rx.len];
    size_t len = USBSerial.read(buf, data->rx.len);
    SerialData += String(buf,len);
  }else if(event_id == ARDUINO_USB_CDC_RX_OVERFLOW_EVENT){
    Serial.printf("CDC RX Overflow of %d bytes", data->rx_overflow.dropped_bytes);
  }
}