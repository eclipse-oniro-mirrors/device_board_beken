#PREFIX = /opt/risc-v/nds32le-elf-mculib-v5/bin/riscv32-unknown-elf-
#PREFIX = riscv32-unknown-elf-
#PREFIX = $(CONFIG_TOOLCHAIN_PATH)/$(CONFIG_TOOLCHAIN_PREFIX)
TARGET = run_wifiiot_scons

PREFIX = riscv32-elf-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
AR = $(PREFIX)ar
OBJDUMP = $(PREFIX)objdump

BIN = $(CP) -O binary
ENCRYPT = ./encrypt
ENCRYPT_FLAG = 0 0 0 0 

BEKEN_CHIP_TYPE=bk7235

#maybe adjust
OUTPUT_DST_DIR ?= ../../../../../out/bk7235x/wifi_${BEKEN_CHIP_TYPE}
BK_SDK_DIR = ../../../../soc/beken/${BEKEN_CHIP_TYPE}/liteos_m/bk_sdk_armino
BK_PROJECT_CONFIG = build/harmony/${BEKEN_CHIP_TYPE}/armino_as_lib/${BEKEN_CHIP_TYPE}/config
BK_PROJECT_INCLUDE = build/harmony/${BEKEN_CHIP_TYPE}/armino_as_lib/include
BK_PROJECT_PART = build/harmony/${BEKEN_CHIP_TYPE}/armino_as_lib/${BEKEN_CHIP_TYPE}/libs
BK_LIB_DIR = $(BK_SDK_DIR)/$(BK_PROJECT_PART)
LITEOS_V = liteos_m_mst
BUILD_DIR = build

C_SOURCES = entry.c

OBJECTS = 
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

