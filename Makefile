#--------------------------------------Makefile-------------------------------------
BUILD_DIR = ./object
SRC_DIR = ./src

CFILES = $(wildcard $(SRC_DIR)/*.c)
OFILES = $(CFILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib  #-nostartfiles 
LDFLAGS = -nostdlib #-nostartfiles

all: clean kernel8.img run

$(BUILD_DIR)/boot.o: $(SRC_DIR)/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	aarch64-none-elf-gcc $(CFLAGS) -ffreestanding  -c $< -o $@

kernel8.img: $(BUILD_DIR)/boot.o $(OFILES)
	aarch64-none-elf-ld $(LDFLAGS) $(BUILD_DIR)/boot.o $(OFILES) -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img

clean:
	rm -f *.img $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/*.o

run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio

test: all run