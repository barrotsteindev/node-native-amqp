#ifndef SRC_CONSUMERWRAP_H_
#define SRC_CONSUMERWRAP_H_

#include <nan.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "Message.h"
#include "AMQPConsumer.h"
#include "AsyncConsumer.h"
#include "ChannelImpl.h"
#include <string>

class ConsumerWrap : public Nan::ObjectWrap {
 public:
  static void Init();
  static ConsumerWrap * Create(Channel * channel,
                               const v8::Local<v8::Object> & conf);
  v8::Local<v8::Object> V8Instance();
  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }
  static const inline v8::Local<v8::String> & queueKey() {
    static v8::Local<v8::String> v8Queue = Nan::New("queue")
                                                         .ToLocalChecked();
    return v8Queue;
  }

  static const inline v8::Local<v8::String> & ackKey() {
    static v8::Local<v8::String> v8Ack = Nan::New("autoAck").ToLocalChecked();
    return v8Ack;
  }

  static const inline v8::Local<v8::String> & timeOutKey() {
    static v8::Local<v8::String> v8TimeOut = Nan::New("timeOut")
                                                     .ToLocalChecked();
    return v8TimeOut;
  }

  static const inline v8::Local<v8::String> & routingKey() {
    static v8::Local<v8::String> v8RoutingKey = Nan::New("routingKey")
                                                        .ToLocalChecked();
    return v8RoutingKey;
  }

  static const inline v8::Local<v8::String> & prefetchKey() {
    static v8::Local<v8::String> v8PrefetchKey = Nan::New("prefetch")
                                                         .ToLocalChecked();
    return v8PrefetchKey;
  }

 private:
  bool m_isOpen = false;
  explicit ConsumerWrap(Channel * channel, const std::string & queue,
      const std::string & routingKey, bool autoAck,
      int prefetchCount, int timeOut);
  ~ConsumerWrap();
  static NAN_METHOD(New);
  static NAN_METHOD(Close);
  static NAN_METHOD(GetMessageSync);
  static NAN_METHOD(GetMessage);
  static NAN_METHOD(GetHandle);
  static NAN_METHOD(GetHostname);
  std::string m_hostname;
  AMQPConsumer * m_consumer;
};

#endif  // SRC_CONSUMERWRAP_H_
