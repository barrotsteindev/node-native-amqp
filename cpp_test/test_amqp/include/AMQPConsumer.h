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
        explicit AMQPConsumer(std::string broker_address, std::string queue_name, std::string routing_key, bool acks);
        //explicit AMQPConsumer(std::string broker_address, std::string queue_name, std::string routing_key, bool true);
        //explicit AMQPConsumer(std::string broker_address, std::string queue_name, (""), (true));
        //explicit AMQPConsumer(("localhost"), std::string queue_name, (""), (true));
        virtual ~AMQPConsumer();
        AmqpClient::BasicMessage::ptr_t Poll();

    protected:

    private:
        std::string m_consumer_string;
        AmqpClient::Channel::ptr_t m_channel;
        AmqpClient::Channel::ptr_t m_cur_msg;
};

#endif // AMQPCONSUMER_H
