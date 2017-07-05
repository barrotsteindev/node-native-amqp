#include "AsyncConsumer.h"
#include <iostream>

ConsumerWorker::ConsumerWorker(Nan::Callback * callback,
                               AMQPConsumer & consumer)
                               : _consumer(consumer),
                               Nan::AsyncWorker(callback) {}
// Executes in worker thread
void ConsumerWorker::Execute() {
  _message = _consumer.Poll();
}
// Executes in event loop
void ConsumerWorker::HandleOKCallback() {
  if (_message == NULL) {
    v8::Local<v8::Value> argv[] = { Nan::New("Consumer time out")
                                    .ToLocalChecked(), Nan::Null() };
    callback->Call(2, argv);
  } else {
    v8::Local<v8::Object> msg_obj = _message->V8Instance();
    v8::Local<v8::Value> argv[] = { Nan::Null(), msg_obj };
    callback->Call(2, argv);
  }
}
