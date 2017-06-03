#include <iostream>
#include <stdlib.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <AMQPConsumer.h>

using namespace std;

int main()
{
    cout << "polling" << endl;
    AMQPConsumer consumer("10.0.0.8", "jobs", "jobs", true);
    const uint16_t MAX_MSGS = 1000;
    uint16_t msgs = 0;
    while (msgs < MAX_MSGS) {
        AmqpClient::BasicMessage::ptr_t msg = consumer.Poll();
        msgs++;
        std::cout << "Message number:" << msgs << " text: " << msg->Body() << std::endl;
    }
    return 0;
}
