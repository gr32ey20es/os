uint8_t port_in8(uint16_t port)
{
uint8_t data;

	__asm__(
		  "in %%dx, %%al"
		: "=a" (data)
		: "d"  (port)
	);

	return data;
}

uint16_t port_in16(uint16_t port)
{
uint16_t data;
	
	__asm__(
		  "in %%dx, %%ax"
		: "=a" (data)
		: "d"  (port)
	);

	return data;
}

void port_out8(uint16_t port, uint8_t data)
{
	__asm__(
		  "out %%al, %%dx"
		:
		: "a" (data), "d" (port)
	);
}
