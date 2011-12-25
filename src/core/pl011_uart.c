#include "pl011_uart.h"
#include "platform.h"
#include "interrupts.h"
#include "macros.h"
#include "errno.h"

#define PL011_STACK_SIZE	256
#define PL011_PRIORITY		1
#define PL011_QUEUE_SIZE	80

#define UART_BASE	UART0_BASE
#define UART_IRQ	INTERRUPT_UART0
#define UART_CLK	14745600
#define BAUD_RATE	115200

/* UART Registers */
#define UART_DR		(UART_BASE + 0x00)
#define UART_RSR	(UART_BASE + 0x04)
#define UART_ECR	(UART_BASE + 0x04)
#define UART_FR		(UART_BASE + 0x18)
#define UART_IBRD	(UART_BASE + 0x24)
#define UART_FBRD	(UART_BASE + 0x28)
#define UART_LCRH	(UART_BASE + 0x2c)
#define UART_CR		(UART_BASE + 0x30)
#define UART_IMSC	(UART_BASE + 0x38)
#define UART_MIS	(UART_BASE + 0x40)
#define UART_ICR	(UART_BASE + 0x44)

/* Flag register */
#define FR_RXFE		0x10	/* Receive FIFO empty */
#define FR_TXFF		0x20	/* Transmit FIFO full */

/* Masked interrupt status register */
#define MIS_RX		0x10	/* Receive interrupt */
#define MIS_TX		0x20	/* Transmit interrupt */

/* Interrupt clear register */
#define ICR_RX		0x10	/* Clear receive interrupt */
#define ICR_TX		0x20	/* Clear transmit interrupt */

/* Line control register (High) */
#define LCRH_WLEN8	0x60	/* 8 bits */
#define LCRH_FEN	0x10	/* Enable FIFO */

/* Control register */
#define CR_UARTEN	0x0001	/* UART enable */
#define CR_TXE		0x0100	/* Transmit enable */
#define CR_RXE		0x0200	/* Receive enable */

/* Interrupt mask set/clear register */
#define IMSC_RX		0x10	/* Receive interrupt mask */
#define IMSC_TX		0x20	/* Transmit interrupt mask */

pl011_config_t _pl011_default_config = {19200,8,0,1};

#define PL011_BUFFER_SIZE 256
#define WRAPPING_INCREMENT(x,y) (((x) + 1) % (y))

void pl011_irq(void);

void pl011_init() {
	pl011_configure(&_pl011_default_config);
	irq_enable(INTERRUPT_UART0, &pl011_irq);
}

void pl011_putc(uint8_t c) {
  while (read32(UART_FR) & FR_TXFF) DMB;
  write32(UART_DR, c);
}

void pl011_puts(uint8_t * data, size_t count) {
  for (int i = 0; i < count; i++) {
    pl011_putc(data[i]);
  }
}


void pl011_irq(void) {
	uint32_t c;
	uint32_t mis;

	mis = read32(UART_MIS);

	if (mis & MIS_RX) {
		/* Clear interrupt status */
		write32(UART_ICR, ICR_RX);
		
		while (read32(UART_FR) & FR_RXFE)
			DMB;
		do {
			c = read32(UART_DR);
		} while ((read32(UART_FR) & FR_RXFE) == 0);
	}	
}



uint32_t pl011_configure(pl011_config_t * config) {
		// Stop the uart itself
		write32(UART_CR, 0L);
		write32(UART_ICR, 0x07ff);
		
		// Set up the baud rate
		uint32_t divider, remainder, fraction;
		divider = UART_CLK / (16 * config->_baud);
		remainder = UART_CLK % (16 * config->_baud);
		fraction = (8 * remainder / config->_baud) >> 1;
		fraction += (8 * remainder / config->_baud) & 1;
		write32(UART_IBRD, divider);
		write32(UART_FBRD, fraction);
		
		// Data bits
		uint32_t format_config = LCRH_FEN;
		switch (config->_data_bits) {
			case 5:
			break;
			case 6:
			format_config |= (0x01 << 5);
			break;
			case 7:
			format_config |= (0x02 << 5);
			break;
			case 8:
			format_config |= (0x03 << 5);
			break;
			default:
			return EINVAL;
		}
		
		// Parity
		switch (config->_parity) {
			case 0:	// No parity checking
			break;
			case 1:	// Even parity
			format_config |= (0x03 << 1);
			break;
			case 255:	// Odd parity
			format_config |= (0x01 << 1);
			break;
			default:
			return EINVAL;
		}
		
		// Stop bits
		switch(config->_stop_bits) {
			case 1:
			break;
			case 2:
			format_config |= (0x01 << 3);
			break;
			default:
			return EINVAL;
		}
		// Finalise the configuration
		write32(UART_LCRH, format_config);
		// Enable UART
		write32(UART_CR, (CR_RXE | CR_TXE | CR_UARTEN));
		/* Enable RX interrupt */
		write32(UART_IMSC, (IMSC_RX));
		return 0;
}

