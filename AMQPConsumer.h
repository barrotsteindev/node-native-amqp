#ifndef AMQPCONSUMER_H_
#define AMQPCONSUMER_H_

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <string>
#include <mutex>
#include "Message.h"


class AMQPConsumer {
  std::string broker_address;
  std::string queue_name;
  std::string routing_key;
  bool m_acks;
  int m_timeout;

 public:
  explicit AMQPConsumer(std::string broker_address, std::string queue_name,
                        std::string routing_key, bool m_acks,
                        int prefetchCount, int timeout);
  explicit AMQPConsumer(std::string queue_name) { broker_address = "";
                                                  routing_key = "#",
                                                  m_acks = true;
                                                }
  explicit AMQPConsumer(std::string broker_address, std::string queue_name,
                        std::string routing_key) { bool m_acks = true; }
  explicit AMQPConsumer(std::string broker_address,
                        std::string queue_name) { routing_key = "#";
                                                  m_acks = true;
                                                }
  virtual ~AMQPConsumer();
  Message * Poll();
  void Close();

 private:
  std::string m_consumer_string;
  AmqpClient::Channel::ptr_t m_channel;
  std::mutex _consume_lock;
};

#endif  // AMQPCONSUMER_H_
