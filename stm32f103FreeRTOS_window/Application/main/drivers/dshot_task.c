#include "dshot_task.h"

#ifdef __cplusplus
extern "C" {
#endif
// Start file

// The throttle value can be set from another task. (PID, RC receiver...)
// volatile: prevent compiler caching
static volatile uint16_t g_throttle = 0;

void setDshotThrottle(uint16_t value) {
  g_throttle = value; // atomic on Cortex-M1 (32-bit aligned)
}

// ── DWT busy-wait ─────────────────────────────────────────────
static void dwt_init(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

#define DELAY_CYCLES(n)                                                        \
  do {                                                                         \
    uint32_t _s = DWT->CYCCNT;                                                 \
    while ((DWT->CYCCNT - _s) < (uint32_t)(n)) {                               \
    }                                                                          \
  } while (0)

// ── Send 1 frame ──────────────────────────────────────────────
static void dshot_send_frame(GPIO_TypeDef *port, uint16_t pin,
                             uint16_t packet) {
  __disable_irq(); // require

  for (int i = 15; i >= 0; i--) {
    port->BSRR = pin;
    if ((packet >> i) & 1) {
      DELAY_CYCLES(DS_H1);
      port->BSRR = (uint32_t)pin << 16;
      DELAY_CYCLES(DS_L1);
    } else {
      DELAY_CYCLES(DS_H0);
      port->BSRR = (uint32_t)pin << 16;
      DELAY_CYCLES(DS_L0);
    }
  }
  port->BSRR = (uint32_t)pin << 16; // ensure LOW
  DELAY_CYCLES(DS_RST);

  __enable_irq(); // <-- unlock interrupt for RTOS task
}

// ── Task core ────────────────────────────────────────────────
void DShot_Task(void const *argument) {
  dwt_init();

  dshotProtocolControl_t pcb = {.value = 0, .requestTelemetry = 0};

  for (;;) {
    pcb.value = g_throttle;

    uint16_t packet = prepareDshotPacket(&pcb);
    dshot_send_frame(GPIOA, GPIO_PIN_1, packet);

    osDelay(1); // yield for RTOS, 1ms loop
  }
}

// End file
#ifdef __cplusplus
}
#endif