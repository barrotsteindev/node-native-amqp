#include "AsyncConsumer.h"
#include <iostream>

ConsumerWorker::ConsumerWorker(Nan::Callback * callback,
                               AMQPConsumer & consumer)
                               : m_consumer(consumer),
                               Nan::AsyncWorker(callback) {}
// Executes in worker thread
void ConsumerWorker::Execute() {
  m_message = m_consumer.Poll();
}
// Executes in event loop
void ConsumerWorker::HandleOKCallback() {
  if (m_message == NULL) {
    v8::Local<v8::Value> argv[] = { Nan::New("Consumer time out")
                                    .ToLocalChecked(), Nan::Null() };
    callback->Call(2, argv);
  } else {
    v8::Local<v8::Object> msg_obj = m_message->V8Instance();
    v8::Local<v8::Value> argv[] = { Nan::Null(), msg_obj };
    callback->Call(2, argv);
  }
}
