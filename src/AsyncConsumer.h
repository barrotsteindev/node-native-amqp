#ifndef ASYNCCONSUMER_H
#define ASYNCCONSUMER_H

#include <nan.h>
#include "Message.h"
#include "AMQPConsumer.h"
template<class ChannelType> class ConsumerWorker : public Nan::AsyncWorker {
 public:
    ConsumerWorker(Nan::Callback * callback, AMQPConsumer<ChannelType> & consumer);
    void Execute();
    void HandleOKCallback();
 private:
    Message * _message;

    AMQPConsumer<ChannelType> & _consumer;
};

#endif  // ASYNCCONSUMER_H
