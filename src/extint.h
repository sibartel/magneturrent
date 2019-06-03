#pragma once

void extint_init();
void extint_register_handler(void (*handler)());
void gpio_interrupt_handler();