// See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0196g/index.html

#define DMA_REG_BASE                       0x10130000
#define DMA_REG_CONFIG                     (DMA_REG_BASE + 0x030)

#define DMA_REG_CONFIG_E                   0x1

#define DMA_REG_CHAN0_CONFIG               (DMA_REG_BASE + 0x110)
#define DMA_REG_CHAN0_CONTROL              (DMA_REG_BASE + 0x10C)

#define DMA_REG_CHAN_CONTROL_I             (0x1 << 31)
#define DMA_REG_CHAN_CONTROL_DI            (0x1 << 27)
#define DMA_REG_CHAN_CONTROL_SI            (0x1 << 26)
#define DMA_REG_CHAN_CONTROL_PROT_MASK     (0b1111)
#define DMA_REG_CHAN_CONTROL_PROT_SHIFT    (28)
#define DMA_REG_CHAN_CONTROL_D             (0x1 << 25)
#define DMA_REG_CHAN_CONTROL_S             (0x1 << 24)
#define DMA_REG_CHAN_CONTROL_DWIDTH_MASK   (0b111)
#define DMA_REG_CHAN_CONTROL_DWIDTH_SHIFT  (21)
#define DMA_REG_CHAN_CONTROL_SWIDTH_MASK   (0b111)
#define DMA_REG_CHAN_CONTROL_SWIDTH_SHIFT  (18)
#define DMA_REG_CHAN_CONTROL_SBURST_MASK   (0b111)
#define DMA_REG_CHAN_CONTROL_SBURST_SHIFT  (15)
#define DMA_REG_CHAN_CONTROL_DBURST_MASK   (0b111)
#define DMA_REG_CHAN_CONTROL_DBURST_SHIFT  (12)
#define DMA_REG_CHAN_CONTROL_TRANS_MASK    (0b11111111111)
#define DMA_REG_CHAN_CONTROL_TRANS_SHIFT   (0)

#define DMA_REG_CHAN_CONFIG_HALT         (0x1 << 17)     
#define DMA_REG_CHAN_CONFIG_ACTIVE       (0x1 << 16)     
#define DMA_REG_CHAN_CONFIG_LOCK         (0x1 << 16)     
#define DMA_REG_CHAN_CONFIG_ITC          (0x1 << 15) 
#define DMA_REG_CHAN_CONFIG_IE           (0x1 << 14)
#define DMA_REG_CHAN_CONFIG_MEM_MEM_DMA  0b000
#define DMA_REG_CHAN_CONFIG_MEM_PER_DMA  0b001
#define DMA_REG_CHAN_CONFIG_PER_MEM_DMA  0b010
#define DMA_REG_CHAN_CONFIG_PER_PER_DMA  0b011
#define DMA_REG_CHAN_CONFIG_PER_PER_DST  0b100
#define DMA_REG_CHAN_CONFIG_MEM_PER_DST  0b101
#define DMA_REG_CHAN_CONFIG_PER_MEM_SRC  0b110
#define DMA_REG_CHAN_CONFIG_PER_PER_SRC  0b111
#define DMA_REG_CHAN_SRC_SHIFT           0x1
#define DMA_REG_CHAN_DST_SHIFT           0x6
#define DMA_REG_CHAN_SRC_MASK            0b111
#define DMA_REG_CHAN_DST_MASK            0b111
#define DMA_REG_CHAN_CONFIG_E            0x1

char pl011_output_buffer[1024]; // should be enough

void dma_enable() {
  uint32_t dma_status = read32(DMA_REG_CONFIG);
  write32(DMA_REG_CONFIG, dma_status | DMA_REG_CONFIG_E);
};

void dma_config_chan_per_mem(char *buf, uint32_t src) {
  // not finished yet

  uint32_t status = read32(DMA_REG_CHAN0_CONFIG);
  status = 
    DMA_REG_CHAN_CONFIG_E | 
    DMA_REG_CHAN_CONFIG_MEM_PER_DMA |
    ((src & DMA_REG_CHAN_DST_MASK) << DMA_REG_CHAN_DST_SHIFT)
    ;
};

void pl011_dma_puts(uint8_t * data, size_t count) {
  // not finished yet


  // silently truncate input (this is experimental code)
  if ( count > sizeof(pl011_output_buffer) ) {
    count = sizeof(pl011_output_buffer);
  };

  dma_enable();

  for (uint32_t i = 0; i < count; i++) {
    pl011_putc(data[i]);
  }
}
