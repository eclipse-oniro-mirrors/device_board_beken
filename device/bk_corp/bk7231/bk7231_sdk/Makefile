LITEOSTOPDIR := ../../../../kernel/liteos_m/
LITEOSTOPDIR := $(realpath $(LITEOSTOPDIR))


C_INCLUDES    = -I$(LITEOSTOPDIR)/utils \
                 -I$(LITEOSTOPDIR)/kal/cmsis \
                 -I$(LITEOSTOPDIR)/kernel/include \
                 -I$(LITEOSTOPDIR)/components/cpup \
                 -I$(LITEOSTOPDIR)/../../third_party/bounds_checking_function/include

C_INCLUDES    += -I$(LITEOSTOPDIR)/kernel/arch/include \
                 -I$(LITEOSTOPDIR)/kernel/arch/arm/arm9/gcc

C_INCLUDES    += -I$(LITEOSTOPDIR)/targets/bk7231

BUILD_DIR = build

OUTPUT_DST_DIR ?= ../../../../out/bk7231/wifi_iot

C_SOURCES = 
#C_SOURCES += ohos/ohos_entry.c ohos/printf.c
#C_SOURCES += ohos/printf.c

EXTRA_LIBS_DIR = extra_libs
EXTRA_LIBS = $(shell ./get_libs.sh $(EXTRA_LIBS_DIR))
EXTRA_DEP1 = $(shell ls $(EXTRA_LIBS_DIR))
EXTRA_DEP = $(addprefix $(EXTRA_LIBS_DIR)/, $(EXTRA_DEP1))

SDK_LIBS_DIR = sdk_libs
SDK_LIBS = $(shell ./get_libs.sh $(SDK_LIBS_DIR))
SDK_DEP1 = $(shell ls $(SDK_LIBS_DIR))
SDK_DEP = $(addprefix $(SDK_LIBS_DIR)/, $(SDK_DEP1))

OHOS_LIBS = $(shell ./get_libs.sh $(OUTPUT_DST_DIR)/libs)
OHOS_DEP1 = $(shell ls $(OUTPUT_DST_DIR)/libs)
OHOS_DEP = $(addprefix $(OUTPUT_DST_DIR)/libs/, $(OHOS_DEP1))

SDK_OBJS_DIR = sdk_objs
SDK_OBJS1 = $(shell ls $(SDK_OBJS_DIR))
#SDK_OBJS = 
SDK_OBJS = $(addprefix $(SDK_OBJS_DIR)/, $(SDK_OBJS1))

LIBS = 
#LIBS += -L$(SDK_LIBS_DIR) -lbk7231_sdk $(SDK_LIBS) -lbk7231_sdk
LIBS += -L$(OUTPUT_DST_DIR)/libs
LIBS += -Wl,--whole-archive $(OHOS_LIBS) -Wl,--no-whole-archive
LIBS += -L$(SDK_LIBS_DIR) -L$(EXTRA_LIBS_DIR) $(SDK_LIBS) $(EXTRA_LIBS) $(SDK_LIBS) $(EXTRA_LIBS)
#LIBS += -lgcc

PREFIX = arm-none-eabi-

CFLAGS = $(C_INCLUDES)
CFLAGS += -g -mcpu=arm968e-s -march=armv5te -marm -mthumb-interwork -mlittle-endian -Os -std=c99 -ffunction-sections -Wall -Wno-format -Wno-unknown-pragmas -fsigned-char -fdata-sections -fno-strict-aliasing

LINK_FLAGS = 
LINK_FLAGS += -g -Wl,--gc-sections -marm -mthumb-interwork -mcpu=arm968e-s

EXTRA_FLAGS = 
EXTRA_FLAGS += -Wl,-wrap,printf -Wl,-wrap,vsnprintf -Wl,-wrap,snprintf -Wl,-wrap,sprintf -Wl,-wrap,puts
#EXTRA_FLAGS += -Wl,-wrap,malloc -Wl,-wrap,_malloc_r -Wl,-wrap,free -Wl,-wrap,_free_r -Wl,-wrap,zalloc -Wl,-wrap,calloc -Wl,-wrap,realloc  -Wl,-wrap,_realloc_r
EXTRA_FLAGS += -Wl,-wrap,malloc -Wl,-wrap,free -Wl,-wrap,zalloc -Wl,-wrap,calloc -Wl,-wrap,realloc

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
AR = $(PREFIX)ar
OBJDUMP = $(PREFIX)objdump

BIN = $(CP) -O binary -S
ENCRYPT = ./encrypt
ENCRYPT_FLAG = 0 0 0 0 

LDSCRIPT = bk7231.ld
#keep sync with build_sdk.sh
TARGET = bk7231_sdk
MAP_FILE = $(TARGET).map

LDSCRIPT_OTA = bk7231_ota.ld
#keep sync with pack.json
TARGET_OTA = bk7231_ota
MAP_FILE_OTA = $(TARGET_OTA).map

OBJECTS = 
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

.PHONY: all

#all: $(BUILD_DIR)/$(TARGET).bin
all: $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET_OTA).bin

$(BUILD_DIR)/$(TARGET).bin : $(BUILD_DIR)/$(TARGET).elf
	$(BIN) $< $@
	$(OBJDUMP) -d $< > $(BUILD_DIR)/$(TARGET).asm
	$(ENCRYPT) $@ $(ENCRYPT_FLAG)
	@echo please burn the file : $(BUILD_DIR)/$(TARGET)_crc.bin

$(BUILD_DIR)/$(TARGET).elf : $(OBJECTS) $(SDK_OBJS_DIR)/arch_main.o $(OHOS_DEP) $(SDK_DEP) $(EXTRA_DEP)
	@echo "building bk7231 sdk..."
	$(CC) $(OBJECTS) $(SDK_OBJS) $(EXTRA_FLAGS) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) $(LINK_FLAGS) -Xlinker -Map=$(BUILD_DIR)/$(MAP_FILE) -o $@

$(BUILD_DIR)/$(TARGET_OTA).bin : $(BUILD_DIR)/$(TARGET_OTA).elf
	$(BIN) $< $@
	$(OBJDUMP) -d $< > $(BUILD_DIR)/$(TARGET_OTA).asm
	(cd $(BUILD_DIR) && ../beken_packager ../pack.json )

$(BUILD_DIR)/$(TARGET_OTA).elf : $(OBJECTS) $(SDK_OBJS_DIR)/arch_main.o $(OHOS_DEP) $(SDK_DEP) $(EXTRA_DEP)
	$(CC) $(OBJECTS) $(SDK_OBJS) $(EXTRA_FLAGS) -specs=nano.specs -T$(LDSCRIPT_OTA) $(LIBS) $(LINK_FLAGS) -Xlinker -Map=$(BUILD_DIR)/$(MAP_FILE_OTA) -o $@

$(SDK_OBJS_DIR)/arch_main.o : $(SDK_LIBS_DIR)/libbk7231_sdk.a
	@echo "uppack libbk7231_sdk.a..."
	rm -rf $(SDK_OBJS_DIR)
	mkdir $(SDK_OBJS_DIR)
	cd $(SDK_OBJS_DIR) && $(AR) -x ../$(SDK_LIBS_DIR)/libbk7231_sdk.a

$(BUILD_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@


$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR $(BUILD_DIR)
	mkdir $(BUILD_DIR)

