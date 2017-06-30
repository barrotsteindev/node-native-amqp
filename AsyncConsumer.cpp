#include <nan.h>
#include "AsyncConsumer.h"
#include "Message.h"
#include "AMQPConsumer.h"


ConsumerWorker(Callback * callback, int timeout)
  : Nan::AsyncWorker(callback), _timeout(timeout) {
}
// Executes in worker thread
void Execute() {
  _message = obj->_consumer->Poll();
}
// Executes in event loop
void HandleOKCallback() {
  if (!_message->Valid()) {
    callback->Call("Consumer time out", NULL);
  } else {
    v8::Local<v8::Object> msg_obj = _message->V8Instance();
    v8::Local<Value> argv[] = { msg_obj };
    callback->Call(1, argv);
  }
}
