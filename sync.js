const addon = require('./build/Release/addon.node');

let consumer = new addon.Consumer({ 'queue': 'jobs',
                                    'routingKey': 'jobs',
                                    'timeOut': 250 });

function consumeSync() {
  let msg = consumer.getMessageSync();
  console.log(msg.value().toString());
  msg.ack();
}

console.log(consumer.getHostname());

for(let i=0; i < 10000; i++) {
  setImmediate(consumeSync);
}
