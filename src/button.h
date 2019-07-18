/**
 * @file button.h
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @author Martin Bauer
 * @author Moritz Amann
 * @brief Small library for button of eval board.
 * @version 0.1
 * @date 2019-07-18
 * 
 * If you search for the documentation,
 * have a look at the source file.
 *
 */
 
 #pragma once

void button_init();
void button_register_handler(void (*handler)());