const addon = require('./build/Release/addon.node');

let consumer = new addon.Consumer({ 'queue': 'jobs',
                                    'routingKey': 'jobs',
                                    'timeOut': 1000 });

console.log(consumer.getHostname());

for(let i=0; true; i++) {
  if (i === 100) {
    consumer.close();
    process.exit();
  }
  let msg = consumer.getMessageSync();
  try {
    console.log(msg.value());
    msg.reject();
    //msg.ack();
  } catch (e) {
    console.log(msg);
  }
}
