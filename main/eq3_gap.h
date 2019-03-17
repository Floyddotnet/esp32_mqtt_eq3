
#ifndef EQ3_GAP_H
#define EQ3_GAP_H

/* Address types */
#define BLE_ADDR_PUBLIC         0x00
#define BLE_ADDR_RANDOM         0x01
#define BLE_ADDR_PUBLIC_ID      0x02
#define BLE_ADDR_TYPE_MASK      (BLE_ADDR_RANDOM | BLE_ADDR_PUBLIC)

/* Device list handling */
struct found_device {
  //esp_bd_addr_t bda;
  char bda[6]; /* Should really make this consistent with esp_bd_addr_t */
  int rssi;
  struct found_device *next;
};

enum eq3_scanstate { EQ3_NO_SCAN_RESULTS = 0, EQ3_SCAN_UNDERWAY, EQ3_SCAN_COMPLETE };

enum eq3_scanstate eq3gap_get_device_list(struct found_device **devlist, int *numdevs);

void start_scan(void);

bool scan_complete(void);

#endif
