/**
 * @file interface.c
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @brief Library responsible for the binary data communication.
 * @version 0.1
 * @date 2019-07-14
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "interface.h"
#include "uart.h"

/**
 * @brief Magic byte used by the interface.
 */
#define INTERFACE_MAGIC_BYTE 0xbe;

/**
 * @brief Structure used to pack the data
 */
typedef union {
    struct {
        uint8_t magic_byte;
        uint8_t status;
        float payload;
    } __attribute__((packed));
    uint8_t raw[6];
} InterfaceDataFrame_t;

/**
 * @brief Packs and sends a status and a payload.
 *
 * @param status current status of the model
 * @param payload data to be send
 */
void interface_send(uint8_t status, float payload) {
    InterfaceDataFrame_t data_frame;
    data_frame.magic_byte = INTERFACE_MAGIC_BYTE;
    data_frame.status = status;
    data_frame.payload = payload;
    uart_send((uint8_t*) &(data_frame.raw), sizeof(data_frame.raw));
}