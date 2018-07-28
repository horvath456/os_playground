#include "common.h"
#include "console.h"

void init(void) {
    uint8 hw[] = "Hello World!";

    k_clrscr();
    k_print(hw, 0x07);
}
