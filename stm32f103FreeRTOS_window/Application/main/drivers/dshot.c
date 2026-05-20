/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: jflyper
 *
 * Follows the extended dshot telemetry documentation found at
 * https://github.com/bird-sanctuary/extended-dshot-telemetry
 */

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

// platform
#define USE_DSHOT
#define FAST_CODE

// fix order
#include "drivers/nvic.h"
// fix order
#include "common/atomic.h"
// fix order

#include "dshot.h"

#ifdef USE_DSHOT

#define ERPM_PER_LSB 100.0f

FAST_CODE uint16_t prepareDshotPacket(dshotProtocolControl_t *pcb) {
  uint16_t packet;

  ATOMIC_BLOCK(NVIC_PRIO_DSHOT_DMA) {
    packet = (pcb->value << 1) | (pcb->requestTelemetry ? 1 : 0);
    pcb->requestTelemetry = false; // reset telemetry request to make sure it's
                                   // triggered only once in a row
  }

  // compute checksum
  unsigned csum = 0;
  unsigned csum_data = packet;
  for (int i = 0; i < 3; i++) {
    csum ^= csum_data; // xor data by nibbles
    csum_data >>= 4;
  }
  // append checksum
  csum &= 0xf;
  packet = (packet << 4) | csum;

  return packet;
}

#endif // USE_DSHOT

// temporarily here, needs to be moved during refactoring
void validateAndfixMotorOutputReordering(uint8_t *array, const unsigned size) {
  bool invalid = false;

  for (unsigned i = 0; i < size; i++) {
    if (array[i] >= size) {
      invalid = true;
      break;
    }
  }

  int valuesAsIndexes[size];

  for (unsigned i = 0; i < size; i++) {
    valuesAsIndexes[i] = -1;
  }

  if (!invalid) {
    for (unsigned i = 0; i < size; i++) {
      if (-1 != valuesAsIndexes[array[i]]) {
        invalid = true;
        break;
      }

      valuesAsIndexes[array[i]] = array[i];
    }
  }

  if (invalid) {
    for (unsigned i = 0; i < size; i++) {
      array[i] = i;
    }
  }
}
