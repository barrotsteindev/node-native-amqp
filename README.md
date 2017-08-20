node-native-amqp
================

> A native high performance NodeJS AMQP module

This native module wraps the SimpleAMQPClient C++ library for nodejs to provide better performance and efficiency.

### Build Status: ###
master  | develop
------------- | -------------
[![Build Status](https://travis-ci.org/barrotsteindev/node-native-amqp.svg?branch=master)](https://travis-ci.org/barrotsteindev/node-native-amqp)  | [![Build Status](https://travis-ci.org/barrotsteindev/node-native-amqp.svg?branch=develop)](https://travis-ci.org/barrotsteindev/node-native-amqp)


The API is quite simple.

## Channel API

### Params
* uri -> pass uri to connect to broker.
* hostName, port -> pass hostName and port to connect to broker

## Functions
* Consumer -> creates a new consumer

```javascript
const amqp = require('node-native-amqp');
let channel = new amqp.Channel({'uri': 'amqp://guest:guest@localhost:5672'});
```

## Consumer API

### Params

* queue -> Queue to consume.
* routingKey -> Routing key to bind.
* timeOut -> Default is 250ms (must be 250ms or higher).
### Functions

* getMessage(cb(err, msg)) -> Callback gets an error and a Message Object.
* getMessageSync -> Return a Message object on success, otherwise throws an error.
* close -> Gracefully removes consumer, notifying the AMQP broker.

```javascript
let consumer = channel.Consumer({ 'queue': 'jobs',
                                  'routingKey': 'jobs',
                                  'timeOut': 250 });
consumer.getMessage(function(err, msg) {
    if (err) {
      return console.log('err: ' + err);
    }
      console.log(msg.value().toString());
      msg.ack();

});
```

## Message API

### Functions

* ack -> Acknowledges message. return true if the acknowledge was successful.
* reject(toReque) -> Rejects message. bool toReque defaults to true. if true message is resent to queue.
* value -> Returns a buffer containing the consumed message payload(content)

### Installing
> This library requires the following C/C++ libraries to be installed:
* boost-1.47.0 or newer.
* rabbitmq-c 0.5.1 or newer.
* SimapleAmqpClient 2.4.0 or newer.


### Examples
Examples can be found in the [examples folder](examples/)
