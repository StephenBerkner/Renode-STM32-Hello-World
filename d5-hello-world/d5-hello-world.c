#include "api.h"
#include "api-asm.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

// Initialize PA.2
static void gpio_init(void){
	// Set up PA.2 as an alternate function pin
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);
	
	// Set up PA.2 as USART2 Tx (alternate function 7)
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

// Set up peripheral clocks
static void clock_init(void){
	// Enable port A peripheral clock for usart
	rcc_periph_clock_enable(RCC_GPIOA);

	// Enable usart2 perhipheral clock
	rcc_periph_clock_enable(RCC_USART2);
}

// Intialize USART2
static void usart_init(void){
	// Set serial communication over USART2 with 115200 baud rate, 8 bit data, 1 stop bit,
	// no flow control or parity, and only transmission mode since we are only transmisison
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	// Enable usart2 now that it has been configured
	usart_enable(USART2);
}



// Override _write syscall to redirect output to USART2 
int _write(int file, char *ptr, int len){
	int i;
	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				usart_send_blocking(USART2, '\r');
			}
			usart_send_blocking(USART2, ptr[i]);
		}
		return i;
	}

	errno = EIO;
	return -1;
}

int main(void) {
	// Initialize clock, gpio, and usart
	clock_init();
	gpio_init();
	usart_init();

	// The preceding new lines are needed so the text is visible below the RENODE logo
	printf("\n\n\n\n\nHello world from Dojo Five!\n");

	// Infinite loop and a return statement to keep the compiler happy
	while (1) {}

	return 0;
}
