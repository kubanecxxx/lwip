/*
 ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
 2011,2012 Giovanni Di Sirio.

 This file is part of ChibiOS/RT.

 ChibiOS/RT is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 ChibiOS/RT is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "lwipthread.h"

#include "web/web.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#include "mac_lld.h"
#include "chsprintf.h"

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg)
{

	(void) arg;
	chRegSetThreadName("blinker");
	while (TRUE)
	{
		palClearPad(GPIOF, GPIOF_STAT1);
		chThdSleepMilliseconds(500);
		palSetPad(GPIOF, GPIOF_STAT1);
		chThdSleepMilliseconds(500);
	}
}

//extern struct udp_pcb *udp_pcbs;

/*
 * Application entry point.
 */
int main(void)
{

	/*
	 * System initializations.
	 * - HAL initialization, this also initializes the configured device drivers
	 *   and performs the board-specific initializations.
	 * - Kernel initialization, the main() function becomes a thread and the
	 *   RTOS is active.
	 */
	halInit();
	chSysInit();

	/*
	 * ethernet  pin setup
	 */
	/*	palSetPadMode(GPIOA, 1, PAL_MODE_ALTERNATE(11));
	 palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(11));
	 palSetPadMode(GPIOB, 11, PAL_MODE_ALTERNATE(11));
	 palSetPadMode(GPIOB, 12, PAL_MODE_ALTERNATE(11));
	 palSetPadMode(GPIOB, 13, PAL_MODE_ALTERNATE(11));
	 palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(11));
	 palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(11));
	 palSetPadMode(GPIOC, 1, PAL_MODE_ALTERNATE(11));
	 palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(11));
	 */
	//RESET
	palSetPadMode(GPIOB, 10, PAL_MODE_OUTPUT_PUSHPULL);
	//INTERRUPT
	palSetPadMode(GPIOB, 14, PAL_MODE_INPUT);

	palSetPad(GPIOB, 10);

	palSetPadMode(GPIOE, 3, PAL_MODE_OUTPUT_PUSHPULL);
	palClearPad(GPIOE, 3);

	/*
	 * Activates the serial driver 6 using the driver default configuration.
	 */
	sdStart(&SD6, NULL);

	/*
	 * Creates the blinker thread.
	 */
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

	/*
	 * Creates the LWIP threads (it changes priority internally).
	 */
	chThdCreateStatic(wa_lwip_thread, LWIP_THREAD_STACK_SIZE, NORMALPRIO + 1,
			lwip_thread, NULL);

	/*
	 * Creates the HTTP thread (it changes priority internally).
	 */
	chThdCreateStatic(wa_http_server, sizeof(wa_http_server), NORMALPRIO + 1,
			http_server, NULL);

	/*
	 * Normal main() thread activity, in this demo it does nothing except
	 * sleeping in a loop and check the button state.
	 */

	uint8_t * data = "dezo";

	uint8_t pole[50];

	struct pbuf *p;

#if 0
	//loopback
	uint32_t i = mii_read(&ETHD1, 0);
	mii_write(&ETHD1, 0, i | (1 << 14));
#endif

	while (TRUE)
	{

		chThdSleepMilliseconds(500);
#if 1

		chsprintf(pole, "zkurveny pakety %d", ETH->MMCRFCECR);

		p = pbuf_alloc(PBUF_TRANSPORT, strlen(pole), PBUF_RAM);
		p->payload = pole;
		udp_sendto(udp_pcbs, p, IP_ADDR_BROADCAST, 7);
		pbuf_free(p);

#endif
	}
}
