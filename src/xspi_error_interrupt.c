#include "hal_data.h"

void xSPI_error_interrupt(void)
{
    __BKPT(0);
}
