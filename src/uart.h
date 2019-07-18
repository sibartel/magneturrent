/**
 * @file uart.h
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @author Martin Bauer
 * @author Moritz Amann
 * @brief Small library for the internal uart0.
 * @version 0.1
 * @date 2019-05-19
 * 
 * If you search for the documentation,
 * have a look at the source file.
 *
 */

#pragma once

void uart_init();
void uart_send(const uint8_t* data, uint32_t size);
void uart_interrupt_handler();