#include "Channel.h"

std::string Channel::Describe() {
  return m_description;
}

AmqpClient::Channel::ptr_t Channel::GetChannel() {
  return m_channel;
}

Channel::~Channel() {
  // dtor
}
