#include "Message.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <nan.h>

Nan::Persistent<v8::Function> Message::constructor;

void Message::Init() {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Message").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetPrototypeMethod(tpl, "value", JsValue);
  Nan::SetPrototypeMethod(tpl, "ack", JsAck);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
}


v8::Local<v8::Object> Message::NewInstance(v8::Local<v8::Value> arg) {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { arg };
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

  return scope.Escape(instance);
}

v8::Local<v8::Object> Message::V8Instance(v8::Local<v8::Value> arg) {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 2;
  v8::Local<v8::Value> argv[argc] = { Nan::New<v8::External>(&this->m_envelope),
                                      Nan::New<v8::External>(&this->m_channel) };
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

  return scope.Escape(instance);
}

void Message::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Handle<v8::External> external_envelope = v8::Handle<v8::External>::Cast(info[0]);
  v8::Handle<v8::External> external_channel = v8::Handle<v8::External>::Cast(info[1]);
  AmqpClient::Envelope::ptr_t* envelope = static_cast<AmqpClient::Envelope::ptr_t*>(external_envelope->Value());
  AmqpClient::Channel::ptr_t* channel = static_cast<AmqpClient::Channel::ptr_t*>(external_envelope->Value());
  Message* obj = new Message(channel, envelope);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

void Message::New(const Nan::FunctionCallbackInfo<v8::Value>& info, AmqpClient::Channel::ptr_t channel, const AmqpClient::Envelope::ptr_t &msg_envelope) {
  Message* obj = new Message(channel, msg_envelope);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

void Message::JsValue(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Message* msg = Nan::ObjectWrap::Unwrap<Message>(info.Holder());
  info.GetReturnValue().Set(Nan::New(msg->MessageBody()).ToLocalChecked());
}

void Message::JsAck(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Message* msg = Nan::ObjectWrap::Unwrap<Message>(info.Holder());
  msg->Ack();
  info.GetReturnValue().Set(Nan::New(true));
}

Message::Message(AmqpClient::Channel::ptr_t channel, const AmqpClient::Envelope::ptr_t &msg_envelope) {
  m_channel = channel;
  m_envelope = msg_envelope;
}

Message::Message() { }

Message::Message(AmqpClient::Channel::ptr_t* channel, const AmqpClient::Envelope::ptr_t* msg_envelope) {
  m_channel = *channel;
  m_envelope = *msg_envelope;
}

Message::Message(std::string test) {
  testing = test;
}

void Message::Ack(void) {
  m_channel->BasicAck(m_envelope);
}

bool Message::Valid(void) {
  return (m_envelope != NULL);
}

std::string Message::MessageBody(void) {
  return m_envelope->Message()->Body();
}


Message::~Message() {
    // dtor
}
