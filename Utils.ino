byte hexton (byte h)
{
  if (h >= '0' && h <= '9')
    return(h - '0');
  if (h >= 'A' && h <= 'F')
    return((h - 'A') + 10);
  //error("Bad hex digit!");
}

uint8_t exploreFiles(uint8_t index, char part){
  SdFile dirFile;
  //getText(index, part);
  dirFile.open("/");
  uint8_t n = 0;
  uint8_t nMax = 8;
  uint8_t dirIndex[8];
  uint8_t pageIndex;
  
  while (n < nMax && myFile.openNext(&dirFile, O_READ)) {

    // Skip directories and hidden files.
    if (!myFile.isSubDir() && !myFile.isHidden()) {

      // Save dirIndex of file in directory.
      dirIndex[n] = myFile.dirIndex();

      // Print the file number and name.
      //Serial.print(n++);
      //Serial.write(' ');
      //myFile.printName(&Serial);
      //Serial.println();
    }
    myFile.close();
  }
  capturePress();  
  
}

uint8_t getText(uint8_t index, char part){
  myFile = SD.open((String("lang/") + lang[0] + lang[1] + String(".txt")).c_str(), FILE_READ);
  char item[3] = {index+0x30, part, ':'};
  char buf[3];
  
  while(myFile.available()){
    myFile.read(buf, 3); 
    if(strncmp(buf, item, 3) == 0){
      uint8_t counter = 0; 
      char tmp = myFile.read();
         while(tmp != '\n'){
           tmpText[counter++] = tmp;
           tmp = myFile.read();
         }
      tmpText[counter] = 0;  
     myFile.close(); 
     return 0;
    }
    while(myFile.read() != '\n');
  }
}

void getMcuDetails(void){
  
  myFile = SD.open("chip.db", FILE_READ);
  char buf[6];
  String sig = String(myMCU.signature, HEX);
  sig.toUpperCase();
  while(myFile.available()){
  myFile.read(buf,6);     
       if(strncmp(buf, sig.c_str(), 6) == 0)
       {
       memset(buf, 0, sizeof(buf));

       myMCU.defFuseLow = hexton(myFile.read())<<4;
       myMCU.defFuseLow |= hexton(myFile.read());
//       Serial.print("Default Fuse Low: ");
//       Serial.println(myMCU.defFuseLow, HEX);
       
       myMCU.defFuseHigh = hexton(myFile.read())<<4;
       myMCU.defFuseHigh |= hexton(myFile.read());
//       Serial.print("Default Fuse High: ");
//       Serial.println(myMCU.defFuseHigh, HEX);
       

       myMCU.defFuseExt = hexton(myFile.read())<<4;
       myMCU.defFuseExt |= hexton(myFile.read());
//       Serial.print("Default Fuse Ext: ");
//       Serial.println(myMCU.defFuseExt, HEX);
       
       myFile.read(buf, 2);

       myMCU.flashPageSize = hexton(myFile.read())<<4;
       myMCU.flashPageSize |= hexton(myFile.read());
//       Serial.print("Flash Page Size: ");
//       Serial.println(myMCU.flashPageSize*2, DEC);
       
       myMCU.flashPages = hexton(myFile.read())<<4;
       myMCU.flashPages |= hexton(myFile.read()); 
       myMCU.flashPages |= hexton(myFile.read())<<12; 
       myMCU.flashPages |= hexton(myFile.read())<<8;      
//       Serial.print("Number of Flash Pages: ");
//       Serial.println(myMCU.flashPages, DEC);
       

       myMCU.eepromPageSize = hexton(myFile.read())<<4;
       myMCU.eepromPageSize |= hexton(myFile.read());
//       Serial.print("EEPROM Page Size: ");
//       Serial.println(myMCU.eepromPageSize*2, DEC);
       
       myMCU.eepromPages = hexton(myFile.read())<<4;
       myMCU.eepromPages |= hexton(myFile.read()); 
       myMCU.eepromPages |= hexton(myFile.read())<<12; 
       myMCU.eepromPages |= hexton(myFile.read())<<8;      
//       Serial.print("Number of EEPROM Pages: ");
//       Serial.println(myMCU.eepromPages, DEC);
       
       myMCU.fuseDelay = hexton(myFile.read())<<4;
       myMCU.fuseDelay |= hexton(myFile.read());
       
       myMCU.flashDelay = hexton(myFile.read())<<4;
       myMCU.flashDelay |= hexton(myFile.read());
       
       myMCU.eepromDelay = hexton(myFile.read())<<4;
       myMCU.eepromDelay |= hexton(myFile.read());
       
       myFile.read(buf, 4);
       myMCU.Name = "";
       while(myFile.position() % 52){
             char buff = myFile.read();
             if(buff != ' ' && buff != 13 && buff != 10)
             myMCU.Name += buff;  
          }
       
          break;     
       }
       myFile.seek(myFile.position() + 46);
  }
  myFile.close();
}

void initSD(){
    setBit(PORTB, PB2)  //pinMode(SS, OUTPUT);
    if(!SD.begin(chipSelect)){
      tft.setCursor(50, 50);
      tft.setTextColor(RED);  tft.setTextSize(4);
      tft.print("SD Error!");
      return;
      }
}
