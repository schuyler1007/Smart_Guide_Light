/**
 * @file DFRobot_LIDAR07.cpp
 * @brief Implementation of DFRobot_LIDAR07 class
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @SKU SEN0413
 * @licence     The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version  V1.0
 * @date  2021-04-16
 * @get from https://www.dfrobot.com
 * @url  https://github.com/DFRobot/DFRobot_LIDAR07
 */
#include"DFRobot_LIDAR07.h"
DFROBOT_LIDAR07 :: DFROBOT_LIDAR07(uint8_t type)
{
  distance = 0;
  amplitude = 0;
  version = 0;

  readVersionPacket = new sSendPacket_t;
  setIntervalPacket = new sSendPacket_t;
  setModePacket = new sSendPacket_t;
  startPacket = new sSendPacket_t;
  stopPacket = new sSendPacket_t;
  startFilterPacket = new sSendPacket_t;
  stopFilterPacket = new sSendPacket_t;

  if(type == 0){
    _type = LIDAR07_IIC;
  } else{
    _type = LIDAR07_UART;
  }
}

DFROBOT_LIDAR07 ::~DFROBOT_LIDAR07()
{
  delete readVersionPacket;
  delete setIntervalPacket;
  delete setModePacket;
  delete startPacket; 
  delete stopPacket;
  delete startFilterPacket;
  delete stopFilterPacket;
  readVersionPacket = NULL;
  setIntervalPacket = NULL;
  setModePacket = NULL;
  startPacket = NULL;
  stopPacket = NULL;
  startFilterPacket = NULL;
  stopFilterPacket = NULL; 
}

bool DFROBOT_LIDAR07:: begin()
{
  bool ret = false;
  uint8_t buff[12];
  delay(500);
  write((void *)readVersionPacket,sizeof(sSendPacket_t));
  read((void *)buff,12);
  if(crc32Check(buff,8)){
    readValue(buff,LIDAR07_VERSION);
    ret = true;
  }
  return setMeasureMode() && ret;
}

bool DFROBOT_LIDAR07:: setMeasureMode()
{
  uint8_t buff[12];
  bool ret = false;
  delay(20);
  write((void *)setModePacket,sizeof(sSendPacket_t));
  read((void *)buff,12);
  if(crc32Check(buff,8)){
    ret = true;
  }
  return ret;
}

bool DFROBOT_LIDAR07::startMeasure()
{
  uint8_t buff[24];
  bool ret = false;
  
  write((void *)startPacket,sizeof(sSendPacket_t));
  delay(20);
  read((void *)buff,24);
  if(crc32Check(buff,20)){
    readValue(buff,LIDAR07_MEASURE);
    ret = true;
  }
  return ret;
}

uint16_t DFROBOT_LIDAR07::getDistanceMM()
{
  return distance;
}

uint16_t DFROBOT_LIDAR07::getSignalAmplitude()
{
  return amplitude;
}

uint32_t DFROBOT_LIDAR07:: getVersion()
{
  return version;
}

void DFROBOT_LIDAR07::stopMeasure()
{
  write((void *)stopPacket,sizeof(sSendPacket_t));
}

bool DFROBOT_LIDAR07::startFilter()
{
  uint8_t buff[12];
  bool ret = false;
  write((void *)startFilterPacket,sizeof(sSendPacket_t));
  delay(20);
  read((void *)buff,12);
  if(crc32Check(buff,8)){
    ret = true;
  }
  return ret;
}

bool DFROBOT_LIDAR07::stopFilter()
{
  uint8_t buff[12];
  bool ret = false;
  write((void *)stopFilterPacket,sizeof(sSendPacket_t));
  delay(20);
  read((void *)buff,12);
  if(crc32Check(buff,8)){
    ret = true;
  }
  return ret;
}

bool DFROBOT_LIDAR07:: crc32Check(uint8_t *buff,uint8_t len)
{
  uint8_t data;
  uint8_t ret  = false;
  uint32_t checkData;
  uint32_t crc = 0xFFFFFFFF;          //Original value
  uint8_t buffCheck[len];
  if(_type == LIDAR07_UART){
   checkData = (uint32_t)buff[len] | ((uint32_t)buff[len+1]<<8) | ((uint32_t)buff[len+2]<<16) | ((uint32_t)buff[len+3]<<24);
    for(uint8_t i=0;i<len;i++){
      buffCheck[i] = buff[i];
    }
  } else {
    checkData = (uint32_t)buff[len-1] | ((uint32_t)buff[len]<<8) | ((uint32_t)buff[len+1]<<16) | ((uint32_t)buff[len+2]<<24);
    buffCheck[0] = 0xFA;
    for(uint8_t i=0;i<len-1;i++){
      buffCheck[i+1] = buff[i];
    }
  }
  for (uint8_t i=0; i<len; i++){
    data = buffCheck[i];
    crc = crc ^ ((uint32_t)data<<24);        //8 bits XOR higher than the crc original value
    for (uint8_t y = 0; y < 8; y++){         //Cycle 8 bits
      if (crc & 0x80000000)                  //The bit shifted to the left is 1, and after shifting to the left, it is XORed with the polynomial
        crc = (crc << 1) ^0x04C11DB7;    
      else                                   //Or shift to left directly
        crc <<= 1;                  
    }
  }
  crc = crc^0x00;                             //At last, return to XOR with the XOR value of result
  if(crc == checkData)
    ret =true;
  return ret;                                //Return the final check value
}

