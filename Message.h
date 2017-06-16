#ifndef Message_H
#define Message_H

#include <string>
#include <nan.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>


class Message: public Nan::ObjectWrap {
    public:
        static void Init();
        explicit Message(AmqpClient::Channel::ptr_t, const AmqpClient::Envelope::ptr_t &msg_envelope);
        explicit Message();
        void Ack(void);
        bool Valid(void);
        std::string MessageBody(void);
        virtual ~Message();

    protected:

    private:
        AmqpClient::Channel::ptr_t m_channel;
        AmqpClient::Envelope::ptr_t m_envelope;
        static Nan::Persistent<v8::Function> constructor;
        static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
};

#endif // Message_H
