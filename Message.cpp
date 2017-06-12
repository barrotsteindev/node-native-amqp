#include "Message.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>

Message::Message(AmqpClient::Channel::ptr_t &Channel, const AmqpClient::Envelope::ptr_t &msg_envelope)
{
    //m_channel = Channel;
    m_channel = AmqpClient::Channel::Create("localhost");
}



Message::~Message()
{
    //dtor
}

void Message::ack(const AmqpClient::Envelope::ptr_t &msg_envelope) {
    m_channel->BasicAck(msg_envelope);
}
