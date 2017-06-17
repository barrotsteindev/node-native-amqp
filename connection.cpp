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
  explicit Consumer(v8::Local<v8::String> hostname)  {
    Nan::Utf8String nan_string(hostname);
    hostname_ = std::string(*nan_string);
    AMQPConsumer *consumer = new AMQPConsumer(hostname_, "jobs", "jobs", false);
    consumer_ = consumer;
  }
  ~Consumer() {}

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      v8::Local<v8::String> firstArg = info[0]->IsUndefined() ? Nan::New<v8::String>("localhost").ToLocalChecked() : info[0]->ToString();
      Consumer *obj = new Consumer(firstArg);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]->ToString()};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
  }

  static NAN_METHOD(GetMessage) {
    Consumer* obj = Nan::ObjectWrap::Unwrap<Consumer>(info.Holder());
    Message* msg = obj->consumer_->Poll();
    if (!(msg->Valid())) {
      return info.GetReturnValue().Set(Nan::New<v8::String>("consumer time out").ToLocalChecked());
    }
    info.GetReturnValue().Set(Nan::New(msg->MessageBody()).ToLocalChecked());
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
