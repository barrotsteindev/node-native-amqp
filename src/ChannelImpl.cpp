#include "ChannelImpl.h"

ChannelImpl::ChannelImpl(std::string const & broker_host, int broker_port,
  std::string const & username, std::string const & pass,
  std::string const & vhost, int framemax) {
    try {
      m_channel = AmqpClient::Channel::Create(broker_host, broker_port,
        username, pass, vhost, framemax);
    } catch (std::exception & e) {
      std::string eMessage = std::string(
        std::string("could not create channel: ") + e.what());
      throw std::runtime_error(eMessage.c_str());
    }
    m_description = broker_host + ":" + std::to_string(broker_port);
}

ChannelImpl::ChannelImpl(std::string const & amqpUri, int framemax) {
  m_description = amqpUri;
  try {
    m_channel = AmqpClient::Channel::CreateFromUri(amqpUri, framemax);
  } catch (std::exception & e) {
    std::string eMessage = std::string(
      std::string("could not create channel: ") + e.what());
    throw std::runtime_error(eMessage.c_str());
  }
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
