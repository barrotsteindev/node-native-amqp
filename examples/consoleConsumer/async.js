const amqp = require('../../lib/index.js');
let channel = new amqp.Channel({});
let consumer = channel.Consumer({ 'queue': 'jobs',
                                  'routingKey': 'jobs',
                                  'timeOut': 250 });

function consumeAsync(i) {
  consumer.getMessage(function(err, msg) {
    if (err) {
      return console.log('err: ' + err);
    }
    console.log(msg.value().toString());
    msg.ack();
    if(i === 9999) {
      console.log('done');
    }
  });
}

for(let i = 0; i < 1000; i++) {
  setImmediate(consumeAsync, i);
}
