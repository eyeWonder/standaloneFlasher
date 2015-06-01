TSPoint p;

#define MAINMENU 1
#define PROGRAMMENU 2
#define READMENU 3
#define OPTIONSMENU 4
#define PROGRAMFUSEMENU 5
#define NOMENU 0
uint8_t currentMenu = NOMENU;

uint8_t getText(uint8_t index, char button);

uint8_t menu(uint8_t menuIndex, uint8_t (*func1)(), uint8_t (*func2)(),uint8_t (*func3)(),uint8_t (*func4)())
{
  while(1){
    if(currentMenu != menuIndex){
      currentMenu = menuIndex;
      tft.fillScreen(WHITE);
      tft.setTextSize(2);tft.setTextColor(BLACK);
      getText(menuIndex, 'A');
      drawButton(10,10,145,105);
      getText(menuIndex, 'B');
      drawButton(165,10,145,105);
      getText(menuIndex, 'C');
      drawButton(10,125,145,105);
      getText(menuIndex, 'D');
      drawButton(165,125,145,105);
    }
    capturePress();  
    switch(optionPressed()){
      case 1 : if(func1()) return 0; break;
      case 2 : if(func2()) return 0; break;
      case 3 : if(func3()) return 0; break;
      case 4 : if(func4()) return 0; break;
      default: break;
  }
  }
}

uint8_t mainMenu(){
  return  menu(MAINMENU, &programMenu, &readMenu, &drawSpecs, &optionsMenu);
}

uint8_t programMenu(){
  return menu(PROGRAMMENU, &noFunc,&fuseMenu,&noFunc,&back);
}

uint8_t readMenu(){
  return menu(READMENU, &noFunc,&noFunc,&noFunc,&back);
}

uint8_t fuseMenu(){
return menu(PROGRAMFUSEMENU, &noFunc, &noFunc, &setDefaultFuses, &back);
}

uint8_t optionsMenu(){
  exploreFiles(0,0);
  return 0;
}

uint8_t noFunc(){
return 0;
}

uint8_t back(){
return 1;
}

//Define the first Line and the Distance to the next. FisrtLine + NextlineNumber * LineNumber
#define lineNumber(LINE) 40+21*LINE  

uint8_t drawSpecs(){
    currentMenu = NOMENU; 
    getChipInformation();
    tft.fillScreen(0xFFFF);
    
    //Draw the Frame
    tft.fillRoundRect(10,10,300,220,3,LIGHTGREY);
    tft.drawRoundRect(10,10,300,220,3,BLACK);
    tft.drawRoundRect(10,10,299,219,3,DARKGREY);
    tft.drawFastHLine(10,34,300, BLACK);
      
    //Print Header
    tft.setCursor(13, 16);
    tft.setTextColor(BLACK);  tft.setTextSize(2);
    getText(17, 'A');
    tft.print(tmpText);
    if(myMCU.signature != 0 && myMCU.signature != 0xFFFFFF){  
        //Print Device Name
        tft.setCursor(13, lineNumber(0));
        getText(17, 'B');
        tft.print(tmpText);
        tft.setCursor(136, lineNumber(0));
        tft.print(myMCU.Name);
          
        //Print Low Fuse (+ Default)
        tft.setCursor(13, lineNumber(1));
        getText(17, 'C');
        tft.print(tmpText);
        tft.setCursor(136, lineNumber(1));
        tft.print("0x");
        if(myMCU.fuseLow < 0x10) tft.print("0");
        tft.print(myMCU.fuseLow, HEX);
        tft.print(" (0x");
        if(myMCU.defFuseLow < 0x10) tft.print("0");
        tft.print(myMCU.defFuseLow, HEX);
        tft.print(")");
         
        //Print High Fuse (+ Default)
        tft.setCursor(13, lineNumber(2));
        getText(17, 'D');
        tft.print(tmpText);
        tft.setCursor(136, lineNumber(2));
        tft.print("0x");
        if(myMCU.fuseHigh < 0x10) tft.print("0");
        tft.print(myMCU.fuseHigh, HEX);
        tft.print(" (0x");
        if(myMCU.defFuseHigh < 0x10) tft.print("0");
        tft.print(myMCU.defFuseHigh, HEX);
        tft.print(")");
          
        //Print Ext Fuse (+ Default)
        tft.setCursor(13, lineNumber(3));
        getText(17, 'E');
        tft.print(tmpText);
        tft.setCursor(136, lineNumber(3));
        tft.print("0x");
        if(myMCU.fuseExt < 0x10 || myMCU.fuseExt == 0xFF) tft.print("0");
        tft.print(((myMCU.fuseExt == 0xFF) ? 0 : myMCU.fuseExt), HEX);
        tft.print(" (0x");
        if(myMCU.defFuseExt < 0x10) tft.print("0");
        tft.print(myMCU.defFuseExt, HEX);
        tft.print(")");
          
        //Print Flash Size
        tft.setCursor(13, lineNumber(4));
        getText(17, 'F');
        tft.print(tmpText);
        tft.setCursor(136, lineNumber(4));
        tft.print(myMCU.flashPageSize * myMCU.flashPages * 2, DEC);
          
        //Print Eeprom Size
        tft.setCursor(13, lineNumber(5));
        getText(17, 'G');
        tft.print(tmpText);
        tft.setCursor(136, lineNumber(5));
        tft.print(myMCU.eepromPageSize * myMCU.eepromPages * 2, DEC);
         
        //Print Signature
        tft.setCursor(13, lineNumber(6));
        getText(17, 'H');
        tft.print(tmpText);
        tft.setCursor(136, lineNumber(6));
        tft.println(myMCU.signature, HEX);
    }else{
        tft.setCursor(13, lineNumber(0));
        getText(17, 'I');
        tft.print(tmpText);
        tft.setCursor(13, lineNumber(1));
        getText(17, 'J');
        tft.print(tmpText);
    }
    capturePress();
    return 0;
}

