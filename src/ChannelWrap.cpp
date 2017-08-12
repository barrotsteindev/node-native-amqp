#include <nan.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "ConsumerWrap.h"
#include "Message.h"
#include "ChannelImpl.h"

class ChannelWrap : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init) {
    ConsumerWrap::Init();
    Message::Init();
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Channel").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "describe", Describe);
    Nan::SetPrototypeMethod(tpl, "Consumer", CreateConsumer);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("Channel").ToLocalChecked(),
     Nan::GetFunction(tpl).ToLocalChecked());
  }
  Nan::Persistent<v8::Object> m_conf;

 protected:
  explicit ChannelWrap(const Nan::FunctionCallbackInfo<v8::Value> & info) :
    m_conf(info[0]->ToObject()) { }

  static NAN_METHOD(CreateConsumer) {
    if (info.IsConstructCall()) {
      return Nan::ThrowTypeError("Does not support constructor calls");
    }
    ChannelWrap * channel = Nan::ObjectWrap::Unwrap<ChannelWrap>(info.Holder());
    v8::Local<v8::Object> consumerConf = info[0]->ToObject();

    if (!consumerConf->Has(ConsumerWrap::queueKey())) {
      return Nan::ThrowTypeError("Key: queue must be supllied");
    }
    if (!consumerConf->Has(ConsumerWrap::routingKey())) {
      return Nan::ThrowTypeError("Key: routingKey must be supllied");
    }
    ConsumerWrap * consumer;
    try {
      ConsumerWrap::Create(channel->GetChannel(), consumerConf);
    } catch (std::exception & e) {
      Nan::ThrowError(e.what());
      return;
    }
    v8::Local<v8::Object> consumerObj = consumer->V8Instance();
    info.GetReturnValue().Set(consumerObj);
  }

  Channel * GetChannel() {
    v8::Local<v8::Object> conf = Nan::New(m_conf);
    if (conf->Has(uriKey())) {
      v8::Local<v8::String> uri = conf->Get(uriKey())->ToString();
      Nan::Utf8String utfUri(uri);
      std::string stdUri = std::string(* utfUri);
      return ChannelImpl::CreateFromUri(stdUri);
    } else {
      v8::Local<v8::String> hostname = conf->Has(hostnameKey()) ?
                                        conf->Get(hostnameKey())->ToString() :
                                        localhost();
      int port = conf->Has(portKey()) ?
                  conf->Get(portKey())->NumberValue() : 5672;
      Nan::Utf8String utfHostname(hostname);
      std::string stdHostname = std::string(* utfHostname);
      return ChannelImpl::Create(stdHostname, port);
    }
  }

  static NAN_METHOD(Describe) {
    ChannelWrap * channel = Nan::ObjectWrap::Unwrap<ChannelWrap>(info.Holder());
    info.GetReturnValue().Set(Nan::New(channel->GetChannel()->Describe())
                                                             .ToLocalChecked());
  }

  static NAN_METHOD(New) {
     if (!info[0]->IsObject()) {
       Nan::ThrowTypeError("Channel configuration must be a json object");
     } else if (info.IsConstructCall()) {
      ChannelWrap * channel = new ChannelWrap(info);
      channel->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
     } else {
       // support non constructor call
       const int argc = 1;
       v8::Local<v8::Value> argv[argc] = {info[0]->ToObject()};
       v8::Local<v8::Function> cons = Nan::New(constructor());
       info.GetReturnValue().Set(cons->NewInstance(argc, argv));
     }
  }

 private:
  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  static const inline v8::Local<v8::String> hostnameKey() {
    static Nan::Persistent<v8::String> v8Hostname(Nan::New("hostName")
                                                  .ToLocalChecked());
    return Nan::New(v8Hostname);
  }

  static const inline v8::Local<v8::String> localhost() {
    static Nan::Persistent<v8::String> v8Localhost(Nan::New("localhost")
                                                  .ToLocalChecked());
    return Nan::New(v8Localhost);
  }

  static const inline v8::Local<v8::String> portKey() {
    static Nan::Persistent<v8::String> v8PortKey(Nan::New("port")
                                                .ToLocalChecked());
    return Nan::New(v8PortKey);
  }

  static const inline v8::Local<v8::String>  uriKey() {
    static Nan::Persistent<v8::String> uriKeyPersist(Nan::New("uri")
                                                    .ToLocalChecked());
    return Nan::New(uriKeyPersist);
  }
};

NODE_MODULE(AMQP, ChannelWrap::Init)
