//################################################################################################
void serialWorker()
{
  pthread_t serialReaderThread;
  
  int iret = pthread_create(&serialReaderThread, NULL, readSerial, (void*)NULL);
  pthread_detach(serialReaderThread);
  
}
//################################################################################################
//################################################################################################
void *readSerial(void *arg)
{
  std::deque<byte> packetBuffer; //data buffer before pushing into queue
  while(true)
  {
    if(DEBUG == 2)
    {
      Serial.println("Serial loop");
    }
    /**
    while(Serial1.available())
    {
      packetBuffer.push_back(Serial1.read());
      //Serial.println(packetBuffer[index-1]);
    }
    **/
    if(Serial1.available())
    {
      while(Serial1.available())
      {
         packetBuffer.push_back((byte)Serial1.read());
      }
        
        //push whatever was read into queue
        if(!packetBuffer.empty())
        {
          //data was read
          pthread_mutex_lock(&dataBufferMutex);
          while(!packetBuffer.empty())
          {
            dataBuffer.push_back (packetBuffer.front());
            packetBuffer.pop_front();
          }
          pthread_mutex_unlock(&dataBufferMutex);
        }
    }
    else
    {
      if(DEBUG == 2)
      {
        Serial.println("No serial data available");
      }
    }
    delay(100);  //if no data sleep for 100ms
  }
}
//################################################################################################
//################################################################################################
void sendDataToModule(byte output[], int modID, int packetLength)
{
  for(int i =0; i<packetLength; i++)
  {
    Serial1.write(output[i]);
  }
}
//################################################################################################
