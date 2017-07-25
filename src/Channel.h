#ifndef SRC_CHANNEL_H_
#define SRC_CHANNEL_H_
#include "ChannelBase.h"
#include <string>

class Channel: public ChannelBase {
 public:
  explicit Channel(std::string broker_host, int broker_port = 5672);
};

#endif  // SRC_CHANNEL_H_
