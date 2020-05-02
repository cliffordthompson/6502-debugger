//***************************************************************************//
//*                                                                         *//
//*  Copyright (c) 2020 Clifford Thompson                                   *//
//*                                                                         *//
//*  All code in this file is released under Creative Commons Attribution   *//
//*  (CC-BY) license : https://creativecommons.org/licenses/by/4.0/.        *//
//*                                                                         *//
//*  Based on 6502-monitor.ino by Ben Eater: https://eater.net/6502         *//
//*                                                                         *//
//***************************************************************************//


constexpr size_t NUM_ADDRESS_PINS = 16;
constexpr size_t NUM_DATA_BUS_PINS = 8;

constexpr uint8_t CLOCK_PIN = 2;
constexpr uint8_t READ_WRITE_PIN = 3;
constexpr uint8_t SYNC_PIN = 4;
const uint8_t ADDRESS_PIN[NUM_ADDRESS_PINS] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const uint8_t DATA_BUS_PIN[NUM_DATA_BUS_PINS] = {39, 41, 43, 45, 47, 49, 51, 53};

void setup() {

  for(size_t i = 0; i < NUM_ADDRESS_PINS; ++i) {
    pinMode(ADDRESS_PIN[i], INPUT);
  }

  for(size_t i = 0; i < NUM_DATA_BUS_PINS; ++i) {
    pinMode(DATA_BUS_PIN[i], INPUT);
  }

  pinMode(CLOCK_PIN, INPUT);
  pinMode(READ_WRITE_PIN, INPUT);
  pinMode(SYNC_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), onClock, RISING);
  Serial.begin(57600);
}

void
onClock(void) {

  uint16_t address = 0;
  int bit;
  for(size_t i = 0; i < NUM_ADDRESS_PINS; i++) {
    bit = digitalRead(ADDRESS_PIN[i]);
    Serial.print(bit);
    address = (address << 1) + bit;
  }

  Serial.print("  ");

  uint8_t data = 0;
  for(int i = 0; i < NUM_DATA_BUS_PINS; i++) {
    bit = digitalRead(DATA_BUS_PIN[i]) == HIGH ? 1 : 0;
    Serial.print(bit);
    data = (data << 1) + bit;
  }

  char readWrite = digitalRead(READ_WRITE_PIN) == HIGH ? 'r': 'W';
  char fetch = digitalRead(SYNC_PIN) == HIGH ? 'f' : '-';
  const char* opcode = "---";

  // Decode the instuction opcode if we are fetching
  //
  if('f' == fetch) {
    opcode = lookupOpcodeStr(data);
  }

  char output[64];
  sprintf(output, "  %04x %c %02x %s %c", address, readWrite ,data, opcode, fetch);
  Serial.println(output);
}

const char* 
lookupOpcodeStr(const uint8_t& opcode) {
  
  const char* opcodeStr = nullptr;
  
  switch(opcode) {
      case 0xea:
        opcodeStr = "NOP";
        break;
      case 0xa9:
        opcodeStr = "LDA";
        break;
      case 0x8d:
        opcodeStr = "STA";
        break;
      default:
        opcodeStr = "???";
        break;
    };

    return opcodeStr;
}

void
loop(void) {
  // do nothing
}
