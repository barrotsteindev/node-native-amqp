#include "ChannelImpl.h"

ChannelImpl::ChannelImpl(std::string broker_host, int broker_port) {
  m_channel = AmqpClient::Channel::Create(broker_host, broker_port);
  m_description = broker_host + ":" + std::to_string(broker_port);
}
