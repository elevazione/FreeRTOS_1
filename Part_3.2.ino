#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int led2=2;
static const int led4=4;

void toggleLED_1(void *parameter) {
  while(1) {
    digitalWrite(led2, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led2, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void toggleLED_2(void *parameter) {
  while(1) {
    digitalWrite(led4, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(led4, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void setup() {

  pinMode(led4, OUTPUT);
  pinMode(led2, OUTPUT);
  xTaskCreatePinnedToCore(toggleLED_1,"Toggle 1",1024,NULL,1,NULL,app_cpu);    
  xTaskCreatePinnedToCore(toggleLED_2,"Toggle 1",1024,NULL,1,NULL,app_cpu); 

}

void loop() {
 
}
