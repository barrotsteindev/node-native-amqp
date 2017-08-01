#ifndef SRC_CHANNELIMPL_H_
#define SRC_CHANNELIMPL_H_
#include "Channel.h"
#include <string>

class ChannelImpl: public Channel {
 public:
  static ChannelImpl * Create(std::string const & broker_host,
    int broker_port = 5672, std::string const & username = "guest",
    std::string const & pass = "guest",
    std::string const & vhost = "/", int framemax = 131072);

  static ChannelImpl  * CreateFromUri(std::string const & amqpUri,
                                      int framemax = 131072);

  static ChannelImpl * CreateFromSecureUri(std::string const & amqpUri,
    std::string const & ca_cert_path = "",
    std::string const & client_key_path = "",
    std::string const & client_cert_path = "",
    bool verify = true, int framemax = 131072);
 protected:
  explicit ChannelImpl(std::string const & broker_host, int broker_port,
                       std::string const & username, std::string const & pass,
                       std::string const & vhost, int framemax);
  explicit ChannelImpl(std::string const & amqpUri, int framemax);
};

#endif  // SRC_CHANNELIMPL_H_
