#include "ChannelImpl.h"

ChannelImpl::ChannelImpl(std::string const & broker_host, int broker_port,
  std::string const & username, std::string const & pass,
  std::string const & vhost, int framemax) {
  m_channel = AmqpClient::Channel::Create(broker_host, broker_port, username,
    pass, vhost, framemax);
  m_description = broker_host + ":" + std::to_string(broker_port);
}

ChannelImpl::ChannelImpl(std::string const & amqpUri, int framemax) {
  m_description = amqpUri;
  m_channel = AmqpClient::Channel::CreateFromUri(amqpUri, framemax);
}

ChannelImpl * ChannelImpl::Create(std::string const & broker_host,
  int broker_port, std::string const & username, std::string const & pass,
  std::string const & vhost, int framemax) {
  return new ChannelImpl(broker_host, broker_port, username,
    pass, vhost, framemax);
}

ChannelImpl * ChannelImpl::CreateFromUri(std::string const & amqpUri,
  int framemax) {
    return new ChannelImpl(amqpUri, framemax);
}
