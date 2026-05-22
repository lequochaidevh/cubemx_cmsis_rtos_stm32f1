#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "dshot.h" // prepareDshotPacket()
#include "main.h"

#include "stm32f103xb.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

// Throttle value được set từ task khác (PID, RC receiver...)
// volatile để compiler không cache
static volatile uint16_t g_throttle = 0;

void setDshotThrottle(uint16_t value) {
  g_throttle = value; // atomic trên Cortex-M4 (32-bit aligned)
}

// ── DWT busy-wait ─────────────────────────────────────────────
static void dwt_init(void);

// DShot300 @ 168 MHz
#define DS_H1 420u  // 2.50 µs
#define DS_L1 140u  // 0.83 µs
#define DS_H0 210u  // 1.25 µs
#define DS_L0 350u  // 2.08 µs
#define DS_RST 336u // reset gap 2 µs

// ── Gửi 1 frame ──────────────────────────────────────────────
// static void dshot_send_frame(GPIO_TypeDef *port, uint16_t pin, uint16_t
// packet);

// ── Task dshot pulse ────────────────────────────────────────────────
// void DShot_Task(void const *argument);