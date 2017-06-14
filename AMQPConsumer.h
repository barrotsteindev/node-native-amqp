#ifndef AMQPCONSUMER_H
#define AMQPCONSUMER_H

#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "Message.h"


class AMQPConsumer {
    std::string broker_address;
    std::string queue_name;
    std::string routing_key;
    bool m_acks;

    public:
        explicit AMQPConsumer(std::string broker_address, std::string queue_name, std::string routing_key, bool m_acks);
        explicit AMQPConsumer(std::string broker_address, std::string queue_name, std::string routing_key) {bool m_acks=true;}
        explicit AMQPConsumer(std::string broker_address, std::string queue_name) {routing_key="#"; m_acks=true;}
        explicit AMQPConsumer(std::string queue_name) {broker_address=""; routing_key="#", m_acks=true;}
        virtual ~AMQPConsumer();
        Message* Poll();
        void ack(const AmqpClient::Envelope::ptr_t &msg_envelope);

    protected:

    private:
        std::string m_consumer_string;
        AmqpClient::Channel::ptr_t m_channel;
        AmqpClient::Channel::ptr_t m_cur_msg;
};

#endif // AMQPCONSUMER_H
