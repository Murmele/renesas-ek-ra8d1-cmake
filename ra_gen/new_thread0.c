/* generated thread source file - do not edit */
#include "new_thread0.h"

#if 1
static StaticTask_t new_thread0_memory;
#if defined(__ARMCC_VERSION)           /* AC6 compiler */
static uint8_t
    new_thread0_stack[16 * 1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX
                                                      ".stack.thread")
        BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#else
static uint8_t
    new_thread0_stack[16 * 1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX
                                                      ".stack.new_thread0")
        BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif
#endif
TaskHandle_t new_thread0;
void new_thread0_create(void);
static void new_thread0_func(void *pvParameters);
void rtos_startup_err_callback(void *p_instance, void *p_data);
void rtos_startup_common_init(void);
/* I2C Communication Device */
#define RA_NOT_DEFINED (1)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device0_ctrl;

/* Lower level driver configuration */
const i2c_master_cfg_t g_comms_i2c_device0_lower_level_cfg =
{ .slave = 0x5D, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT, .p_callback = rm_comms_i2c_callback, };

const rm_comms_cfg_t g_comms_i2c_device0_cfg =
{ .semaphore_timeout = 0xFFFFFFFF, .p_lower_level_cfg = (void*) &g_comms_i2c_device0_lower_level_cfg, .p_extend =
          (void*) &g_comms_i2c_bus0_extended_cfg,
  .p_callback = comms_i2c_callback,
#if defined(NULL)
    .p_context          = NULL,
#else
  .p_context = (void*) &NULL,
#endif
        };

const rm_comms_instance_t g_comms_i2c_device0 =
{ .p_ctrl = &g_comms_i2c_device0_ctrl, .p_cfg = &g_comms_i2c_device0_cfg, .p_api = &g_comms_on_comms_i2c, };
ospi_b_instance_ctrl_t g_ospi_flash_ctrl;

static const spi_flash_erase_command_t g_ospi_flash_erase_command_list[] =
{
#if ((0x2121 > 0) && (4096 > 0))
  { .command = 0x2121, .size = 4096 },
#endif
#if ((0xDCDC > 0) && (262144 > 0))
  { .command = 0xDCDC, .size = 262144 },
#endif
#if (0x6060 > 0)
  { .command = 0x6060, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE },
#endif
        };

static ospi_b_timing_setting_t g_ospi_flash_timing_settings =
{ .command_to_command_interval = OSPI_B_COMMAND_INTERVAL_CLOCKS_2, .cs_pullup_lag =
          OSPI_B_COMMAND_CS_PULLUP_CLOCKS_NO_EXTENSION,
  .cs_pulldown_lead = OSPI_B_COMMAND_CS_PULLDOWN_CLOCKS_NO_EXTENSION };

#if !(0)

#if (1)
static const spi_flash_erase_command_t g_ospi_flash_high_speed_erase_command_list[] =
{
#if ((0x21DE > 0) && (4096 > 0))
  { .command = 0x21DE, .size = 4096 },
#endif
#if ((0xDC23 > 0) && (4096 > 0))
  { .command = 0xDC23, .size = 262144 },
#endif
#if (0x609F > 0)
  { .command = 0x609F, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE },
#endif
        };

static const ospi_b_table_t g_ospi_flash_high_speed_erase_command_table =
{ .p_table = (void*) g_ospi_flash_high_speed_erase_command_list, .length = (uint8_t) (
        sizeof(g_ospi_flash_high_speed_erase_command_list) / sizeof(g_ospi_flash_high_speed_erase_command_list[0])), };
#endif

const ospi_b_xspi_command_set_t g_ospi_flash_high_speed_command_set =
{ .protocol = SPI_FLASH_PROTOCOL_8D_8D_8D,
  .command_bytes = OSPI_B_COMMAND_BYTES_2,
  .read_command = 0xEE11,
  .page_program_command = 0x12ED,
  .write_enable_command = 0x06F9,
  .status_command = 0x05FA,
  .read_dummy_cycles = 10,
  .program_dummy_cycles = 0, /* Unused by OSPI Flash */
  .status_dummy_cycles = 3,
#if (1)
  .p_erase_commands = &g_ospi_flash_high_speed_erase_command_table,
#else
                .p_erase_commands = NULL, /* Use the erase commands specified in spi_flash_cfg_t */
             #endif
        };
#else
            extern ospi_b_xspi_command_set_t [];
            #endif

const ospi_b_table_t g_ospi_flash_command_set =
{
#if (0)
                .p_table = (void *),
                .length = 0,
            #else
  .p_table = (void*) &g_ospi_flash_high_speed_command_set,
  .length = 1,
#endif
        };

#if OSPI_B_CFG_DOTF_SUPPORT_ENABLE
            extern uint8_t g_ospi_dotf_iv[];
            extern uint8_t g_ospi_dotf_key[];

            static ospi_b_dotf_cfg_t g_ospi_dotf_cfg=
            {
                .key_type       = OSPI_B_DOTF_AES_KEY_TYPE_128,
                .format         = OSPI_B_DOTF_KEY_FORMAT_PLAINTEXT,
                .p_start_addr   = (uint32_t *)0x90000000,
                .p_end_addr     = (uint32_t *)0x90001FFF,
                .p_key          = (uint32_t *)g_ospi_dotf_key,
                .p_iv           = (uint32_t *)g_ospi_dotf_iv,
            };
            #endif

