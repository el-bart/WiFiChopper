/*
 * config.hpp
 *
 */
#ifndef INCLUDE_CONFIG_HPP_FILE
#define INCLUDE_CONFIG_HPP_FILE

// units commonly used
#define KHz   1000L
#define MHz   (1000L*KHz)

// quarc frequency in [Hz]
#define F_CPU (8L*MHz)

// USART speed (bps)
#define USART_BAUD (19200L)

// max queue size
#define QUEUE_MAX_SIZE 8

#endif
