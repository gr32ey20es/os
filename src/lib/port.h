#ifndef PORT_H
#define PORT_H

#define CRTC_ADDR_REG 0x03d4
#define CRTC_DATA_REG 0x03d5

uint8_t  port_in8 (uint16_t port);
uint16_t port_in16(uint16_t port);
uint32_t port_in32(uint16_t port);

void port_out8 (uint16_t port, uint8_t  data);
void port_out16(uint16_t port, uint16_t data);
void port_out32(uint16_t port, uint32_t data);

#endif
