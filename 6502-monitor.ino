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

const char* opcodeMatrix[] = {
/*         0      1      2     3    4     5       6      7       8     9       A     B       C      D      E      F */
/* 0 */  "BRK", "ORA", ""   , "", "TSB", "ORA", "ASL", "RMB0", "PHP", "ORA", "ASL", ""   , "TSB", "ORA", "ASL", "BBR0",
/* 1 */  "BPL", "ORA", "ORA", "", "TRB", "ORA", "ASL", "RMB1", "CLC", "ORA", "INC", ""   , "TRB", "ORA", "ASL", "BBR1",
/* 2 */  "JSR", "AND", ""   , "", "BIT", "AND", "ROL", "RMB2", "PLP", "AND", "ROL", ""   , "BIT", "AND", "ROL", "BBR2",
/* 3 */  "BMI", "AND", "AND", "", "BIT", "AND", "ROL", "RMB3", "SEC", "AND", "DEC", ""   , "BIT", "AND", "ROL", "BBR3",
/* 4 */  "RTI", "EOR", ""   , "", ""   , "EOR", "LSR", "RMB4", "PHA", "EOR", "LSR", ""   , "JMP", "EOR", "LSR", "BBR4",
/* 5 */  "BVC", "EOR", "EOR", "", ""   , "EOR", "LSR", "RMB5", "CLI", "EOR", "PHY", ""   , ""   , "EOR", "LSR", "BBR5",
/* 6 */  "RTS", "ADC", ""   , "", "STZ", "ADC", "ROR", "RMB6", "PLA", "ADC", "ROR", ""   , "JMP", "ADC", "ROR", "BBR6",
/* 7 */  "BVS", "ADC", "ADC", "", "STZ", "ADC", "ROR", "RMB7", "SEI", "ADC", "PLY", ""   , "JMP", "ADC", "ROR", "BBR7",
/* 8 */  "BRA", "STA", ""   , "", "STY", "STA", "STX", "SMB0", "DEY", "BIT", "TXA", ""   , "STY", "STA", "STX", "BBS0",
/* 9 */  "BCC", "STA", "STA", "", "STY", "STA", "STX", "SMB1", "TYA", "STA", "TXS", ""   , "STZ", "STA", "STZ", "BBS1",
/* A */  "LDY", "LDA", "LDX", "", "LDY", "LDA", "LDX", "SMB2", "TAY", "LDA", "TAX", ""   , "LDY", "LDA", "LDX", "BBS2",
/* B */  "BCS", "LDA", "LDA", "", "LDY", "LDA", "LDX", "SMB3", "CLV", "LDA", "TSX", ""   , "LDY", "LDA", "LDX", "BBS3",
/* C */  "CPY", "CMP", ""   , "", "CPY", "CMP", "DEC", "SMB4", "INY", "CMP", "DEX", "WAI", "CPY", "CMP", "DEC", "BBS4",
/* D */  "BNE", "CMP", "CMP", "", ""   , "CMP", "DEC", "SMB5", "CLD", "CMP", "PHX", "STP", ""   , "CMP", "DEC", "BBS5",
/* E */  "CPX", "SBC", ""   , "", "CPX", "SBC", "INC", "SMB6", "INX", "SBC", "NOP", ""   , "CPX", "SBC", "INC", "BBS6",
/* F */  "BEQ", "SBC", "SBC", "", ""   , "SBC", "INC", "SMB7", "SED", "SBC", "PLX", ""   , ""   , "SBC", "INC", "BBS7"};

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
  char output[32];

  if(digitalRead(SYNC_PIN) == HIGH) {
    // Decode the instruction opcode if we are fetching
    snprintf(output, sizeof(output), "  %04x %c %02x %s", address, readWrite, data, opcodeMatrix[data]);
  }
  else {
    snprintf(output, sizeof(output), "  %04x %c %02x", address, readWrite, data);
  }
  Serial.println(output);
}

void
loop(void) {
  // do nothing
}