void drawButton(uint8_t x, uint8_t y, uint8_t width, uint8_t height){
  tft.fillRoundRect(x, y, width, height, 3, LIGHTGREY);
  tft.drawRoundRect(x, y, width, height, 3, BLACK);
  tft.drawRoundRect(x, y, width-1, height-1, 3, DARKGREY);
  if(strchr(tmpText, '\\')){
    char text2[strlen(tmpText)+1];
    strcpy(text2, tmpText);
    char *tmp = strtok(text2, "\\"); 
    tft.setCursor(x + (width/2 - strlen(tmp)*6), y + (height/2 - 17));
    tft.print(tmp);
    
    tmp = strtok(NULL, "\\"); 
    tft.setCursor(x + (width/2 - strlen(tmp)*6), y + (height/2 + 3));
    tft.print(tmp);  
  }else{
    tft.setCursor(x + (width/2 - strlen(tmpText)*6), y + (height/2 - 7));
    tft.print(tmpText);
  } 
}
uint8_t optionPressed(){
    //returns 1-4 for good Press, 5-7 for bad press
    return ((p.x >= 10 && p.x <= 155) ? 1 : ((p.x >= 165 && p.x <= 310) ? 2 : 0)) +
           ((p.y >= 10 && p.y <= 115) ? 0 : ((p.y >= 125 && p.y <= 230) ? 2 : 5));
}
void capturePress(){
do{
  setBit(PORTB, SCK);  //digitalWrite(13, HIGH);
  p = ts.getPoint();
  clrBit(PORTB, SCK);  //digitalWrite(13, LOW);
  #if DEBUG == 1
    //Serial.print("x: ");
    //Serial.println(p.x);
    //Serial.print("y: ");
    //Serial.println(p.y);
    //Serial.print("z: ");
    //Serial.println(p.z);
  #endif
  }while(p.z < 10 || p.z > 1000);
  int tmp = p.x;
  p.x = map(p.y, 120, 915, 320, 0);
  p.y = 240-map(tmp, 110, 900, 240, 0);
  setBit(DDRC, PC1);   //pinMode(XM, OUTPUT);
  setBit(DDRC, PC2);   //pinMode(YP, OUTPUT);
  p.z = 0;
  delay(100); 
}

void initTFT(){
  tft.reset();
  tft.begin(tft.readID());
  tft.fillScreen(WHITE);
  tft.setRotation(3);
}
