//magic toilet

#include <SD.h>
#include <TMRpcm.h>  
#include <SPI.h>

TMRpcm tmrpcm; 

#define SENSOR_PIN A0
#define SD_PIN 4
#define SPEAKER_PIN 9

File fileDirectory;

void setup() {
  Serial.begin(115200);
  
  tmrpcm.speakerPin = SPEAKER_PIN;
  
  pinMode(SD_PIN, OUTPUT);
  if (!SD.begin(SD_PIN)) {
    Serial.println("SD failed!");
    while(true);
  }

  fileDirectory = SD.open("/");
  
  tmrpcm.play("RECORD~1.WAV"); //the sound file "music" will play each time the arduino powers up, or is reset
}

void printConcatLine(const char* mask, ...) {
    va_list params;
    va_start(params, mask);
    
    char *ptr = (char *)mask;
    while (*ptr != '\0') {
        if (*ptr == 'c') {
            int c = va_arg(params, int);
            Serial.write(c);
        } else if (*ptr == 'i') {
            int i = va_arg(params, int);
            Serial.print(i);
        } else if (*ptr == 's') {
            const char *s = va_arg(params, const char *);
            Serial.print(s);
        } else if (*ptr == 'f' || *ptr == 'd') {
            // Be careful with this. It's not portable. On AVR float
            // and double are the same, but that's not the case on other
            // microcontrollers. It would be better to split them.
            double d = va_arg(params, double);
            Serial.print(d);
        } else {
            Serial.write(*ptr);
        }
        ptr++;
    }
    
    va_end(params);
    Serial.println();
}

void loop() {
  int sensor = analogRead(SENSOR_PIN); 

  Serial.println(sensor);
  if(sensor <= 500) {
    //flushed toilet
    playFile(fileDirectory);
    
    delay(60000);
  }
}

void playFile(File playDirectory) {
  while(true) {
    File playFile = playDirectory.openNextFile();
      printConcatLine("c", "Playing: ");
      Serial.println(playFile.name());
    if(!playFile) {
      playDirectory.rewindDirectory();
    } else {
      break;
    }  
  }
  //printConcatLine("cc", "Playing: ", playFile.name());
//  AudioZero.play(playFile);
}
