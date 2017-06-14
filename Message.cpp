#include "Message.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>

Message::Message(AmqpClient::Channel::ptr_t channel, const AmqpClient::Envelope::ptr_t &msg_envelope) {
  m_channel = channel;
  m_envelope = msg_envelope;
}

void Message::Ack(void) {
  m_channel->BasicAck(m_envelope);
}

bool Message::Valid(void) {
  return (m_envelope != NULL);
}

Message::~Message() {
    // dtor
}
