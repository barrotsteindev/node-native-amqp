#ifndef SRC_CHANNEL_H_
#define SRC_CHANNEL_H_
#include "Channel.h"
#include <string>

class ChannelImpl: public Channel {
 public:
  explicit ChannelImpl(std::string broker_host, int broker_port = 5672);
};

#endif  // SRC_CHANNEL_H_
