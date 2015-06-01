void start_pmode () {
  //pinMode(SCK, INPUT); // restore to default

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128); 
  
  //spi_init();
  
  //debug("...spi_init done");
  // following delays may not work on all targets...

  setBit(DDRC, RESET);     //pinMode(RESET, OUTPUT);
  setBit(PORTC, RESET);    //digitalWrite(RESET, HIGH);
  setBit(DDRB, SCK);     //pinMode(SCK, OUTPUT);
  clrBit(PORTB, SCK);    //digitalWrite(SCK, LOW);
  delay(50);
  clrBit(PORTC, RESET);    //digitalWrite(RESET, LOW);
  delay(50);
  clrBit(PORTB, MISO);    //pinMode(MISO, INPUT);
  setBit(PORTB, MOSI);    //pinMode(MOSI, OUTPUT);
  //debug("...spi_transaction");
  spi_transaction(0xAC, 0x53, 0x00, 0x00);
  //debug("...Done");
}

void end_pmode() {
  SPI.end();
  clrBit(DDRB, MISO);   //pinMode(MISO, INPUT);
  clrBit(DDRB, MISO);   //pinMode(MOSI, INPUT);
  clrBit(DDRB, SCK);    //pinMode(SCK, INPUT);
  clrBit(DDRC, RESET);  //pinMode(RESET, INPUT);
  delay(20);
}

void getChipInformation(){
  start_pmode();
  readSignature();
  readFuses();
  getMcuDetails();
}

void readSignature (void)
{
  SPI.setClockDivider(clockSpeed); 
    
  uint8_t error = 3;
  while(error--){
  myMCU.signature = spi_transaction(0x30, 0x00, 0x00, 0x00) & 0xFF;
  myMCU.signature <<= 8;
  myMCU.signature |= spi_transaction(0x30, 0x00, 0x01, 0x00) & 0xFF;
  myMCU.signature <<= 8;  
  myMCU.signature |= spi_transaction(0x30, 0x00, 0x02, 0x00) & 0xFF;
  if((myMCU.signature & 0xFF0000) == 0x1E0000) break;
  end_pmode();
  start_pmode();
  }
}

void readFuses (void)
{
  SPI.setClockDivider(clockSpeed); 
  
  myMCU.fuseHigh = spi_transaction(0x58, 0x08, 0x00, 0x00);
  myMCU.fuseLow  = spi_transaction(0x50, 0x00, 0x00, 0x00);
  myMCU.fuseExt  = spi_transaction(0x50, 0x08, 0x00, 0x00);

}

boolean setFuses(char *filename){
  myFile = SD.open(filename, FILE_READ);
  uint8_t high = (hexton(myFile.read())<<4) | hexton(myFile.read());
  uint8_t low = (hexton(myFile.read())<<4) | hexton(myFile.read());
  uint8_t ext = (hexton(myFile.read())<<4) | hexton(myFile.read());
  myFile.close();
  programFuses(high, low, ext);
}

uint8_t setDefaultFuses(){
  getChipInformation(); 
  programFuses(myMCU.defFuseHigh, myMCU.defFuseLow, myMCU.defFuseExt);
  return 0;
}

boolean programFuses (uint8_t high, uint8_t low, uint8_t ext)
{
  start_pmode ();
    if(high)spi_transaction(0xAC, 0xA8, 0x00, high);
    delay(myMCU.fuseDelay);
    if(low)spi_transaction(0xAC, 0xA0, 0x00, low);    
    delay(myMCU.fuseDelay);
    if(ext)spi_transaction(0xAC, 0xA4, 0x00, ext);
    delay(myMCU.fuseDelay);
   //end_pmode();
  return true;			/* */
}

void eraseChip(void) {
  SPI.setClockDivider(clockSpeed);    
  spi_transaction(0xAC, 0x80, 0, 0);	// chip erase    
  busyWait();
}

// Simply polls the chip until it is not busy any more - for erasing and programming
void busyWait(void)  {
  while(spi_transaction(0xF0, 0x0, 0x0, 0x0) & 0x01);
}

uint8_t spi_transaction(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  SPI.transfer(a);
  SPI.transfer(b);
  //if (n != a) error = -1;
  SPI.transfer(c);
  return SPI.transfer(d);
}
