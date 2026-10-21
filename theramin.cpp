/**********************************************************************
* Filename    : UltrasonicRanging.c
* Description : Get distance via UltrasonicRanging sensor
* Author      : www.freenove.com
* modification: 2019/12/27
**********************************************************************/
#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>



#define trigPin 4       
#define echoPin 5
#define BUZZER_OUT_PIN  1
#define MAX_DISTANCE 220        // define the maximum measured distance
#define timeOut MAX_DISTANCE*60 // calculate timeout according to the maximum measured distance
#define THREAD_MILLISEC_DELAY  100  // Polling delay in mSecs
//function pulseIn: obtain pulse time of a pin
int pulseIn(int pin, int level, int timeout);

//Global vars
pthread_mutex_t lock;
unsigned long pingTime=0;
pthread_t thread1;


void* getSonar(void* arg){   //get the measurement result of ultrasonic module with unit: cm
    
//   float distance;
    while(1) 
    {
        pthread_mutex_lock(&lock);
        digitalWrite(trigPin,HIGH); //send 10us high level to trigPin 
        delayMicroseconds(10);
        digitalWrite(trigPin,LOW);
        pingTime = pulseIn(echoPin,HIGH,timeOut);   //read plus time of echoPin
        printf("PingTime = %lu\n",pingTime);
        pthread_mutex_unlock(&lock);
        delay(THREAD_MILLISEC_DELAY);
    }
//    distance = (float)pingTime * 340.0 / 2.0 / 10000.0; //calculate distance with sound speed 340m/s
    return NULL;
}


void * buzzerOut(void* arg)
{
    
    digitalWrite(BUZZER_OUT_PIN,LOW);
    
    while (1) 
    {
        
        
int main(){
    void* res;
    
    printf("Program is starting ... \n");
    
    // Init wiring Pi   
    wiringPiSetup();
    pinMode(trigPin,OUTPUT);
    pinMode(echoPin,INPUT);
    pinMode(BUZZER_OUT_PIN,OUTPUT);
    
    // Initialize the mutex lock
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex initialization failed\n");
        return 1;
    }   
    
    pthread_create(&thread1, NULL, getSonar, (void*)NULL);
    
     pthread_join(thread1, &res);
    
//   while(1){
//       distance = getSonar();
//       printf("The distance is : %.2f cm\n",distance);
//        delay(1000);
//    }  

    printf("Main: Thread 1 returned %d\n", *(int*)res);
    return 0;
}

int pulseIn(int pin, int level, int timeout)
{
   struct timeval tn, t0, t1;
   long micros;
   gettimeofday(&t0, NULL);
   micros = 0;
   while (digitalRead(pin) != level)
   {
      gettimeofday(&tn, NULL);
      if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
      micros += (tn.tv_usec - t0.tv_usec);
      if (micros > timeout) return 0;
   }
   gettimeofday(&t1, NULL);
   while (digitalRead(pin) == level)
   {
      gettimeofday(&tn, NULL);
      if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
      micros = micros + (tn.tv_usec - t0.tv_usec);
      if (micros > timeout) return 0;
   }
   if (tn.tv_sec > t1.tv_sec) micros = 1000000L; else micros = 0;
   micros = micros + (tn.tv_usec - t1.tv_usec);
   return micros;
}
