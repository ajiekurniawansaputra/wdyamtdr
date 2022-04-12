#include <WiFi.h>
#include <ArduinoMqttClient.h>

#define ssid "mywifissid"
#define pass "mywifipass"
#define conmqtt false       //flags mqtt connection

WiFiClient client;
MqttClient mqttClient(client);

void wifi(void *parameters)
{
    for (;;)
    {
        if (WiFi.status()==WL_CONNECTED){
            vTaskDelay(10000/portTICK_PERIOD_MS);    
            mqttClient.poll()
        }
        else {
            WiFi.begin(ssid,pass);
            unsigned long mystart = millis();
            unsigned long myend = millis();
            while (WiFi.status()!=WL_CONNECTED && (myend-mystart)<10000){
                myend = millis();
            }
            if (WiFi.status()!=WL_CONNECTED){
                vTaskDelay(5000/portTICK_PERIOD_MS);
            }
        }        
    }
}

void mymqttcon(void *parameters)
{
    for (;;)
    {
        if (WiFi.status()==WL_CONNECTED && conmqtt==true){
            vTaskDelay(10000/portTICK_PERIOD_MS);    
        }
        else {
            if (!mqttClient.connect(broker, port)) {
                conmqtt = false;
            }
            else {
                conmqtt = true;
            }
        }        
    }
}

uint8_t readVoltage(){
    return random(5, 12);
}
uint8_t readTemperature(){
    return random(30, 50);
}
uint8_t readSomeSensor(){
    return random(0, 255);
}

void teganganSumberDaya(void *parameters)
{
    for (;;)
    {        
        uint8_t voltage = readVoltage();
        mqttClient.beginMessage(topic);
        mqttClient.print(voltage);
        mqttClient.endMessage();
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}

void InternalSuhu(void *parameters)
{
    for (;;)
    {        
        uint8_t temperature = readTemperature();
        mqttClient.beginMessage(topic);
        mqttClient.print(temperature);
        mqttClient.endMessage();
        vTaskDelay(35/portTICK_PERIOD_MS);
    }
}

void satuSensor(void *parameters)
{
    for (;;)
    {        
        uint8_t value = readSomeSensor();
        mqttClient.beginMessage(topic);
        mqttClient.print(value);
        mqttClient.endMessage();
        vTaskDelay(45/portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    if (!mqttClient.connect(broker, port)) {
        conmqtt = false;
    }
    else {
        conmqtt = true;
    }

    xTaskCreatePinnedToCore(
        wifi
        , "wifi"
        , 1024             // stack size
        , NULL             // parameters
        , 2                // priority
        , NULL             // task handle
        , CONFIG_ARDUINO_RUNNING_CORE 
        );

    xTaskCreate(
        mymqttcon
        ,  "mymqttcon"
        ,  1024
        ,  NULL
        ,  2
        ,  NULL
        );

    xTaskCreate(
        teganganSumberDaya
        ,  "teganganSumberDaya"
        ,  1024
        ,  NULL
        ,  2
        ,  NULL 
        );

    xTaskCreate(
        InternalSuhu
        ,  "InternalSuhu"
        ,  1024
        ,  NULL
        ,  2
        ,  NULL
        );

    xTaskCreate(
        satuSensor
        ,  "satuSensor"
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