void DFROBOT_LIDAR07::readValue(uint8_t *buff,uint8_t type)
{
  if(_type == LIDAR07_UART){
    if(type == LIDAR07_VERSION){
      version = (uint32_t)buff[4] | ((uint32_t)buff[5]<<8) | ((uint32_t)buff[6]<<16) | ((uint32_t)buff[7]<<24);
    } else if(type == LIDAR07_MEASURE){
      distance = (uint16_t)buff[4] | ((uint16_t)buff[5]<<8);
      amplitude = (uint16_t)buff[8] | ((uint16_t)buff[9]<<8);
    }
  } else {
    if(type == LIDAR07_VERSION){
      version = (uint32_t)buff[3] | ((uint32_t)buff[4]<<8) | ((uint32_t)buff[5]<<16) | ((uint32_t)buff[6]<<24);
    } else if(type == LIDAR07_MEASURE){
      distance = (uint16_t)buff[3] | ((uint16_t)buff[4]<<8);
      amplitude = (uint16_t)buff[7] | ((uint16_t)buff[8]<<8);
    }
  }
}

DFROBOT_LIDAR07_UART::DFROBOT_LIDAR07_UART():DFROBOT_LIDAR07(LIDAR07_UART)
{
}

bool DFROBOT_LIDAR07_UART ::begin(Stream &s)
{
  readVersionPacket->head = 0xF5;
  readVersionPacket->command = 0x43;
  readVersionPacket->data[0] = 0x00;
  readVersionPacket->data[1] = 0x00;
  readVersionPacket->data[2] = 0x00;
  readVersionPacket->data[3] = 0x00;
  readVersionPacket->checkData[0] = 0xAC;
  readVersionPacket->checkData[1] = 0x45;
  readVersionPacket->checkData[2] = 0x62;
  readVersionPacket->checkData[3] = 0x3B;

  setModePacket->head = 0xF5;
  setModePacket->command = 0xE1;
  setModePacket->data[0] = 0x00;
  setModePacket->data[1] = 0x00;
  setModePacket->data[2] = 0x00;
  setModePacket->data[3] = 0x00;
  setModePacket->checkData[0] = 0xA5;
  setModePacket->checkData[1] = 0x8D;
  setModePacket->checkData[2] = 0x89;
  setModePacket->checkData[3] = 0xA7;
    
  startPacket->head = 0xF5;
  startPacket->command = 0xE0;
  startPacket->data[0] = 0x01;
  startPacket->data[1] = 0x00;
  startPacket->data[2] = 0x00;
  startPacket->data[3] = 0x00;
  startPacket->checkData[0] = 0x9F;
  startPacket->checkData[1] = 0x70;
  startPacket->checkData[2] = 0xE9;
  startPacket->checkData[3] = 0x32;

  stopPacket->head = 0xF5;
  stopPacket->command = 0xE0;
  stopPacket->data[0] = 0x00;
  stopPacket->data[1] = 0x00;
  stopPacket->data[2] = 0x00;
  stopPacket->data[3] = 0x00;
  stopPacket->checkData[0] = 0x28;
  stopPacket->checkData[1] = 0xEA;
  stopPacket->checkData[2] = 0x84;
  stopPacket->checkData[3] = 0xEE;

  startFilterPacket->head = 0xF5;
  startFilterPacket->command = 0xD9;
  startFilterPacket->data[0] = 0x01;
  startFilterPacket->data[1] = 0x00;
  startFilterPacket->data[2] = 0x00;
  startFilterPacket->data[3] = 0x00;
  startFilterPacket->checkData[0] = 0xB7;
  startFilterPacket->checkData[1] = 0x1F;
  startFilterPacket->checkData[2] = 0xBA;
  startFilterPacket->checkData[3] = 0xBA;
   
  stopFilterPacket->head = 0xF5;
  stopFilterPacket->command = 0xD9;
  stopFilterPacket->data[0] = 0x00;
  stopFilterPacket->data[1] = 0x00;
  stopFilterPacket->data[2] = 0x00;
  stopFilterPacket->data[3] = 0x00;
  stopFilterPacket->checkData[0] = 0x00;
  stopFilterPacket->checkData[1] = 0x85;
  stopFilterPacket->checkData[2] = 0xD7;
  stopFilterPacket->checkData[3] = 0x66;

  _s = &s;
  return DFROBOT_LIDAR07:: begin();
}


