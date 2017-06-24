#!/usr/bin/env node

var amqp = require('amqplib/callback_api');
let i = 0;

amqp.connect('amqp://localhost', function(err, conn) {
  conn.createChannel(function(err, ch) {
    var q = 'jobs';

    ch.assertQueue(q, {durable: true});
    console.log(" [*] Waiting for messages in %s. To exit press CTRL+C", q);
    ch.consume(q, function(msg) {
      console.log(" [x] Received %s", msg.content.toString());
      i += 1;
      ch.ack(msg);
      if (i === 10000) {
        console.log(i);
        conn.close();
      }
    }, {noAck: false});
  });
});
