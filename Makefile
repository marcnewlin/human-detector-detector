OPENOCD=./openocd/src/openocd
OPENOCD_SCRIPTS=./openocd/tcl
GCC=arm-none-eabi-gcc

DRIVERS=./at32-sdk/libraries/drivers
MIDDLEWARES=./at32-sdk/middlewares
CMSIS=./at32-sdk/libraries/cmsis

firmware:
	mkdir -p build
	arm-none-eabi-gcc -I$(CMSIS)/cm4/device_support \
										-I$(CMSIS)/cm4/core_support \
										-I$(DRIVERS)/inc \
										-I./src \
										-I$(MIDDLEWARES)/usbd_drivers/inc \
										-I$(MIDDLEWARES)/usbd_class/cdc \
										-DAT32F403ACGT7 \
										-O3 \
	                  --specs=nosys.specs \
	                  -mcpu=cortex-m4 \
										-march=armv7e-m \
										-mfpu=fpv4-sp-d16 \
										-mfloat-abi=hard \
										-fno-common \
										$(CMSIS)/cm4/device_support/system_at32f403a_407.c \
										$(CMSIS)/cm4/device_support/startup/gcc/startup_at32f403a_407.s \
										-T$(CMSIS)/cm4/device_support/startup/gcc/linker/AT32F403AxG_FLASH.ld \
										$(MIDDLEWARES)/usbd_drivers/src/usbd_core.c \
										$(MIDDLEWARES)/usbd_drivers/src/usbd_sdr.c \
										$(MIDDLEWARES)/usbd_drivers/src/usbd_int.c \
										$(MIDDLEWARES)/usbd_class/cdc/cdc_class.c \
										$(MIDDLEWARES)/usbd_class/cdc/cdc_desc.c \
										$(DRIVERS)/src/at32f403a_407_acc.c \
										$(DRIVERS)/src/at32f403a_407_gpio.c \
										$(DRIVERS)/src/at32f403a_407_adc.c \
										$(DRIVERS)/src/at32f403a_407_dma.c \
										$(DRIVERS)/src/at32f403a_407_crm.c \
										$(DRIVERS)/src/at32f403a_407_misc.c \
										$(DRIVERS)/src/at32f403a_407_usb.c \
										src/at32f403a_407_clock.c \
										src/at32f403a_407_int.c \
										src/at32f403a_407_board.c \
										src/main.c \
										-o build/firmware.elf
		arm-none-eabi-objcopy -O binary build/firmware.elf build/firmware.bin

flash:
	cd openocd/tcl && ./openocd -f interface/jlink.cfg \
						 -c "transport select swd" \
 						 -f target/stm32f1x.cfg \
 						 -c "init" \
 						 -c "reset halt" \
 						 -c "flash erase_address unlock 0x08000000 0x8000" \
 						 -c "flash write_bank 0 ../../build/firmware.bin" \
 						 -c "reset run" \
						 -c "exit"

reset:
	cd openocd/tcl && ./openocd -f interface/jlink.cfg \
  	         -c "transport select swd" \
  					 -f target/stm32f1x.cfg \
  					 -c "init" \
  					 -c "reset run" \
 						 -c "exit"
