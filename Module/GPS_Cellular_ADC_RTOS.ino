#include <Wire.h>

#define SDA 1
#define SCL 2
#define RX 3
#define TX 4

void GPSModuleI2C(void *parameters) // akses data GPS
{ 
    Wire.begin(SDA,SCL)
    for (;;)
    {
        // send something through I2C based on GPS module datasheet
        Wire.beginTransmission(99); // 99(device address check from datasheet)
        Wire.write(223);            // 223(command or other message needed from datasheet)
        Wire.endTransmission();

        // read something from the module
        Wire.requestFrom(99, 11);   // 99(device address check from datasheet), 11(number of character requested)
        Serial.print("GPS I2C read:");
        while (Wire.available()):
            Serial.print(Wire.read());
        Serial.println();
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void CellularModuleUART(void *parameters)
{
    SoftwareSerial myuart(RX, TX);
    myuart.begin(9600);     // baudrate check on datasheet
    for (;;)
    {
        // send something through UART based on Cellular module datasheet
        myuart.write(223)   // 223(command or other message needed from datasheet)

        // read something from the module
        Serial.print("Cellular UART read: ");
        while (myuart.available()):
            Serial.print(myuart.read());
        Serial.println();
        vTaskDelay(50/portTICK_PERIOD_MS);
    }
}

void ADC(void *parameters)
{
    for (;;)
    {
        int val = analogRead(5);  //for example adc in pinout 5
        Serial.print("ADC read: ");
        Serial.println(val);
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);

    xTaskCreate(
        GPSModuleI2C
        ,  "GPSModuleI2C"
        ,  1024             // stack size
        ,  NULL             // parameters
        ,  2                // priority
        ,  NULL             // task handle 
);

    xTaskCreate(
        CellularModuleUART
        ,  "CellularModuleUART"
        ,  1024
        ,  NULL
        ,  2
        ,  NULL 
        );

    xTaskCreate(
        ADC
        ,  "ADC"
        ,  1024
        ,  NULL
        ,  2
        ,  NULL
        );
}

void loop()
{
    // empty
}