#C_INCLUDES = -I. -I${BK_SDK_DIR}/$(BK_PROJECT_CONFIG) -I${BK_SDK_DIR}/middleware/soc/bk7235/soc -I${BK_SDK_DIR}/middleware/soc/bk7235/hal -I${BK_SDK_DIR}/include -I${BK_SDK_DIR}/include/arch/compiler -I${BK_SDK_DIR}/components/bk_common/include -I${BK_SDK_DIR}/components/release -I${BK_SDK_DIR}/middleware/driver -I${BK_SDK_DIR}/middleware/driver/include -I${BK_SDK_DIR}/middleware/driver/include/bk_private -I${BK_SDK_DIR}/middleware/driver/include/bk_private/legacy -I${BK_SDK_DIR}/middleware/driver/common -I${BK_SDK_DIR}/middleware/driver/pwm -I${BK_SDK_DIR}/middleware/driver/rc_beken -I${BK_SDK_DIR}/middleware/driver/flash -I${BK_SDK_DIR}/middleware/driver/rw_pub -I${BK_SDK_DIR}/middleware/driver/uart -I${BK_SDK_DIR}/middleware/driver/sys_ctrl -I${BK_SDK_DIR}/middleware/driver/gpio -I${BK_SDK_DIR}/middleware/driver/general_dma -I${BK_SDK_DIR}/middleware/driver/icu -I${BK_SDK_DIR}/middleware/driver/i2c -I${BK_SDK_DIR}/middleware/driver/sdcard -I${BK_SDK_DIR}/middleware/driver/saradc -I${BK_SDK_DIR}/middleware/driver/pmu -I${BK_SDK_DIR}/middleware/driver/mailbox -I${BK_SDK_DIR}/middleware/driver/spinlock -I${BK_SDK_DIR}/middleware/driver/touch -I${BK_SDK_DIR}/middleware/driver/sbc -I${BK_SDK_DIR}/middleware/driver/rtc -I${BK_SDK_DIR}/middleware/driver/i2s -I${BK_SDK_DIR}/middleware/driver/fft -I${BK_SDK_DIR}/middleware/compal/common_io/include -I${BK_SDK_DIR}/middleware/driver/spi -I${BK_SDK_DIR}/middleware/driver/jpeg_enc -I${BK_SDK_DIR}/middleware/driver/bk7235 -I${BK_SDK_DIR}/components/bk_rtos/include -I${BK_SDK_DIR}/components/bk_rtos/${LITEOS_V} -I${BK_SDK_DIR}/middleware/soc/common/soc/include -I${BK_SDK_DIR}/middleware/soc/common/hal/include -I${BK_SDK_DIR}/components/app/video_work -I${BK_SDK_DIR}/components/app/net_work -I${BK_SDK_DIR}/components/bk_wifi/include -I${BK_SDK_DIR}/components/bk_wifi/include/bk_private -I${BK_SDK_DIR}/components/bk_wifi/include/bk_private/legacy -I${BK_SDK_DIR}/components/bk_netif/include -I${BK_SDK_DIR}/components/bk_cli/include -I${BK_SDK_DIR}/components/bk_cli/include/bk_private -I${BK_SDK_DIR}/properties/modules/aec -I${BK_SDK_DIR}/components/utf8 -I${BK_SDK_DIR}/components/temp_detect -I${BK_SDK_DIR}/components/user_driver/include/bk_private -I${BK_SDK_DIR}/components/bk_system/soc/bk7231n -I${BK_SDK_DIR}/components/http -I${BK_SDK_DIR}/middleware/arch/riscv -I${BK_SDK_DIR}/middleware/arch/riscv/include -I${BK_SDK_DIR}/middleware/arch/riscv/include/bk_private -I${BK_SDK_DIR}/components/bk_startup/liteos_m -I${BK_SDK_DIR}/components/easy_flash -I${BK_SDK_DIR}/components/easy_flash/inc -I${BK_SDK_DIR}/components/easy_flash/port -I${BK_SDK_DIR}/components/base64 -I${BK_SDK_DIR}/components/bk_init/include -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/config -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/components/exchook -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/components/cpup -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/components/power -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/components/bounds_checking_function/include -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/components/backtrace -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/kernel/include -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/kernel/arch/include -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/targets/bk72xx -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/utils -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/kernel/arch/risc-v/riscv32/gcc -I${BK_SDK_DIR}/components/os_source/${LITEOS_V}/kernel/arch/risc-v/riscv32/gcc/asm -I${BK_SDK_DIR}/components/lwip_intf_v2_1/lwip-2.1.2/src -I${BK_SDK_DIR}/components/lwip_intf_v2_1/lwip-2.1.2/port -I${BK_SDK_DIR}/components/lwip_intf_v2_1/lwip-2.1.2/src/include -I${BK_SDK_DIR}/components/lwip_intf_v2_1/lwip-2.1.2/src/include/netif -I${BK_SDK_DIR}/components/lwip_intf_v2_1/lwip-2.1.2/src/include/lwip -I${BK_SDK_DIR}/components/bk_ps/include -I${BK_SDK_DIR}/properties/modules/btdm_5_2_rw/ble_pub/include/bk_private -I${BK_SDK_DIR}/components/bk_ate -I${BK_SDK_DIR}/components/bk_ate/include -I${BK_SDK_DIR}/middleware/boards/bk7235/vnd_cal -I${BK_SDK_DIR}/middleware/boards/bk7235/vnd_cal/include -I${BK_SDK_DIR}/components/demos/media/video/dvp -I${BK_SDK_DIR}/components/demos/media/video/spidma -I${BK_SDK_DIR}/components/demos/media/video/jpeg_dec -I${BK_SDK_DIR}/components/demos/media/video/uvc -I${BK_SDK_DIR}/components/demos/media/video/video_transfer -I${BK_SDK_DIR}/components/demos/media/video/image_save -I${BK_SDK_DIR}/components/at/include -I${BK_SDK_DIR}/middleware/compal/homekit -I${BK_SDK_DIR}/middleware/compal/matter -I${BK_SDK_DIR}/components/ali_mqtt/mqtt -I${BK_SDK_DIR}/components/ali_mqtt/mqtt/utils -I${BK_SDK_DIR}/components/ali_mqtt/mqtt/MQTTPacket -I${BK_SDK_DIR}/components/bk_adapter/include -I${BK_SDK_DIR}/components/bk_adapter/include/bk_private -I${BK_SDK_DIR}/components/key_handle -I${BK_SDK_DIR}/components/music_player/include/bk_private -I${BK_SDK_DIR}/components/paho-mqtt/client -I${BK_SDK_DIR}/components/paho-mqtt/client/src -I${BK_SDK_DIR}/components/paho-mqtt/packet/src -I${BK_SDK_DIR}/components/paho-mqtt/mqtt_ui -I${BK_SDK_DIR}/components/paho-mqtt/mqtt_ui/ssl_mqtt -I${BK_SDK_DIR}/components/paho-mqtt/mqtt_ui/tcp_mqtt -I${BK_SDK_DIR}/components/saradc_intf
C_INCLUDES = -I. -I${BK_SDK_DIR}/$(BK_PROJECT_CONFIG) -I${BK_SDK_DIR}/${BK_PROJECT_INCLUDE} -I${BK_SDK_DIR}/${BK_PROJECT_INCLUDE}/arch/compiler -I${BK_SDK_DIR}/${BK_PROJECT_INCLUDE}/extra

