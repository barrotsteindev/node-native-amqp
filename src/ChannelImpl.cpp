#include "ChannelImpl.h"

ChannelImpl::ChannelImpl(std::string broker_host, int broker_port,
                         std::string username, std::string pass,
                         std::string vhost, int framemax) {
  m_channel = AmqpClient::Channel::Create(broker_host, broker_port, username,
                                          pass, vhost, framemax);
  m_description = broker_host + ":" + std::to_string(broker_port);
}

ChannelImpl::ChannelImpl(std::string amqpUri, int framemax) {
  m_channel = AmqpClient::Channel::CreateFromUri(amqpUri, framemax);
  m_description = amqpUri;
}

ChannelImpl * ChannelImpl::Create(std::string broker_host, int broker_port,
                                  std::string username, std::string pass,
                                  std::string vhost, int framemax) {
  return new ChannelImpl(broker_host, broker_port, username, pass,
                         vhost, framemax);
}

ChannelImpl * ChannelImpl::CreateFromUri(std::string amqpUri, int framemax) {
  return new ChannelImpl(amqpUri, framemax);
}
