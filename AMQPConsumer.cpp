#include "AMQPConsumer.h"

AMQPConsumer::AMQPConsumer(std::string broker_address, std::string queue_name,
  std::string routing_key, bool m_acks, int prefetchCount, int timeout) {
    m_channel = AmqpClient::Channel::Create(broker_address);
    m_acks = m_acks;
    m_consumer_string = m_channel->BasicConsume(queue_name, routing_key,
                                                true, m_acks, false,
                                                prefetchCount);
    broker_address = broker_address;
    routing_key = routing_key;
    queue_name = queue_name;
    m_timeout = timeout;
    m_open = true;
}



AMQPConsumer::~AMQPConsumer() {
    // dtor
}

Message * AMQPConsumer::Poll() {
    AmqpClient::Envelope::ptr_t msg;
    AMQPConsumer::m_consume_lock.lock();
    m_channel->BasicConsumeMessage(m_consumer_string, msg, m_timeout);
    AMQPConsumer::m_consume_lock.unlock();
    if (msg == NULL) {
      return NULL;
    }
    Message * msg_obj = new Message(m_channel, msg);
    return msg_obj;
}

void AMQPConsumer::Close() {
    if (m_open) {
      m_channel->BasicCancel(m_consumer_string);
      m_open = false;
    }
}