#WRAPS = -D_CUT_ED25519_ -D__freertos_irq_stack_top=_stack -DWIFI_BLE_COEXIST -DCONFIG_CMAKE=1 -DBK_MAC=1 -Wl,--gc-sections -nostdlib -Wl,-wrap,strlen -Wl,-wrap,malloc -Wl,-wrap,_malloc_r -Wl,-wrap,free -Wl,-wrap,_free_r -Wl,-wrap,zalloc -Wl,-wrap,calloc -Wl,-wrap,realloc  -Wl,-wrap,_realloc_r -Wl,-wrap,printf -Wl,-wrap,vsnprintf -Wl,-wrap,snprintf -Wl,-wrap,sprintf -Wl,-wrap,puts -Wl,-wrap,vprintf -Wl,-wrap,strncmp -Wl,-wrap,sprintf -Wl,--defsym,memcpy=memcpy_ss 
#WRAPS = -D_CUT_ED25519_ -D__freertos_irq_stack_top=_stack -DWIFI_BLE_COEXIST -DCONFIG_CMAKE=1 -DBK_MAC=1 -Wl,--gc-sections -nostdlib -Wl,-wrap,strlen -Wl,-wrap,malloc -Wl,-wrap,_malloc_r -Wl,-wrap,free -Wl,-wrap,_free_r -Wl,-wrap,zalloc -Wl,-wrap,calloc -Wl,-wrap,realloc  -Wl,-wrap,_realloc_r -Wl,-wrap,printf -Wl,-wrap,vsnprintf -Wl,-wrap,snprintf -Wl,-wrap,sprintf -Wl,-wrap,puts -Wl,-wrap,vprintf -Wl,-wrap,strncmp -Wl,-wrap,sprintf -Wl,--defsym,memcpy=memcpy_s 
WRAPS = -D_CUT_ED25519_ -D__freertos_irq_stack_top=_stack -DWIFI_BLE_COEXIST -DCONFIG_CMAKE=1 -DBK_MAC=1 -Wl,--gc-sections -nostdlib -Wl,-wrap,strlen -Wl,-wrap,malloc -Wl,-wrap,_malloc_r -Wl,-wrap,free -Wl,-wrap,_free_r -Wl,-wrap,zalloc -Wl,-wrap,calloc -Wl,-wrap,realloc  -Wl,-wrap,_realloc_r -Wl,-wrap,printf -Wl,-wrap,vsnprintf -Wl,-wrap,snprintf -Wl,-wrap,sprintf -Wl,-wrap,puts -Wl,-wrap,vprintf -Wl,--defsym,memcpy=memcpy_ss  -Wl,-wrap,strncmp 

CFLAGS = $(C_INCLUDES)
CFLAGS += $(WRAPS)

LDSCRIPT = ${BEKEN_CHIP_TYPE}.ld
#keep sync with build_sdk.sh
TARGET = ${BEKEN_CHIP_TYPE}_sdk
MAP_FILE = $(TARGET).map

LDSCRIPT_OTA = ${BEKEN_CHIP_TYPE}_ota.ld
#keep sync with pack.json
TARGET_OTA = ${BEKEN_CHIP_TYPE}_ota
MAP_FILE_OTA = $(TARGET_OTA).map

EXTRA_FLAGS = $(WRAPS)
LINK_FLAGS = 

#STD_LIBS = -lkernel -lmusl-c -lmusl-m -lm  -lgcc  -lc  -lgloss  -lm  -lgcc  -lc  -lgloss
#STD_LIBS = -lkernel -lm  -lgcc  -lc  -lgloss  -lm  -lgcc  -lc  -lgloss
STD_LIBS = -lkernel -lsdk_adapter -lm  -lgcc  -lc  -lm  -lgcc  -lc
#STD_LIBS = 

