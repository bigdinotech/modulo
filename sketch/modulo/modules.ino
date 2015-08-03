//################################################################################################
void moduleWorker()
{
  pthread_t moduleWorkerThread;
  int iret = pthread_create(&moduleWorkerThread, NULL, moduleHandler, (void*)NULL);
  pthread_detach(moduleWorkerThread);
}
//################################################################################################
//################################################################################################
void *moduleHandler(void *arg)
{
  while(true)
  {
    if(DEBUG == 2)
    {
      Serial.println("Module worker loop");
    }

    struct moduleData m;
    while(!validPackets.empty())  //read should be atomic enough
    {
      m.ID = -1;
      pthread_mutex_lock(&validPacketMutex);
      if(!validPackets.empty())
      {
        m = validPackets.front();  //this will also set m.ID to a valid value greater than 0
        validPackets.pop_front();
      }
      pthread_mutex_unlock(&validPacketMutex);
      if(m.ID >= 0)
      {
        //add module data to udp packets to be sent
        pthread_mutex_lock(&udpPacketMutex);
        udpPackets.push_back(m);
        pthread_mutex_unlock(&udpPacketMutex);


        //module specific tasks
        if(DEBUG)
        {
          Serial.print("Processing data from module: ");
          Serial.println(m.ID);
        }
        switch(m.ID)
        {
        case 0:
          break;
        default:
          break;
        }
      }
    }
    delay(100);
  }  
}
//################################################################################################
//################################################################################################
void commandWorker()
{
  pthread_t commandWorkerThread;
  int iret = pthread_create(&commandWorkerThread, NULL, commandHandler, (void*)NULL);
  pthread_detach(commandWorkerThread);
}
//################################################################################################
//################################################################################################
void *commandHandler(void *arg)
{
  while(true)
  {
    if(DEBUG == 2)
    {
      Serial.println("Command Loop");
    }
    String cmd = "";
    int packetLength = 0;  
    pthread_mutex_lock(&commandQueueMutex);
    if(!commandQueue.empty())
    {
      cmd = commandQueue.front();
      commandQueue.pop_front();
      if(DEBUG)
      {
        Serial.print("Cmd String length: ");
        Serial.println(cmd.length());
        Serial.print("Command String: ");
        Serial.println(cmd);
      }
    }
    pthread_mutex_unlock(&commandQueueMutex);

    {
      //parse Command
      int cmdStringLength = cmd.length();
      if(cmdStringLength >=8)
      {
        String byteString = "";
        byte cmdBuffer[128];
        byte *dataBuff;
        int byteCounter = 0;
        for(int i =0; i < cmdStringLength; i++)
        {
          if(cmd[i] != ' ')
          {
            byteString += cmd[i];
          }
          else
          {
            //convert byteString into byte array
            cmdBuffer[byteCounter] = (byte)byteString.toInt(); 
            byteCounter++;
            byteString = "";
          }
        }
        //cmdBuffer[byteCounter] = (byte)byteString.toInt(); 
        //byteCounter++;
        dataBuff = new byte[byteCounter];
        packetLength = byteCounter;
        if(DEBUG)
        {
          Serial.print("Bytes received: ");
          Serial.println(byteCounter);
        }
        for(int i = 0; i < byteCounter; i++)
        {
          dataBuff[i] = cmdBuffer[i];
        }

        if(dataBuff[1] != 0)
        {
          //command is for other modules
          sendDataToModule(dataBuff,dataBuff[1], packetLength);
        }
        else
        {
          //command is for dataFusion module
        }
        delete [] dataBuff;
      }
    }
    delay(100);
  }
}
//################################################################################################
//################################################################################################
void transferModuleData(struct moduleData modData)
{
}
//################################################################################################


//*******************************************Modules**********************************************
//################################################################################################
void garageModule(byte packet[])
{
}
//################################################################################################

