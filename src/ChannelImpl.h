#ifndef SRC_CHANNELIMPL_H_
#define SRC_CHANNELIMPL_H_
#include "Channel.h"
#include <string>

class ChannelImpl: public Channel {
 public:
  static ChannelImpl * Create(std::string broker_host, int broker_port = 5672,
                              std::string username = "guest",
                              std::string pass = "guest",
                              std::string vhost = "/", int framemax = 131072);

  static ChannelImpl  * CreateFromUri(std::string amqpUri,
                                      int framemax = 131072);

  static ChannelImpl * CreateFromSecureUri(std::string amqpUri,
                                           std::string ca_cert_path = "",
                                           std::string client_key_path = "",
                                           std::string client_cert_path = "",
                                           bool verify = true,
                                           int framemax = 131072);
 protected:
  explicit ChannelImpl(std::string broker_host, int broker_port,
                       std::string username, std::string pass,
                       std::string vhost, int framemax);
  explicit ChannelImpl(std::string amqpUri, int framemax);
};

#endif  // SRC_CHANNELIMPL_H_
