#ifndef I2C_H
#define I2C_H

void i2c_init(void);
void i2c_start(void);
void i2c_write(unsigned char data);
// void i2c_read(void);
void print_hex_value(uint8_t c);
void i2c_stop(void);
uint8_t i2c_ack(void);
uint8_t i2c_nack(void);
#endif