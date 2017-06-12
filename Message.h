#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>


class Message
{

    public:
        explicit Message(AmqpClient::Channel::ptr_t &Channel, const AmqpClient::Envelope::ptr_t &msg_envelope);
        virtual ~Message();
        void ack(const AmqpClient::Envelope::ptr_t &msg_envelope);

    protected:

    private:
        AmqpClient::Channel::ptr_t m_channel;
};

#endif // MESSAGE_H
