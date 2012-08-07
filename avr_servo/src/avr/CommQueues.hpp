#ifndef INCLUDE_COMMQUEUES_HPP_FILE
#define INCLUDE_COMMQUEUES_HPP_FILE

#include "config.hpp"
#include "Queue.hpp"

typedef Queue<QUEUE_RECV_SIZE> QueueSend;
typedef Queue<QUEUE_SEND_SIZE> QueueRecv;

#endif
