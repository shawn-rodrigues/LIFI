//Transmitter Code
#include <elapsedMillis.h>
#include <SD.h>

const unsigned long INTERVAL = 30000;
int i=0;
elapsedMicros timer0;
File file;

int CS_PIN = 10;

void setup() {

  int transPin = 8;
  int baudrate = 9600; 
  pinMode(transPin, OUTPUT);//light output port
  Serial.begin(baudrate);
  Serial.print("Transmission Pin: ");
  Serial.println(transPin);
  
  char fileName[] = "hello"; 
  delay(10000);
  Serial.println("Sending file...");
  //Send Data
  sendFile(fileName);
  Serial.println("File Sent!");
}
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

 file = SD.open("/");

  printDirectory(file, 0);

  Serial.println("done!");
}

void loop() {
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

int  sendFile (char input[])
{
   initializeSD();
   openFile(input); 
   //send file size
   unsigned long fileSize = file.size();
   byte buf[4];
   buf[0] = (byte) fileSize & 0xFF;
   buf[1] = (byte) (fileSize >> 8) & 0xFF;
   buf[2] = (byte) (fileSize >> 16) & 0xFF;
   buf[3] = (byte) (fileSize >> 24) & 0xFF;  
   for(int i = 0; i < 4; i++)
   {
    Serial.println(buf[i]);
    sendValue(buf[i]); 
   }
   int lengthName =strlen(input);
   sendValue((byte)lengthName);
   for(i=0;i<lengthName;i++)
   sendValue(input[i]);
   //send data
   while(file.available())
   {
      sendValue(file.read());
   }
   file.close();
   return 0; 
}

void loop() {
}

void sendValue(byte valToSend)
{
  //Send start bit
  digitalWrite(8, HIGH);
  timer0 = 0;
  byte b;
  //Send actual value
  for(int i = 0; i < 8; i++)
  {
      b = valToSend%2; //convert decimal to binary
      valToSend=valToSend/2;
      //Delay until time to send
      while(timer0 < INTERVAL)
      {
      }
      if(b==1)
      {
        digitalWrite(8, HIGH);
      }
      else
      {
        digitalWrite(8, LOW);
      }
      timer0 = 0;
  }
  while(timer0< INTERVAL)
  {}//Busy wait on last bit
  //Send stop bit
  digitalWrite(8, LOW);
  timer0 = 0;
  while(timer0 < INTERVAL)
  {
  }//Delay on stop bit
  
}

void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

int openFile(char filename[])
{
  file = SD.open("hello.txt");
  if (file)
  {
    Serial.println("File opened with success!");
    return 1;
  } else
  {
    Serial.println("Error opening file...");
    return 0;
  }
}
