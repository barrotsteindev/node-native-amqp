#include <nan.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "Message.h"
#include "AMQPConsumer.h"

class Consumer : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init) {
    Message::Init();
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Consumer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "getHandle", GetHandle);
    Nan::SetPrototypeMethod(tpl, "getHostname", GetHostname);
    Nan::SetPrototypeMethod(tpl, "getMessage", GetMessage);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("Consumer").ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked());
  }

 private:
   static inline v8::Local<v8::String> & hostnameKey() {
       static v8::Local<v8::String> v8Hostname = Nan::New("hostname").ToLocalChecked();
       return v8Hostname;
    }

    static const inline v8::Local<v8::String> & localhost() {
      static v8::Local<v8::String> v8Localhost = Nan::New("localhost").ToLocalChecked();
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
      static v8::Local<v8::String> v8timeOut = Nan::New("timeOut").ToLocalChecked();
      return v8timeOut;
    }

    static const inline v8::Local<v8::String> & routingKey() {
      static v8::Local<v8::String> v8RoutingKey = Nan::New("routingKey").ToLocalChecked();
      return v8RoutingKey;
    }

    explicit Consumer(const v8::Local<v8::Object> conf) {
      // todo: make sure object is not empty
      if (!conf->Has(queueKey())) {
        Nan::ThrowTypeError("Key: queue must be supllied");
      }
      if (!conf->Has(routingKey())) {
        Nan::ThrowTypeError("Key: routingKey must be supllied");
      }
      v8::Local<v8::String> hostname = conf->Has(hostnameKey()) ? conf->Get(hostnameKey())->ToString() : localhost();
      int timeOut = conf->Has(timeOutKey()) ? conf->Get(timeOutKey())->NumberValue() : 500;
      v8::Local<v8::String> v8Queue = conf->Get(queueKey())->ToString();
      v8::Local<v8::String> v8RoutingKey = conf->Get(routingKey())->ToString();
      Nan::Utf8String utfHostname(hostname);
      hostname_ = std::string(*utfHostname);
      Nan::Utf8String utfQueue(v8Queue);
      Nan::Utf8String utfRoutingKey(v8RoutingKey);
      hostname_ = std::string(*utfHostname);
      AMQPConsumer *consumer = new AMQPConsumer(hostname_, std::string(*utfQueue), std::string(*utfRoutingKey), false);
      consumer_ = consumer;
    }

  ~Consumer() {}

  static NAN_METHOD(New) {
    if (!info[0]->IsObject()) {
      Nan::ThrowTypeError("Consumer configuration must be a json object");
    } else if (info.IsConstructCall()) {
      v8::Local<v8::Object> conf = info[0]->ToObject();
      Consumer *obj = new Consumer(conf);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]->ToObject()};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
  }

  static NAN_METHOD(GetMessage) {
    Consumer* obj = Nan::ObjectWrap::Unwrap<Consumer>(info.Holder());
    Message* msg = obj->consumer_->Poll();
    if (!(msg->Valid())) {
      Nan::ThrowReferenceError("Consumer time out");
      return info.GetReturnValue().Set(Nan::New<v8::String>("consumer time out").ToLocalChecked());
    }
    v8::Local<v8::Object> msg_obj = msg->V8Instance();
    info.GetReturnValue().Set(msg_obj);
  }

  static NAN_METHOD(GetHandle) {
    Consumer* obj = Nan::ObjectWrap::Unwrap<Consumer>(info.Holder());
    info.GetReturnValue().Set(obj->handle());
  }

  static NAN_METHOD(GetHostname) {
    Consumer* obj = Nan::ObjectWrap::Unwrap<Consumer>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->hostname_).ToLocalChecked());
  }

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  std::string hostname_;
  AMQPConsumer* consumer_;
  Nan::Persistent<v8::Object> consumer_p;
  AmqpClient::Envelope::ptr_t cur_msg_;

};

NODE_MODULE(objectwrapper, Consumer::Init)
