#include "hal_sys_param.h"
#include <securec.h>

static const char OHOS_PRODUCT_TYPE[] = {"wifi"};
static const char OHOS_MANUFACTURE[] = {"beken sz"};
static const char OHOS_BRAND[] = {"beken"};
static const char OHOS_MARKET_NAME[] = {"bk wifi iot"};
static const char OHOS_PRODUCT_SERIES[] = {"bk iot with wifi"};
static const char OHOS_PRODUCT_MODEL[] = {"wifi_iot"};
static const char OHOS_SOFTWARE_MODEL[] = {"bk sw"};
static const char OHOS_HARDWARE_MODEL[] = {"bk hw"};
static const char OHOS_HARDWARE_PROFILE[] = {"wifi:true,ble:true"};
static const char OHOS_BOOTLOADER_VERSION[] = {"bootloader_bk72xx_uart2_v1.0.8"};
static const char OHOS_SECURITY_PATCH_TAG[] = {"2021-07-15"};
static const char OHOS_ABI_LIST[] = {"kal,rtos_pub"};
static const char OHOS_SERIAL[] = {"1234567890"};

char* HalGetProductType(void)
{
    char* value = (char*)malloc(strlen(OHOS_PRODUCT_TYPE) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_PRODUCT_TYPE) + 1, OHOS_PRODUCT_TYPE) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetManufacture(void)
{
    char* value = (char*)malloc(strlen(OHOS_MANUFACTURE) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_MANUFACTURE) + 1, OHOS_MANUFACTURE) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetBrand(void)
{
    char* value = (char*)malloc(strlen(OHOS_BRAND) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_BRAND) + 1, OHOS_BRAND) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetMarketName(void)
{
    char* value = (char*)malloc(strlen(OHOS_MARKET_NAME) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_MARKET_NAME) + 1, OHOS_MARKET_NAME) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetProductSeries(void)
{
    char* value = (char*)malloc(strlen(OHOS_PRODUCT_SERIES) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_PRODUCT_SERIES) + 1, OHOS_PRODUCT_SERIES) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetProductModel(void)
{
    char* value = (char*)malloc(strlen(OHOS_PRODUCT_MODEL) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_PRODUCT_MODEL) + 1, OHOS_PRODUCT_MODEL) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetSoftwareModel(void)
{
    char* value = (char*)malloc(strlen(OHOS_SOFTWARE_MODEL) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_SOFTWARE_MODEL) + 1, OHOS_SOFTWARE_MODEL) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetHardwareModel(void)
{
    char* value = (char*)malloc(strlen(OHOS_HARDWARE_MODEL) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_HARDWARE_MODEL) + 1, OHOS_HARDWARE_MODEL) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetHardwareProfile(void)
{
    char* value = (char*)malloc(strlen(OHOS_HARDWARE_PROFILE) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_HARDWARE_PROFILE) + 1, OHOS_HARDWARE_PROFILE) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetSerial(void)
{
    char* value = (char*)malloc(strlen(OHOS_SERIAL) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_SERIAL) + 1, OHOS_SERIAL) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetBootloaderVersion(void)
{
    char* value = (char*)malloc(strlen(OHOS_BOOTLOADER_VERSION) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_BOOTLOADER_VERSION) + 1, OHOS_BOOTLOADER_VERSION) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetSecurityPatchTag(void)
{
    char* value = (char*)malloc(strlen(OHOS_SECURITY_PATCH_TAG) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_SECURITY_PATCH_TAG) + 1, OHOS_SECURITY_PATCH_TAG) != 0) {
        free(value);
        return NULL;
    }
    return value;
}

char* HalGetAbiList(void)
{
    char* value = (char*)malloc(strlen(OHOS_ABI_LIST) + 1);
    if (value == NULL) {
        return NULL;
    }
    if (strcpy_s(value, strlen(OHOS_ABI_LIST) + 1, OHOS_ABI_LIST) != 0) {
        free(value);
        return NULL;
    }
    return value;
}