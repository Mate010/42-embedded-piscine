MCU			= atmega328p
CC			= avr-gcc
F_CPU		= 16000000UL
OBJCOPY 	= avr-objcopy
AVRDUDE 	= avrdude
CFLAGS		= -nostdlib -Wall -Wextra -Werror -O1 -mmcu=$(MCU) -DF_CPU=$(F_CPU)

PORT = /dev/ttyUSB0
BAUD_RATE = 115200
PROGRAMMER = arduino


SRC = main.c
OBJS = $(SRC:.c=.elf)
HEX = $(SRC:.c=.hex)

all: hex flash

hex: $(OBJS)
	avr-objcopy -O binary $(OBJS) $(basename $(SRC)).bin
	avr-objcopy -I ihex $(OBJS) $(basename $(SRC)).hex

$(OBJS): $(SRC)
	@$(CC) $(CFLAGS) -o $(OBJS) $(SRC)

flash: $(OBJS) 
	$(AVRDUDE) -c $(PROGRAMMER) -p atmega328p -P $(PORT) $(BAUD_RATE) -U flash:w:main.hex

clean:
	rm -f main.bin main.hex main.o
