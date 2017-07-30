#ifndef AMQPCONSUMER_H_
#define AMQPCONSUMER_H_

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <string>
#include <mutex>
#include "Message.h"
#include "Channel.h"


class AMQPConsumer {
  std::string broker_address;
  std::string queue_name;
  std::string routing_key;
  bool m_acks;
  int m_timeout;

 public:
  explicit AMQPConsumer(Channel * connection, std::string queue_name,
                        std::string routing_key, bool m_acks,
                        int prefetchCount, int timeout);
  virtual ~AMQPConsumer();
  Message * Poll();
  void Close();

 private:
  std::string m_consumer_string;
  Channel * m_connection;
  std::mutex m_consume_lock;
};

#endif  // AMQPCONSUMER_H_
