#ifndef AMQPCONSUMER_H
#define AMQPCONSUMER_H

#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>


class AMQPConsumer
{
    std::string broker_address;
    std::string queue_name;
    std::string routing_key;
    bool acks;

    public:
        explicit AMQPConsumer(std::string broker_address, std::string queue_name, std::string routing_key, bool false);
        explicit AMQPConsumer(std::string broker_address, std::string queue_name, std::string routing_key) { acks=true;}
        explicit AMQPConsumer(std::string broker_address, std::string queue_name) {routing_key="#"; acks=true;}
        explicit AMQPConsumer(std::string queue_name) {broker_address=""; routing_key="#", acks=true;}
        virtual ~AMQPConsumer();
        AmqpClient::Envelope::ptr_t Poll();
        void ack(const AmqpClient::Envelope::ptr_t &msg_envelope);

    protected:

    private:
        std::string m_consumer_string;
        AmqpClient::Channel::ptr_t m_channel;
        AmqpClient::Channel::ptr_t m_cur_msg;
};

#endif // AMQPCONSUMER_H