static const ospi_b_extended_cfg_t g_ospi_flash_extended_cfg =
{ .ospi_b_unit = 0, .channel = (ospi_b_device_number_t) 1, .data_latch_delay_clocks = 0x08, .p_timing_settings =
          &g_ospi_flash_timing_settings,
  .p_xspi_command_set = &g_ospi_flash_command_set, .p_autocalibration_preamble_pattern_addr = (uint8_t*) 0x90000000,
#if OSPI_B_CFG_DMAC_SUPPORT_ENABLE
                .p_lower_lvl_transfer                    = &RA_NOT_DEFINED,
            #endif
#if OSPI_B_CFG_DOTF_SUPPORT_ENABLE
                .p_dotf_cfg                              = &g_ospi_dotf_cfg,
            #endif
  .read_dummy_cycles = 0,
  .program_dummy_cycles = 0, /* Unused by OSPI Flash */
  .status_dummy_cycles = 0, };
const spi_flash_cfg_t g_ospi_flash_cfg =
{ .spi_protocol = SPI_FLASH_PROTOCOL_1S_1S_1S,
  .read_mode = SPI_FLASH_READ_MODE_STANDARD, /* Unused by OSPI Flash */
  .address_bytes = SPI_FLASH_ADDRESS_BYTES_4,
  .dummy_clocks = SPI_FLASH_DUMMY_CLOCKS_DEFAULT, /* Unused by OSPI Flash */
  .page_program_address_lines = (spi_flash_data_lines_t) 0U, /* Unused by OSPI Flash */
  .page_size_bytes = 64,
  .write_status_bit = 0,
  .write_enable_bit = 1,
  .page_program_command = 0x12,
  .write_enable_command = 0x06,
  .status_command = 0x05,
  .read_command = 0x13,
#if OSPI_B_CFG_XIP_SUPPORT_ENABLE
                .xip_enter_command           = 0,
                .xip_exit_command            = 0,
            #else
  .xip_enter_command = 0U,
  .xip_exit_command = 0U,
#endif
  .erase_command_list_length = sizeof(g_ospi_flash_erase_command_list) / sizeof(g_ospi_flash_erase_command_list[0]),
  .p_erase_command_list = &g_ospi_flash_erase_command_list[0],
  .p_extend = &g_ospi_flash_extended_cfg, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
const spi_flash_instance_t g_ospi_flash =
{ .p_ctrl = &g_ospi_flash_ctrl, .p_cfg = &g_ospi_flash_cfg, .p_api = &g_ospi_b_on_spi_flash, };

#if defined OSPI_B_CFG_DOTF_PROTECTED_MODE_SUPPORT_ENABLE
            rsip_instance_t const * const gp_rsip_instance = &RA_NOT_DEFINED;
            #endif
extern uint32_t g_fsp_common_thread_count;

const rm_freertos_port_parameters_t new_thread0_parameters =
{ .p_context = (void*) NULL, };

void new_thread0_create(void)
{
    /* Increment count so we will know the number of threads created in the RA Configuration editor. */
    g_fsp_common_thread_count++;

    /* Initialize each kernel object. */

#if 1
    new_thread0 = xTaskCreateStatic (
#else
                    BaseType_t new_thread0_create_err = xTaskCreate(
                    #endif
                                     new_thread0_func,
                                     (const char*) "New Thread", 1024 / 4, // In words, not bytes
                                     (void*) &new_thread0_parameters, //pvParameters
                                     1,
#if 1
                                     (StackType_t*) &new_thread0_stack,
                                     (StaticTask_t*) &new_thread0_memory
#else
                        & new_thread0
                        #endif
                                     );

#if 1
    if (NULL == new_thread0)
    {
        rtos_startup_err_callback (new_thread0, 0);
    }
#else
                    if (pdPASS != new_thread0_create_err)
                    {
                        rtos_startup_err_callback(new_thread0, 0);
                    }
                    #endif
}
static void new_thread0_func(void *pvParameters)
{
    /* Initialize common components */
    rtos_startup_common_init ();

    /* Initialize each module instance. */

#if (1 == BSP_TZ_NONSECURE_BUILD) && (1 == 1)
                    /* When FreeRTOS is used in a non-secure TrustZone application, portALLOCATE_SECURE_CONTEXT must be called prior
                     * to calling any non-secure callable function in a thread. The parameter is unused in the FSP implementation.
                     * If no slots are available then configASSERT() will be called from vPortSVCHandler_C(). If this occurs, the
                     * application will need to either increase the value of the "Process Stack Slots" Property in the rm_tz_context
                     * module in the secure project or decrease the number of threads in the non-secure project that are allocating
                     * a secure context. Users can control which threads allocate a secure context via the Properties tab when
                     * selecting each thread. Note that the idle thread in FreeRTOS requires a secure context so the application
                     * will need at least 1 secure context even if no user threads make secure calls. */
                     portALLOCATE_SECURE_CONTEXT(0);
                    #endif

    /* Enter user code for this thread. Pass task handle. */
    new_thread0_entry (pvParameters);
}
