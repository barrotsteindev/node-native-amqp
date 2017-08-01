#include "AMQPConsumer.h"
#include "ChannelImpl.h"


AMQPConsumer::AMQPConsumer(Channel * connection, std::string queue_name,
  std::string routing_key, bool acks, int prefetchCount, int timeout):
  m_connection(connection), m_acks(acks), m_routing_key(routing_key),
  m_queue_name(queue_name), m_timeout(timeout) {
    m_broker_address = connection->Describe();
    m_consumer_string = m_connection->GetChannel()->BasicConsume(queue_name,
                                                                 routing_key,
                                                                 true, m_acks,
                                                                 false,
                                                                 prefetchCount);
}


AMQPConsumer::~AMQPConsumer() {
    // dtor
    delete m_connection;
}

Message * AMQPConsumer::Poll() {
    AmqpClient::Envelope::ptr_t msg;
    AMQPConsumer::m_consume_lock.lock();
    m_connection->GetChannel()->BasicConsumeMessage(m_consumer_string,
                                                    msg, m_timeout);
    AMQPConsumer::m_consume_lock.unlock();
    if (msg == NULL) {
      return NULL;
    }
    Message * msg_obj = new Message(m_connection, msg);
    return msg_obj;
}

void AMQPConsumer::Close() {
    m_connection->GetChannel()->BasicCancel(m_consumer_string);
    delete m_connection;
}
