MCU          = atmega328p
CC           = avr-gcc
F_CPU        = 16000000UL
OBJCOPY      = avr-objcopy
AVRDUDE      = avrdude
PORT         = /dev/ttyUSB0
BAUD_RATE    = 115200
PROGRAMMER   = arduino

CFLAGS       = -Wall -Wextra -Werror -mmcu=$(MCU) -DF_CPU=$(F_CPU) -O1

SRC          = main.c uart.c
# On transforme les .c en .o (fichiers objets)
OBJS         = $(SRC:.c=.o)
TARGET       = main

all: $(TARGET).hex flash

# Règle pour lier les objets et créer l'exécutable ELF
$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Règle pour compiler chaque fichier .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Extraction du HEX à partir du ELF
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Flashage (Note: on utilise -b pour le baud rate avec avrdude)
flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD_RATE) -U flash:w:$(TARGET).hex

clean:
	rm -f *.o *.elf *.hex

screen:
	screen $(PORT) $(BAUD_RATE)