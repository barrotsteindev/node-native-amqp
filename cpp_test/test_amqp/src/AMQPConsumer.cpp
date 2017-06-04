#include "AMQPConsumer.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>

AMQPConsumer::AMQPConsumer(std::string broker_address,std::string queue_name, std::string routing_key, bool acks)
{
    m_channel = AmqpClient::Channel::Create(broker_address);
    m_consumer_string = m_channel->BasicConsume(queue_name, routing_key, true, acks);
    broker_address = broker_address;
    routing_key = routing_key;
    acks = acks;
    queue_name = queue_name;
}



AMQPConsumer::~AMQPConsumer()
{
    //dtor
}

AmqpClient::Envelope::ptr_t AMQPConsumer::Poll() {
    AmqpClient::Envelope::ptr_t msg;
    while (true) {
        msg = m_channel->BasicConsumeMessage(m_consumer_string);
        if(msg->Message()) { break; }
    }
    return msg;
}

void AMQPConsumer::ack(const AmqpClient::Envelope::ptr_t &msg_envelope) {
    m_channel->BasicAck(msg_envelope);
}
