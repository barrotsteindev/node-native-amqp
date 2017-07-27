#include "AMQPConsumer.h"
#include "ChannelImpl.h"

AMQPConsumer::AMQPConsumer(std::string broker_address, std::string queue_name,
  std::string routing_key, bool m_acks, int prefetchCount, int timeout) {
    // m_channel = AmqpClient::Channel::Create(broker_address);
    m_connection = new ChannelImpl(broker_address, 5672);
    m_acks = m_acks;
    m_consumer_string = m_connection->GetChannel()->BasicConsume(queue_name, routing_key,
                                                true, m_acks, false,
                                                prefetchCount);
    broker_address = broker_address;
    routing_key = routing_key;
    queue_name = queue_name;
    m_timeout = timeout;
}


AMQPConsumer::~AMQPConsumer() {
    // dtor
    delete m_connection;
}

Message * AMQPConsumer::Poll() {
    AmqpClient::Envelope::ptr_t msg;
    AMQPConsumer::m_consume_lock.lock();
    m_connection->GetChannel()->BasicConsumeMessage(m_consumer_string, msg, m_timeout);
    AMQPConsumer::m_consume_lock.unlock();
    if (msg == NULL) {
      return NULL;
    }
    Message * msg_obj = new Message(m_connection->GetChannel(), msg);
    return msg_obj;
}

void AMQPConsumer::Close() {
    m_connection->GetChannel()->BasicCancel(m_consumer_string);
    delete m_connection;
}
