#include <nan.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "Message.h"
#include "AMQPConsumer.h"
#include "AsyncConsumer.h"
#include "ChannelImpl.h"

class ConsumerWrap : public Nan::ObjectWrap {
 public:
  static void Init() {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Consumer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "getHandle", GetHandle);
    Nan::SetPrototypeMethod(tpl, "getHostname", GetHostname);
    Nan::SetPrototypeMethod(tpl, "getMessageSync", GetMessageSync);
    Nan::SetPrototypeMethod(tpl, "getMessage", GetMessage);
    Nan::SetPrototypeMethod(tpl, "close", Close);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  }

  static ConsumerWrap * Create(Channel * channel,
                               const v8::Local<v8::Object> & conf) {
    if (!conf->Has(routingKey())) {
      Nan::ThrowTypeError("Key: routingKey must be supllied");
    }
    int timeOut = conf->Has(timeOutKey()) ?
                  conf->Get(timeOutKey())->NumberValue() :
                  200;
    int prefetchCount = conf->Has(prefetchKey()) ?
                        conf->Get(prefetchKey())->NumberValue() :
                        10;
    v8::Local<v8::String> v8Queue = conf->Get(queueKey())->ToString();
    Nan::Utf8String utfQueue(v8Queue);
    v8::Local<v8::String> v8RoutingKey = conf->Get(routingKey())->ToString();
    Nan::Utf8String utfRoutingKey(v8RoutingKey);
    return new ConsumerWrap(channel, std::string(* utfQueue),
                            std::string(* utfRoutingKey), false,
                            prefetchCount, timeOut);
  }

  v8::Local<v8::Object> V8Instance() {
    Nan::EscapableHandleScope scope;

    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { Nan::New<v8::External>(this) };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor());
    v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);
    return scope.Escape(instance);
  }

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

 private:
  bool m_isOpen = false;

    static inline v8::Local<v8::String> & queueKey() {
      static v8::Local<v8::String> v8Queue = Nan::New("queue").ToLocalChecked();
      return v8Queue;
    }

    static const inline v8::Local<v8::String> & ackKey() {
      static v8::Local<v8::String> v8Ack = Nan::New("autoAck").ToLocalChecked();
      return v8Ack;
    }

    static const inline v8::Local<v8::String> & timeOutKey() {
      static v8::Local<v8::String> v8timeOut = Nan::New("timeOut")
                                                        .ToLocalChecked();
      return v8timeOut;
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

    explicit ConsumerWrap(Channel * channel, const std::string & queue,
        const std::string & routingKey, bool autoAck,
        int prefetchCount, int timeOut) {
          m_hostname = channel->Describe();
          AMQPConsumer * consumer;
          try {
              consumer = new AMQPConsumer(channel, queue, routingKey,
                                          false, prefetchCount, timeOut);
          } catch (...) {
              Nan::ThrowError(std::string(
                "could not create consumer for queue: " + queue).c_str());
          }
          m_isOpen = true;
          m_consumer = consumer;
        }

  ~ConsumerWrap() {
    if (m_isOpen) {
      delete m_consumer;
    }
  }


  static NAN_METHOD(New) {
    v8::Handle<v8::External> externalConsumer = v8::Handle<v8::External>::Cast
                                                 (info[0]);
    ConsumerWrap* obj = static_cast<ConsumerWrap*>(externalConsumer->Value());
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  static NAN_METHOD(Close) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    if (obj->m_isOpen) {
      obj->m_isOpen = false;
      obj->m_consumer->Close();
      delete obj->m_consumer;
      return;
    }
    Nan::ThrowError("Consumer was already closed");
  }

  static NAN_METHOD(GetMessageSync) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    if (!obj->m_isOpen) {
      Nan::ThrowError("Consumer was already closed");
    }
    Message * msg = obj->m_consumer->Poll();
    if (msg == NULL) {
      Nan::ThrowReferenceError("Consumer time out");
      return info.GetReturnValue().Set(Nan::New<v8::String>
                                       ("consumer timed out").ToLocalChecked());
    }
    v8::Local<v8::Object> msg_obj = msg->V8Instance();
    info.GetReturnValue().Set(msg_obj);
  }

  static NAN_METHOD(GetMessage) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    if (!obj->m_isOpen) {
      Nan::ThrowError("Consumer closed");
    }
    Nan::Callback * callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new ConsumerWorker(callback, * obj->m_consumer));
  }

  static NAN_METHOD(GetHandle) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    info.GetReturnValue().Set(obj->handle());
  }

  static NAN_METHOD(GetHostname) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->m_hostname).ToLocalChecked());
  }

  std::string m_hostname;
  AMQPConsumer * m_consumer;
};
