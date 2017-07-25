#include "ChannelBase.h"

std::string ChannelBase::Describe() {
  return m_description;
}

AmqpClient::Channel::ptr_t ChannelBase::GetChannel() {
  return m_channel;
}

ChannelBase::~ChannelBase() {
  // dtor
}
