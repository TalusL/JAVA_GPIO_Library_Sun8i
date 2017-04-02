#include "GPIO_Sun8i_GPIOCtrl.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>



#define SW_PORTC_IO_BASE 0x01c20800
#define GPIO_BANK(pin)	((pin) >> 5)
#define GPIO_CFG_INDEX(pin)	(((pin) & 0x1F) >> 3)
#define GPIO_CFG_OFFSET(pin)	((((pin) & 0x1F) & 0x7) << 2)
#define GPIO_NUM(pin)	((pin) & 0x1F)
#define GPIO_PUL_INDEX(pin)	(((pin) & 0x1F )>> 4)
#define GPIO_PUL_OFFSET(pin)	(((pin) & 0x0F) << 1)

unsigned int SUNXI_IO_BASE = 0;

struct sunxi_gpio {
    unsigned int cfg[4];
    unsigned int dat;
    unsigned int drv[2];
    unsigned int pull[2];
};

struct sunxi_gpio_int {
    unsigned int cfg[3];
    unsigned int ctl;
    unsigned int sta;
    unsigned int deb;
};

struct sunxi_gpio_reg {
    struct sunxi_gpio gpio_bank[9];
    unsigned char res[0xbc];
    struct sunxi_gpio_int gpio_int;
};

JNIEXPORT jint JNICALL Java_GPIO_Sun8i_GPIOCtrl_gpio_1init(JNIEnv * jniEnv, jobject job){
    int fd;
    unsigned int addr_start, addr_offset, PageSize, PageMask;
    void *pc;

    fd = open("/dev/mem", O_RDWR);
    if (fd < 0)
        return -1;

    PageSize = sysconf(_SC_PAGESIZE);
    PageMask = (~(PageSize - 1));

    addr_start = SW_PORTC_IO_BASE & PageMask;
    addr_offset = SW_PORTC_IO_BASE & ~PageMask;

    pc = (void *)mmap(0, PageSize * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr_start);

    if (pc == MAP_FAILED)
        return -1;

    SUNXI_IO_BASE = (unsigned int) pc;
    SUNXI_IO_BASE += addr_offset;

    close(fd);
    return 0;
}

JNIEXPORT jint JNICALL Java_GPIO_Sun8i_GPIOCtrl_gpio_1setcfg(JNIEnv * jniEnv, jobject job, jint pin, jint p1){

    unsigned int cfg;
    unsigned int bank = GPIO_BANK(pin);
    unsigned int index = GPIO_CFG_INDEX(pin);
    unsigned int offset = GPIO_CFG_OFFSET(pin);

    if(SUNXI_IO_BASE == 0)
        return -1;

    struct sunxi_gpio *pio = &((struct sunxi_gpio_reg *) SUNXI_IO_BASE)->gpio_bank[bank];

    cfg = *(&pio->cfg[0] + index);
    cfg &= ~(0xf << offset);
    cfg |= p1 << offset;

    *(&pio->cfg[0] + index) = cfg;

    return 0;

}

JNIEXPORT jint JNICALL Java_GPIO_Sun8i_GPIOCtrl_gpio_1getcfg(JNIEnv * jniEnv, jobject job, jint pin){
    unsigned int cfg;
    unsigned int bank = GPIO_BANK(pin);
    unsigned int index = GPIO_CFG_INDEX(pin);
    unsigned int offset = GPIO_CFG_OFFSET(pin);

    if (SUNXI_IO_BASE == 0)
        return -0;

    struct sunxi_gpio *pio = &((struct sunxi_gpio_reg *) SUNXI_IO_BASE)->gpio_bank[bank];
    cfg = *(&pio->cfg[0] + index);
    cfg >>= offset;
    return (cfg & 0xf);
}

JNIEXPORT jint JNICALL Java_GPIO_Sun8i_GPIOCtrl_gpio_1output(JNIEnv * jniEnv, jobject job, jint pin, jint p1){
    unsigned int bank = GPIO_BANK(pin);
    unsigned int num = GPIO_NUM(pin);

    if (SUNXI_IO_BASE == 0)
        return -1;

    struct sunxi_gpio *pio = &((struct sunxi_gpio_reg *) SUNXI_IO_BASE)->gpio_bank[bank];

    if (p1)
        *(&pio->dat) |= 1 << num;
    else
        *(&pio->dat) &= ~(1 << num);

    return 0;
}

JNIEXPORT jint JNICALL Java_GPIO_Sun8i_GPIOCtrl_gpio_1pullup(JNIEnv * jniEnv, jobject job, jint pin, jint p1){
    unsigned int cfg;
    unsigned int bank = GPIO_BANK(pin);
    unsigned int index = GPIO_PUL_INDEX(pin);
    unsigned int offset = GPIO_PUL_OFFSET(pin);

    if (SUNXI_IO_BASE == 0)
        return -1;

    struct sunxi_gpio *pio =
            &((struct sunxi_gpio_reg *) SUNXI_IO_BASE)->gpio_bank[bank];

    cfg = *(&pio->pull[0] + index);
    cfg &= ~(0x3 << offset);
    cfg |= p1 << offset;

    *(&pio->pull[0] + index) = cfg;

    return 0;

}

JNIEXPORT jint JNICALL Java_GPIO_Sun8i_GPIOCtrl_gpio_1input(JNIEnv * jniEnv, jobject job, jint pin){
    unsigned int dat;
    unsigned int bank = GPIO_BANK(pin);
    unsigned int num = GPIO_NUM(pin);

    if (SUNXI_IO_BASE == 0)
        return -1;

    struct sunxi_gpio *pio = &((struct sunxi_gpio_reg *) SUNXI_IO_BASE)->gpio_bank[bank];

    dat = *(&pio->dat);
    dat >>= num;

    return (dat & 0x1);
}