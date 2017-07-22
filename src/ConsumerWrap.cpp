#include <nan.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "Message.h"
#include "AMQPConsumer.h"
#include "AsyncConsumer.h"

class ConsumerWrap : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init) {
    Message::Init();
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Consumer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "getHandle", GetHandle);
    Nan::SetPrototypeMethod(tpl, "getHostname", GetHostname);
    Nan::SetPrototypeMethod(tpl, "getMessageSync", GetMessageSync);
    Nan::SetPrototypeMethod(tpl, "getMessage", GetMessage);
    Nan::SetPrototypeMethod(tpl, "close", Close);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("Consumer").ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked());
  }

 private:
  bool m_isOpen = false;
  static inline v8::Local<v8::String> & hostnameKey() {
  static v8::Local<v8::String> v8Hostname = Nan::New("hostName")
                                                     .ToLocalChecked();
    return v8Hostname;
  }

    static const inline v8::Local<v8::String> & localhost() {
      static v8::Local<v8::String> v8Localhost = Nan::New("localhost")
                                                          .ToLocalChecked();
      return v8Localhost;
    }

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

    explicit ConsumerWrap(const v8::Local<v8::Object> conf) {
      // todo: make sure object is not empty
      if (!conf->Has(queueKey())) {
        Nan::ThrowTypeError("Key: queue must be supllied");
      }
      if (!conf->Has(routingKey())) {
        Nan::ThrowTypeError("Key: routingKey must be supllied");
      }
      v8::Local<v8::String> hostname = conf->Has(hostnameKey()) ?
                                       conf->Get(hostnameKey())->ToString() :
                                       localhost();
      int timeOut = conf->Has(timeOutKey()) ?
                    conf->Get(timeOutKey())->NumberValue() :
                    200;
      int prefetchCount = conf->Has(prefetchKey()) ?
                          conf->Get(prefetchKey())->NumberValue() :
                          10;
      v8::Local<v8::String> v8Queue = conf->Get(queueKey())->ToString();
      v8::Local<v8::String> v8RoutingKey = conf->Get(routingKey())->ToString();
      Nan::Utf8String utfHostname(hostname);
      m_hostname = std::string(* utfHostname);
      Nan::Utf8String utfQueue(v8Queue);
      Nan::Utf8String utfRoutingKey(v8RoutingKey);
      AMQPConsumer * consumer;
      try {
          consumer = new AMQPConsumer(m_hostname, std::string(* utfQueue),
                                      std::string(* utfRoutingKey),
                                      false, prefetchCount, timeOut);
      } catch (...) {
          std::string exceptionString = "could not connect to host: "
                                        + m_hostname;
          Nan::ThrowError(exceptionString.c_str());
      }
      m_isOpen = true;
      m_consumer = consumer;
    }

  ~ConsumerWrap() {
    delete m_consumer;
  }

  static NAN_METHOD(New) {
    if (!info[0]->IsObject()) {
      Nan::ThrowTypeError("Consumer configuration must be a json object");
    } else if (info.IsConstructCall()) {
      v8::Local<v8::Object> conf = info[0]->ToObject();
      ConsumerWrap * obj = new ConsumerWrap(conf);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]->ToObject()};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
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
    if (!(msg->Valid())) {
      Nan::ThrowReferenceError("Consumer time out");
      return info.GetReturnValue().Set(Nan::New<v8::String>
                                       ("consumer time out").ToLocalChecked());
    }
    v8::Local<v8::Object> msg_obj = msg->V8Instance();
    info.GetReturnValue().Set(msg_obj);
  }

  static NAN_METHOD(GetMessage) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    if (!obj->m_isOpen) {
      Nan::ThrowError("Consumer was already closed");
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

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  std::string m_hostname;
  AMQPConsumer * m_consumer;
};

NODE_MODULE(objectwrapper, ConsumerWrap::Init)
