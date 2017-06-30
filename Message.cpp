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
  Nan::SetPrototypeMethod(tpl, "reject", JsReject);

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

v8::Local<v8::Object> Message::V8Instance() {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New<v8::External>(this) };
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

  return scope.Escape(instance);
}

void Message::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Handle<v8::External> external_msg = v8::Handle<v8::External>::Cast
                                          (info[0]);
  Message* obj = static_cast<Message*>(external_msg->Value());
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

void Message::New(const Nan::FunctionCallbackInfo<v8::Value>& info,
                  AmqpClient::Channel::ptr_t channel,
                  const AmqpClient::Envelope::ptr_t &msg_envelope) {
  Message* obj = new Message(channel, msg_envelope);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

void Message::JsValue(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Message* msg = Nan::ObjectWrap::Unwrap<Message>(info.Holder());
  info.GetReturnValue().Set(Nan::New(msg->MessageBody()).ToLocalChecked());
}

void Message::JsReject(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  bool toRequeue = info[0]->IsUndefined() ? true : info[0]->BooleanValue();
  Message* msg = Nan::ObjectWrap::Unwrap<Message>(info.Holder());
  msg->Reject(toRequeue);
}

void Message::JsAck(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Message* msg = Nan::ObjectWrap::Unwrap<Message>(info.Holder());
  msg->Ack();
  info.GetReturnValue().Set(Nan::New(true));
}

Message::Message(AmqpClient::Channel::ptr_t &channel,
                 const AmqpClient::Envelope::ptr_t &msg_envelope) {
  m_channel = channel;
  m_envelope = msg_envelope;
}

Message::Message() { }

void Message::Ack(void) {
  m_channel->BasicAck(m_envelope);
}

void Message::Reject(bool requeue) {
  m_channel->BasicReject(m_envelope, requeue);
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
