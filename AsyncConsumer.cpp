#include <nan.h>
#include "AsyncConsumer.h"
#include "Message.h"
#include "AMQPConsumer.h"


ConsumerWorker(Callback * callback, int timeout)
  : Nan::AsyncWorker(callback), _timeout(timeout) {
}
// Executes in worker thread
void Execute() {
}
// Executes in event loop
void HandleOKCallback() {
    Local<Array> results = New<Array>(primes.size());
    for ( unsigned int i = 0; i < primes.size(); i++ ) {
        Nan::Set(results, i, New<v8::Number>(primes[i]));
    }
    Local<Value> argv[] = { results };
    callback->Call(1, argv);
}