void DFROBOT_LIDAR07_UART:: write(void* pBuf, size_t size)
{
  uint8_t * _pBuf = (uint8_t *)pBuf;
  this->_s->write((const uint8_t *)_pBuf,size);
}

size_t DFROBOT_LIDAR07_UART:: read(void* pBuf, size_t size)
{
  uint8_t count=0;
  uint8_t * _pBuf = (uint8_t *)pBuf;
  int len=size;
  while(!this->_s->available()){
    yield();
  }
  while(this->_s->available()){
      *_pBuf=(uint8_t)this->_s->read();
      if(len == size){
        if(*_pBuf != 0xFA){
          this->_s->flush();
          break;
        }
      }
      _pBuf++;
      len--;
      count++;
      delay(1);
  }
  return count;
}

DFROBOT_LIDAR07_IIC::DFROBOT_LIDAR07_IIC(TwoWire &wire):DFROBOT_LIDAR07(LIDAR07_IIC)
{
  _deviceAddr = LIDAR07_IIC_ADDR;
  _pWire = &wire;
}

bool DFROBOT_LIDAR07_IIC ::begin()
{
  readVersionPacket->head = 0x00;
  readVersionPacket->command = 0x43;
  readVersionPacket->data[0] = 0x00;
  readVersionPacket->data[1] = 0x00;
  readVersionPacket->data[2] = 0x00;
  readVersionPacket->data[3] = 0x00;
  readVersionPacket->checkData[0] = 0x55;
  readVersionPacket->checkData[1] = 0x10;
  readVersionPacket->checkData[2] = 0xCD;
  readVersionPacket->checkData[3] = 0x9A;
   
  setModePacket->head = 0x00;
  setModePacket->command = 0xE1;
  setModePacket->data[0] = 0x00;
  setModePacket->data[1] = 0x00;
  setModePacket->data[2] = 0x00;
  setModePacket->data[3] = 0x00;
  setModePacket->checkData[0] = 0x5C;
  setModePacket->checkData[1] = 0xD8;
  setModePacket->checkData[2] = 0x26;
  setModePacket->checkData[3] = 0x06;
    
  startPacket->head = 0x00;
  startPacket->command = 0xE0;
  startPacket->data[0] = 0x01;
  startPacket->data[1] = 0x00;
  startPacket->data[2] = 0x00;
  startPacket->data[3] = 0x00;
  startPacket->checkData[0] = 0x66;
  startPacket->checkData[1] = 0x25;
  startPacket->checkData[2] = 0x46;
  startPacket->checkData[3] = 0x93;

  stopPacket->head = 0x00;
  stopPacket->command = 0xE0;
  stopPacket->data[0] = 0x00;
  stopPacket->data[1] = 0x00;
  stopPacket->data[2] = 0x00;
  stopPacket->data[3] = 0x00;
  stopPacket->checkData[0] = 0xD1;
  stopPacket->checkData[1] = 0xBF;
  stopPacket->checkData[2] = 0x2B;
  stopPacket->checkData[3] = 0x4F;

  startFilterPacket->head = 0x00;
  startFilterPacket->command = 0xD9;
  startFilterPacket->data[0] = 0x01;
  startFilterPacket->data[1] = 0x00;
  startFilterPacket->data[2] = 0x00;
  startFilterPacket->data[3] = 0x00;
  startFilterPacket->checkData[0] = 0x4E;
  startFilterPacket->checkData[1] = 0x4A;
  startFilterPacket->checkData[2] = 0x15;
  startFilterPacket->checkData[3] = 0x1B;
   
  stopFilterPacket->head = 0x00;
  stopFilterPacket->command = 0xD9;
  stopFilterPacket->data[0] = 0x00;
  stopFilterPacket->data[1] = 0x00;
  stopFilterPacket->data[2] = 0x00;
  stopFilterPacket->data[3] = 0x00;
  stopFilterPacket->checkData[0] = 0xF9;
  stopFilterPacket->checkData[1] = 0xD0;
  stopFilterPacket->checkData[2] = 0x78;
  stopFilterPacket->checkData[3] = 0xC7;

  _pWire->begin();
  return DFROBOT_LIDAR07:: begin();
}

void DFROBOT_LIDAR07_IIC::write(void* pBuf, size_t size)
{
  if(pBuf == NULL){
	  DBG("pBuf ERROR!! : null pointer");
  }
  while(_pWire->available()){
    _pWire->read();
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_deviceAddr);
  for(uint16_t i = 1; i < size; i++){
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}

size_t DFROBOT_LIDAR07_IIC::read(void* pBuf, size_t size)
{
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->requestFrom(_deviceAddr, (uint8_t) size-1);
  for(uint16_t i = 0; i < size-1; i++){
    _pBuf[i] = _pWire->read();
  }
  return size;
}

