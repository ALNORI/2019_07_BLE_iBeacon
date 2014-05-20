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
#include "nRF51822n.h"

nRF51822n   nrf;                /* BLE radio driver */

DigitalOut  led1(LED1);
DigitalOut  led2(LED2);
Ticker      flipper;
Serial      pc(USBTX,USBRX);

void tickerCallback(void);

/**************************************************************************/
/*!
    @brief  Program entry point
*/
/**************************************************************************/
int main(void)
{
    /* Setup blinky: led1 is toggled in main, led2 is toggled via Ticker */
    led1 = 1;
    led2 = 1;
    flipper.attach(&tickerCallback, 1.0);

    /* Initialise the nRF51822 */
    pc.printf("Initialising the nRF51822\n\r");
    nrf.init();

    GapAdvertisingParams advParams (
        GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED );
    GapAdvertisingData   advData;
    GapAdvertisingData   scanResponse;

    /*
     * For this demo application, populate the beacon advertisement payload
     * with 2 AD structures: FLAG and MSD
     *
     * Reference:
     *  Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 11, 18
     */

    /* Define an Beacon payload.

       This is the data part of the MSD AdvertisingData structure to be added to
       the advertising payload.
       --------------------------------------------------------------
       128-Bit UUID = E2 0A 39 F4 73 F5 4B C4 A1 2F 17 D1 AD 07 A9 61
       Major/Minor  = 0000 / 0000
       Tx Power     = C8
    */
    uint8_t beaconPayload[] = {
        0x4C, 0x00, // Company identifier code (0x004C == Apple)
        0x02,       // ID
        0x15,       // length of the remaining payload
        0xE2, 0x0A, 0x39, 0xF4, 0x73, 0xF5, 0x4B, 0xC4, // UUID
        0xA1, 0x2F, 0x17, 0xD1, 0xAD, 0x07, 0xA9, 0x61,
        0x00, 0x00, // the major value to differenciate a location
        0x00, 0x00, // the minor value to differenciate a location
        0xC8        // 2's complement of the Tx power (-56dB)
    };

    /* Make sure we get a clean start */
    nrf.reset();

    advData.addFlags(GapAdvertisingData::BREDR_NOT_SUPPORTED);
    advData.addData(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA,
                    beaconPayload,
                    sizeof(beaconPayload));

    /* Start advertising! */
    nrf.getGap().setAdvertisingData(advData, scanResponse);
    nrf.getGap().startAdvertising(advParams);

    /* Do blinky on LED1 while we're waiting for BLE events */
    for (;; ) {
        led1 = !led1;
        wait(1);
    }
}

/**************************************************************************/
/*!
    @brief  Ticker callback to switch led2 state
*/
/**************************************************************************/
void tickerCallback(void)
{
    led2 = !led2;
}
