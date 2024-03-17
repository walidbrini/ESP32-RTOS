#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"


#define TAILLE_MAX  10

int tab[TAILLE_MAX] ; 
SemaphoreHandle_t s1 = NULL; 
SemaphoreHandle_t s2 = NULL; 
SemaphoreHandle_t mutex = NULL ; 

int k = 0 ; 

void vProducteur( void *pvParameters )
{
  const char *pcTaskName = "Producteur";
  int valueToSend;
  BaseType_t status;
  UBaseType_t uxPriority;
  valueToSend = ( int ) pvParameters;
  uxPriority = uxTaskPriorityGet( NULL );
  int val ; 
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  int i = 0 ; 
  int j = 0 ; 
  for(;;){
    val = rand(); 
    xSemaphoreTake(s1 , portMAX_DELAY);
    tab[j] = val%2 ; 
    printf("Valeur produite %d par le %s\n" , tab[j], pcTaskName);
    printf("%s - core = %d (priorite %d)\n", pcTaskName, xPortGetCoreID(), uxPriority);
    printf("---------------------------------------------------------------------------------------\n");
    j = (j + 1) % TAILLE_MAX ; 
    i++ ; 
    xSemaphoreGive(s2); 
    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 125 ) );
  }

}

void vConsomateur( void *pvParameters )
{
  const char *pcTaskName = "Consomateur";
  int receivedValue;
  BaseType_t status;
  UBaseType_t uxPriority;
  uxPriority = uxTaskPriorityGet( NULL );
  int val ; 
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  int i = 0 ; 
  for (;;){
      xSemaphoreTake(s2, portMAX_DELAY);
      val = tab[k] ; 
      printf("Valeur consomé %d par le %s\n" , tab[k], pcTaskName);
      printf("%s - core = %d (priorite %d)\n", pcTaskName, xPortGetCoreID(), uxPriority);
      printf("---------------------------------------------------------------------------------------\n");
      k = (k+1)%TAILLE_MAX ; 
      i++ ; 
      xSemaphoreGive(s1); 
      vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(250) );
    }
  vTaskDelete(NULL); 
}


void vConsomateur2( void *pvParameters )
{
  const char *pcTaskName = "Consomateur2";
  int receivedValue;
  BaseType_t status;
  UBaseType_t uxPriority;
  uxPriority = uxTaskPriorityGet( NULL );
  int val ; 
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  int i = 0 ; 
  for (;;){
      
      xSemaphoreTake(s2, portMAX_DELAY);
      val = tab[k] ; 
      printf("Valeur consomé %d par le %s\n" , tab[k], pcTaskName);
      printf("%s - core = %d (priorite %d)\n", pcTaskName, xPortGetCoreID(), uxPriority);
      printf("---------------------------------------------------------------------------------------\n");
      k = (k+1)%TAILLE_MAX ; 
      i++ ; 
      xSemaphoreGive(s1); 
      vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(250) );
    }
  vTaskDelete(NULL); 
}


void app_main() {
    s1 = xSemaphoreCreateCounting( TAILLE_MAX, TAILLE_MAX );
    s2 = xSemaphoreCreateCounting( TAILLE_MAX, 0 );
    mutex = xSemaphoreCreateMutex(); 

    xTaskCreatePinnedToCore( vProducteur, "Producteur", 10000, NULL, 1, NULL , 0 );  
    xTaskCreatePinnedToCore( vConsomateur, "Consomatuer", 10000, NULL, 2,NULL ,  0 );
    xTaskCreatePinnedToCore( vConsomateur2, "Consomatuer", 10000, NULL, 2,NULL ,  0 );


}