ARMINO_LIBS_DIR = $(BK_LIB_DIR)
ARMINO_LIBS1 = $(shell ./get_libs.sh $(ARMINO_LIBS_DIR))
ARMINO_DEP1 = $(shell ls $(ARMINO_LIBS_DIR))
ARMINO_DEP = $(addprefix $(ARMINO_LIBS_DIR)/, $(ARMINO_DEP1))
UNWANTED_ARMINO_LIBS = -los_source
ARMINO_LIBS = $(filter-out $(UNWANTED_ARMINO_LIBS), $(ARMINO_LIBS1))

PREBUILT_LIBS = -L$(ARMINO_LIBS_DIR) $(ARMINO_LIBS)

OHOS_LIBS1 = $(shell ./get_libs.sh $(OUTPUT_DST_DIR)/libs)
OHOS_DEP1 = $(shell ls $(OUTPUT_DST_DIR)/libs)
OHOS_DEP = $(addprefix $(OUTPUT_DST_DIR)/libs/, $(OHOS_DEP1))
#UNWANTED_LIBS = -lmusl-c -lmusl-m -liccarm-c -lkernel
UNWANTED_LIBS = -lkernel -lsdk_adapter -lmusl-c -lmusl-m -lmbedtls
OHOS_LIBS = $(filter-out $(UNWANTED_LIBS), $(OHOS_LIBS1))

SYS_LIBS = 
SYS_LIBS += -L$(OUTPUT_DST_DIR)/libs
SYS_LIBS += -Wl,--whole-archive $(OHOS_LIBS) -Wl,--no-whole-archive
#SYS_LIBS += -L$(SDK_LIBS_DIR) -L$(EXTRA_LIBS_DIR) $(SDK_LIBS) $(EXTRA_LIBS) $(SDK_LIBS) $(EXTRA_LIBS)

.PHONY: all
#all : $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET_OTA).bin
#all : $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).bin
all : $(BUILD_DIR) $(BUILD_DIR)/$(TARGET_OTA).bin

$(BUILD_DIR)/$(TARGET).bin : $(BUILD_DIR)/$(TARGET).elf
	$(BIN) $< $@
	$(OBJDUMP) -d $< > $(BUILD_DIR)/$(TARGET).asm
	$(ENCRYPT) $@ $(ENCRYPT_FLAG)
	@echo please burn the file : $(BUILD_DIR)/$(TARGET)_crc.bin

$(BUILD_DIR)/$(TARGET).elf : $(BK_LIB_DIR)/libbk7235.a $(OBJECTS) $(OHOS_DEP)
	@echo "building ${BEKEN_CHIP_TYPE} sdk..."
	@echo "PREFIX= ${PREFIX} "
	$(CC) $(EXTRA_FLAGS) $(OBJECTS) -o $@ $(SYS_LIBS) -Wl,--start-group $(PREBUILT_LIBS) -Wl,--cref -Wl,--Map=$(BUILD_DIR)/$(MAP_FILE) -fno-rtti -fno-lto -T$(LDSCRIPT) -Wl,--end-group $(STD_LIBS)

$(BUILD_DIR)/$(TARGET_OTA).bin : $(BUILD_DIR)/$(TARGET_OTA).elf
	$(BIN) $< $@
	$(OBJDUMP) -d $< > $(BUILD_DIR)/$(TARGET_OTA).asm
	(cd $(BUILD_DIR) && ../beken_packager ../pack.json )

#diff : two OTA suffix
$(BUILD_DIR)/$(TARGET_OTA).elf : $(BK_LIB_DIR)/libbk7235.a $(OBJECTS) $(OHOS_DEP)
	$(CC) $(EXTRA_FLAGS) $(OBJECTS) -o $@ $(SYS_LIBS) -Wl,--start-group $(PREBUILT_LIBS) -Wl,--cref -Wl,--Map=$(BUILD_DIR)/$(MAP_FILE_OTA) -fno-rtti -fno-lto -T$(LDSCRIPT_OTA) -Wl,--end-group $(STD_LIBS)

$(BK_LIB_DIR)/libbk7235.a:
	@echo "building armino libs"
	rm -rf $(BK_SDK_DIR)/build/
	make -C $(BK_SDK_DIR) bk7235 PROJECT=harmony

$(BUILD_DIR)/%.o: adapter/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir $@

clean:
	@rm -fR $(BUILD_DIR)
	@mkdir $(BUILD_DIR)

xx : 
	echo $(OBJECTS)
