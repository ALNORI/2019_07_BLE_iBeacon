/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "BLEDevice.h"

BLEDevice ble;

/*
 * For this demo application, populate the beacon advertisement payload
 * with 2 AD structures: FLAG and MSD (manufacturer specific data).
 *
 * Reference:
 *  Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 11, 18
 */

/*
 * The Beacon payload (encapsulated within the MSD advertising data structure)
 * has the following composition:
 * 128-Bit UUID = 01 12 23 34 45 56 67 78 89 9A AB BC CD DE EF F0
 * Major/Minor  = 0000 / 0000
 * Tx Power     = C8
 */
const uint8_t beaconPayload[] = {
    0x4C, 0x00,
    0x02,
    0x15,
    0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
    0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0,
    0x00, 0x00,
    0x00, 0x00,
    0xC8
};

int main(void)
{
    ble.init();

    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, beaconPayload, sizeof(beaconPayload));

    ble.setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(Gap::MSEC_TO_ADVERTISEMENT_DURATION_UNITS(1000));
    ble.startAdvertising();

    for (;;) {
        ble.waitForEvent();
    }
}
