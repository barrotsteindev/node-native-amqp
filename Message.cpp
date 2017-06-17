#include "Message.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <nan.h>

static NAN_MODULE_INIT(Init) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Message").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Message").ToLocalChecked(),
    Nan::GetFunction(tpl).ToLocalChecked());
}

static NAN_METHOD(New) {
  Message* obj = new Message();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::Function>& Message::constructor(void) {
  Nan::Persistent<v8::Function> my_constructor;
  return my_constructor;
}

Message::Message(AmqpClient::Channel::ptr_t channel, const AmqpClient::Envelope::ptr_t &msg_envelope) {
  m_channel = channel;
  m_envelope = msg_envelope;
}

Message::Message() {}

void Message::Ack(void) {
  m_channel->BasicAck(m_envelope);
}

bool Message::Valid(void) {
  return (m_envelope != NULL);
}

std::string Message::MessageBody(void) {
  return m_envelope->Message()->Body();
}

// Message::JsObj(const Nan::FunctionCallbackInfo<v8::Value>& info) {
//   this->Wrap(info.This());
//   info.GetReturnValue().Set(info.This());
// }


Message::~Message() {
    // dtor
}
