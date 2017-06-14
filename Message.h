#ifndef Message_H
#define Message_H

#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>


class Message {
    public:
        explicit Message(AmqpClient::Channel::ptr_t, const AmqpClient::Envelope::ptr_t &msg_envelope);
        void Ack(void);
        bool Valid(void);
        virtual ~Message();

    protected:

    private:
        AmqpClient::Channel::ptr_t m_channel;
        AmqpClient::Envelope::ptr_t m_envelope;
};

#endif // Message_H
