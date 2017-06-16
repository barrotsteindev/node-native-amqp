#include "AMQPConsumer.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>

AMQPConsumer::AMQPConsumer(std::string broker_address,std::string queue_name, std::string routing_key, bool m_acks)
{
    m_channel = AmqpClient::Channel::Create(broker_address);
    m_acks = m_acks;
    m_consumer_string = m_channel->BasicConsume(queue_name, routing_key, false, m_acks, true, 10);
    broker_address = broker_address;
    routing_key = routing_key;
    queue_name = queue_name;
}



AMQPConsumer::~AMQPConsumer()
{
    //dtor
}

Message* AMQPConsumer::Poll() throw (bool) {
    AmqpClient::Envelope::ptr_t msg;
    m_channel->BasicConsumeMessage(m_consumer_string, msg, 1000);
    Message* msg_obj = new Message(m_channel, msg);
    return msg_obj;
}

void AMQPConsumer::ack(const AmqpClient::Envelope::ptr_t &msg_envelope) {
    m_channel->BasicAck(msg_envelope);
}
