const amqp = require('../../build/Release/native-amqp.node');

let channel = new amqp.Channel({});
let consumer = channel.Consumer({ 'queue': 'jobs',
                                  'routingKey': 'jobs',
                                  'timeOut': 250 });
function consumeSync() {
  let msg = consumer.getMessageSync();
  console.log(msg.value().toString());
  msg.ack();
}

for(let i=0; i < 1000; i++) {
  setImmediate(consumeSync);
}
