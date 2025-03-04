#ifndef OSPI_COMMON_H_
#define OSPI_COMMON_H_

void ospi_reset(void);
void oclk_change(bsp_clocks_octaclk_div_t divider);
void ospi_change_to_max_speed(void);

#endif /* OSPI_COMMON_H_ */
