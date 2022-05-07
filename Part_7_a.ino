/**
* FreeRTOS Race Condition Demo
*
* Increment a shared global variable.
*
* Date: January 20, 2021
* Author: Shawn Hymel
* License: 0BSD
*/
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
// Globals
static int shared_var = 0;
//*****************************************************************************
// Tasks
void task_A(void *parameters){
  while(1){
    vTaskDelay(1000/ portTICK_PERIOD_MS);
    Serial.print("I am Task A I am Running on Core_");
    Serial.println(xPortGetCoreID()); 
  }    
}
void task_B(void *parameters){
  while(1){
    vTaskDelay(1000/ portTICK_PERIOD_MS);
    Serial.print("I am Task A I am Running on Core_");
    Serial.println(xPortGetCoreID()); 
  }    
}
//*************************************************************************
// Main (runs as its own task with priority 1 on core 1)
void setup() {
  // Hack to kinda get randomness
  randomSeed(analogRead(0));
  // Configure Serial
  Serial.begin(115200);
  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Race Condition Demo---");
  // Start task 1
  xTaskCreatePinnedToCore(task_A,"Task A",1024,NULL,0,NULL,0);
  // Start task 2
  xTaskCreatePinnedToCore(task_B,"Task B",1024,NULL,1,NULL,1);
// Delete "setup and loop" task
  vTaskDelete(NULL);
}
void loop() {
// Execution should never get here
}
