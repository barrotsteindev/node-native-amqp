#include "Message.h"

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

v8::Local<v8::Object> Message::V8Instance() {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New<v8::External>(this) };
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);
  return scope.Escape(instance);
}

void Message::New(const Nan::FunctionCallbackInfo<v8::Value> & info) {
  v8::Handle<v8::External> external_msg = v8::Handle<v8::External>::Cast
                                          (info[0]);
  Message* obj = static_cast<Message*>(external_msg->Value());
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

void Message::JsValue(const Nan::FunctionCallbackInfo<v8::Value> & info) {
  Message * msg = Nan::ObjectWrap::Unwrap<Message>(info.Holder());
  int msg_len = msg->m_envelope->Message()->getAmqpBody().len;
  void * msg_bytes = malloc(msg_len);
  memcpy(msg_bytes, msg->m_envelope->Message()->getAmqpBody().bytes, msg_len);
  info.GetReturnValue().Set(Nan::NewBuffer(static_cast<char *>(msg_bytes),
                                           msg_len).ToLocalChecked());
}

void Message::JsReject(const Nan::FunctionCallbackInfo<v8::Value> & info) {
  bool toRequeue = info[0]->IsUndefined() ? true : info[0]->BooleanValue();
  Message * msg = Nan::ObjectWrap::Unwrap<Message>(info.Holder());
  msg->Reject(toRequeue);
  msg->MakeWeak();
}

void Message::JsAck(const Nan::FunctionCallbackInfo<v8::Value> & info) {
  Message * msg = Nan::ObjectWrap::Unwrap<Message>(info.Holder());
  msg->Ack();
  msg->MakeWeak();
  info.GetReturnValue().Set(Nan::New(true));
}

Message::Message(Channel * channel,
                 const AmqpClient::Envelope::ptr_t & msg_envelope):
                 m_channel(channel), m_envelope(msg_envelope) { }

Message::Message() { }

void Message::Ack(void) {
  m_channel->GetChannel()->BasicAck(m_envelope);
}

void Message::Reject(bool requeue) {
  m_channel->GetChannel()->BasicReject(m_envelope, requeue);
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
