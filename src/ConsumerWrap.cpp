#include "ConsumerWrap.h"

  void ConsumerWrap::Init() {
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

  ConsumerWrap * ConsumerWrap::Create(Channel * channel,
                               const v8::Local<v8::Object> & conf) {
    int timeOut = conf->Has(timeOutKey()) ?
                  conf->Get(timeOutKey())->NumberValue() :
                  200;
    int prefetchCount = conf->Has(prefetchKey()) ?
                        conf->Get(prefetchKey())->NumberValue() :
                        10;
    bool autoAck = conf->Has(ackKey()) ? conf->Get(ackKey())->BooleanValue() :
                   false;
    v8::Local<v8::String> v8Queue = conf->Get(queueKey())->ToString();
    Nan::Utf8String utfQueue(v8Queue);
    v8::Local<v8::String> v8RoutingKey = conf->Get(routingKey())->ToString();
    Nan::Utf8String utfRoutingKey(v8RoutingKey);
    return new ConsumerWrap(channel, std::string(* utfQueue),
                            std::string(* utfRoutingKey), autoAck,
                            prefetchCount, timeOut);
  }

  v8::Local<v8::Object> ConsumerWrap::V8Instance() {
    Nan::EscapableHandleScope scope;

    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { Nan::New<v8::External>(this) };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor());
    v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);
    return scope.Escape(instance);
  }


  ConsumerWrap::ConsumerWrap(Channel * channel, const std::string & queue,
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
      m_is_open = true;
      m_consumer = consumer;
}

  ConsumerWrap::~ConsumerWrap() {
    if (m_is_open) {
      delete m_consumer;
    }
  }


  NAN_METHOD(ConsumerWrap::New) {
    v8::Handle<v8::External> externalConsumer = v8::Handle<v8::External>::Cast
                                                 (info[0]);
    ConsumerWrap* obj = static_cast<ConsumerWrap*>(externalConsumer->Value());
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(ConsumerWrap::Close) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    if (obj->m_is_open) {
      obj->m_is_open = false;
      obj->m_consumer->Close();
      delete obj->m_consumer;
      return;
    }
    Nan::ThrowError("Consumer was already closed");
  }

  NAN_METHOD(ConsumerWrap::GetMessageSync) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    if (!obj->m_is_open) {
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

  NAN_METHOD(ConsumerWrap::GetMessage) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    if (!obj->m_is_open) {
      Nan::ThrowError("Consumer closed");
    }
    Nan::Callback * callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new ConsumerWorker(callback, * obj->m_consumer));
  }

  NAN_METHOD(ConsumerWrap::GetHandle) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    info.GetReturnValue().Set(obj->handle());
  }

  NAN_METHOD(ConsumerWrap::GetHostname) {
    ConsumerWrap * obj = Nan::ObjectWrap::Unwrap<ConsumerWrap>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->m_hostname).ToLocalChecked());
  }
