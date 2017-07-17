#ifndef ASYNCCONSUMER_H
#define ASYNCCONSUMER_H

#include <nan.h>
#include "Message.h"
#include "AMQPConsumer.h"

class ConsumerWorker : public Nan::AsyncWorker {
 public:
    ConsumerWorker(Nan::Callback * callback, AMQPConsumer & consumer);
    void Execute();
    void HandleOKCallback();
 private:
    Message * _message;
    AMQPConsumer & _consumer;
};

#endif  // ASYNCCONSUMER_H
