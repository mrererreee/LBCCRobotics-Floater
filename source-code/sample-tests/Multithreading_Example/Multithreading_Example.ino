/*
* This is a sample sketch for multithreading. Tasks ("sssspecial" Methods) are set up and assigned to specific cores in the ESP32P4.
* Many tasks can be created and ran on the same core, but proper synthax must be provided to allow concurrency. 
* What is concurrency? In computers, it allows to run multiple programs on the same core by having a single CPU core process tasks back-n-forward between two or more different tasks.
* Concurrency vs Parallelism: Two or more task can run in mutiple ways; One core multiple taks and Two cores with an individual(or more) task.
* THIS PROGRAM IS RUNNING IN PARALLEL, WE HAVE TWO CORES DOING TWO DIFFERENT TASK we created: communication taks, communicationTask() & application task, appTask().
* IMPORTANT NOTE!!!!!!!!!!!: The cores are running other (important/automatic) tasks which the system assigns automatically. BUT...
* we have assigned our taks to run in parallet in specific cores (SEE BELOW).
*/ 

#include "USB.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

QueueHandle_t airSigRxQueue; //Queue for messages/commands from PC to be processed in ESP32P4. 
QueueHandle_t airSigTxQueue; //Queue for messages to be transfered over to PC over WiFi or BLE(BlueTooth).

typedef struct {
  char message[64];// Array of characters to be put in Queue, 64 byte long.
} SignalMessage;

//TASKS_START-------------------------------------------------------------------------------------------------------------------------------
void communicationTask(void *parameter){
  SignalMessage msg;

  while (true) {
    //TODO: Logic to connect using WiFi/BLE | Communication module code needed. | CURRENT sample is for SERIAL CONNECTION.
    if(Serial.available()){
      String input = Serial.readStringUntil('\n');

      input.toCharArray(msg.message, sizeof(msg.message)); //converts input to array and stores in msg.message structure.

      xQueueSend(airSigRxQueue, &msg, portMAX_DELAY); // add message to Queue(airSigRxQueue), message passed by reference(&), release core until next task.
    }

    if(xQueueReceive(airSigTxQueue, &msg, 0)){
      Serial.println(msg.message);
    }

    vTaskDelay(pdMS_TO_TICKS(10)); //Release cores for a bit, BUT can increase number to release core for a longer period of time. IF increase is major (i.e 10000) you will see noticeable delays.
  }
}

void appTask(void *parameter) {
  SignalMessage rx, tx;

  delay(10);
  Serial.println("SET UP COMPLETE...");

  while(true){
    if (xQueueReceive(airSigRxQueue, &rx, 0)){
      if (strcmp(rx.message, "COMMAND") == 0) //rx.message comparison to expected command | switch-case block will replace this IF statement.
      {
        strcpy(tx.message, "DATA COLLECTED HERE"); //tx.message container filled with string.

        xQueueSend(airSigTxQueue, &tx, portMAX_DELAY); //add message to Queue(airSigTxQueue), message passed by reference(&), release until next task. | Releases core, might need to change depending on tasks.
      }
      else {
        strcpy(tx.message, "COMMAND NOT AVAILABLE OR COMMAND DOES NOT EXIST");

        xQueueSend(airSigTxQueue, &tx, portMAX_DELAY);
      }
    }
  }
}
//TASKS_END---------------------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  airSigRxQueue = xQueueCreate(10, sizeof(SignalMessage));
  airSigTxQueue = xQueueCreate(10, sizeof(SignalMessage));

  xTaskCreatePinnedToCore(communicationTask, // Task to be assigned to core. 
                          "AirSignal", //Name of task.
                          4096, //RAM Size of task.
                          NULL, 
                          3, //Priority | Matters when dealing with other tasks at the same time.
                          NULL, 
                          0); //Core 0 | This task is asigned to core 0.

  xTaskCreatePinnedToCore(appTask, "Application", 4096, NULL, 2, NULL, 1);

}

void loop(){}
