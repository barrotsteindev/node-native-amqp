#ifndef SRC_CHANNEL_H_
#define SRC_CHANNEL_H_
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <string>

class Channel {
 public:
  AmqpClient::Channel::ptr_t GetChannel();
  virtual ~Channel();
  std::string Describe();


 protected:
  std::string m_description;
  AmqpClient::Channel::ptr_t m_channel = NULL;
};

#endif  // SRC_CHANNEL_H_
