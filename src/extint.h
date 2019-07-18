/**
 * @file extint.h
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @author Martin Bauer
 * @author Moritz Amann
 * @brief Small library for external interrupt on data ready pin.
 * @version 0.1
 * @date 2019-07-18
 * 
 * If you search for the documentation,
 * have a look at the source file.
 *
 */

#pragma once

void extint_init();
void extint_register_handler(void (*handler)());
void gpio_interrupt_handler();