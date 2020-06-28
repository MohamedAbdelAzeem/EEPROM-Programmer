#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPORM_D7 12
#define WRITE_EN 13

void Set_value(int value, bool outputEnable)
{
  // we will tide the last bit of the second shift register with the OUTPUT ENABLE PIN in the 
  //EEPROM, So we need to handle it with the first shiftOut func. 
  /* 0000 0111    value>>8
   *|1000 0000    if ouputEnable is False (EEPROM set in input mode) = 0x80 
   * 1000 0111    so add the outpit enable bit in the last digit of the shiftregister 
   *              because we make it MSBFIRST.
   */
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (value>>8)| (outputEnable ? 0x00 : 0x80)); // TO SHIFT THE TOP 3 BITS FIRST into 
  //the second shift regiter
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, value);
  // moving data form data register to storage register.
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

byte readEEPROM_byte(int Address)
{
  for(int pin = EEPROM_D0 ; pin <= EEPROM_D7; pin++)
  {
    pinMode(pin, INPUT);
  }
  Set_value(Address, true /*EEPROM IN READ MODE*/);
  byte data = 0 ;
  for(int pin = EEPORM_D7 ; pin >= EEPROM_D0 ; pin--)
  {
    data = (data << 1) + digitalRead(pin);
    /* (1) 0000 0000 shifted to 0000 0001 if the D7 is 1 
     *  and we add the value of the pin from D7 to D0 and shift every value 
     *  0000 0001
     *  0000 0011
     *  0000 0111
     *  0000 1111  
     *  ...
     *  1111 1111   if D7 to D0 is 1 
     */
     return data; 
  }
}

void printEEPROM_CONTENT()
{
  for(int pin = EEPROM_D0 ; pin <= EEPROM_D7; pin++)
  {
    pinMode(pin, INPUT);
  }
  Serial.begin(57600);
  // we will print 16 bytes in one line
  for(int base = 0 ; base <= 255 ; base+=16) //in one line 16 bytes
  {// we can put base <= 2048 if we want to read everything in the EEPORM.
    byte data[16]; //array of byts in each line 
    for(int offset = 0 ; offset <= 15;offset++)
    {
      data[offset] = readEEPROM_byte(base + offset);
    }

    char buff[80];
    sprintf(buf, "%03x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
    base , data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
     , data[10], data[12], data[13], , data[14], data[15]);
  }
  Serial.println(buf);
}

void writeEEPORM(int address, int data0)
{
  for(int pin = EEPROM_D0 ; pin <= EEPROM_D7; pin++)
  {
    pinMode(pin, OUTPUT);
  }
  Set_Adress(address, false /*write mode*/);
  for (int pin = EEPROM_D0 ; pin <= EEPROM_D7 ; pin++)
  {
    digitalWrite(pin, data & 1); //to get the LSB of the data and output it 
    //we need to get the second bit 
    data = data << 1 ;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10); //to be tested
  
  
}
  
   

void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT); 

   digitalWrite(WRITE_EN, HIGH);  // we set the write_en high if we want to write we pulse it low.
   pinMode(WRITE_EN, OUTPUT);    // after digitalwirte to make sure that the pin already high.

   
}

void loop() {
  // put your main code here, to run repeatedly:

}
