#pragma once

#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "stm32f103xb.h"
#include "stm32f1xx.h"

#include "dshot.h" // prepareDshotPacket()
#include "main.h"
#include "pch.h"

extern void setDshotThrottle(uint16_t value);

// ── DWT busy-wait ─────────────────────────────────────────────
static void dwt_init(void);

// DShot300 @ 168 MHz
#define DS_H1 420u  // 2.50 µs
#define DS_L1 140u  // 0.83 µs
#define DS_H0 210u  // 1.25 µs
#define DS_L0 350u  // 2.08 µs
#define DS_RST 336u // reset gap 2 µs

// ── Task dshot pulse ────────────────────────────────────────────────
extern void DShot_Task(void const *argument);