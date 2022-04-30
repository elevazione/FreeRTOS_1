#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int led2=2;
static const int led4=4;

int value=500;
char val;
String pass;

void toggleLED_1(void *parameter) {
  
  while(1) {
    digitalWrite(led2, HIGH);
    vTaskDelay(value/ portTICK_PERIOD_MS);
    digitalWrite(led2, LOW);
    vTaskDelay(value/ portTICK_PERIOD_MS);
  }
}

void input(void *parameter){
 while(1)
 {
  if(Serial.available()>0){   
     val=Serial.read();
     pass=pass+val; 
     value=pass.toInt();     
  }  
  if(Serial.available()==0){
     vTaskDelay(100/portTICK_PERIOD_MS);
     pass="";
  }
 }   
}
void setup() {
  Serial.begin(115200);
  pinMode(led2, OUTPUT);
  xTaskCreatePinnedToCore(input,"Input",1024,NULL,1,NULL,app_cpu);  
  xTaskCreatePinnedToCore(toggleLED_1,"Toggle 1",1024,NULL,1,NULL,app_cpu);   
}
void loop() {

}
