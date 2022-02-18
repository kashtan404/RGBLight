void readSettings() {
  eeprom_read_block((void*)&modeSettings, MODE_ADDR, sizeof(modeSettings));
}
void writeSettings() {
  eeprom_write_block((void*)&modeSettings, MODE_ADDR, sizeof(modeSettings));
}
