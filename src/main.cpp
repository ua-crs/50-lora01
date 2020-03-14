/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

/*
 *  project 50-lora01 --- main.cpp
 */

#include <Arduino.h>

#include <LoRa.h>
#include <SPI.h>


/*
 *  Constants defined in platformio.ini
 *  Valid for send and receive
 *      SERIAL_BAUD     Baud rate del port serie
 *      RFM_FREQ        LoRa frequency 433E6, 866E6 or 915E6
 *      RFM_SS          Slave Select for SPI
 *      RFM_RST         Reset for RFM95
 *      RFM_DIO0        Data Input Output of RFM95
 *      LORA_SYNC       Synchronize word between Lora send and rcv - 0 to 0xFF 
 *      SEND_DELAY      Delay between each sending
 *  Different for send and rcv
 *      LORA_SEND       1 for sending, 0 for receiving
 */

void
setup(void)
{
    // initialize Serial Monitor
    Serial.begin(SERIAL_BAUD);
    while (!Serial)
    ;
#if LORA_SEND==1
    Serial.println("LoRa sender");
#else
    Serial.println("Lora receiver");
#endif

    // setup LoRa transceiver module
    LoRa.setPins(RFM_SS, RFM_RST, RFM_DIO0);

    while (!LoRa.begin(RFM_FREQ))
    {
        Serial.println(".");
        delay(500);
    }
    Serial.println();
    LoRa.setSyncWord(LORA_SYNC);
    Serial.println("LoRa Initializing OK!");
}

#if LORA_SEND==1

static void
lora_loop(void)
{
    static int counter = 0;

    Serial.print("Sending packet: ");
    Serial.println(counter);

    // Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.endPacket();

    counter++;

    delay(SEND_DELAY);
}

#else

static void
lora_loop(void)
{
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        // received a packet
        Serial.print("Received packet '");

        // read packet
        while (LoRa.available())
        {
            String LoRaData = LoRa.readString();
            Serial.print(LoRaData);
        }

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }
}

#endif


void
loop(void)
{
    lora_loop();
}
