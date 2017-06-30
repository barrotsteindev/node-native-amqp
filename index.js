const addon = require('./build/Release/addon.node');

let consumer = new addon.Consumer({ 'queue': 'jobs',
                                    'routingKey': 'jobs',
                                    'timeOut': 1000 });

console.log(consumer.getHostname());

for(let i=0; i < 2; ) {
  consumer.getMessage(function(err, msg) {
    i++;
    console.log(i);
    if (err) {
      console.log('err: ' + err);
    } else {
      console.log('OK');
      // console.log(msg.value());
      // msg.ack();
    }
  });
  // let msg = consumer.getMessageSync();
  // try {
  //   console.log(msg.value());
  //   i++;
  //   msg.reject();
  //   //msg.ack();
  // } catch (e) {
  //   console.log(msg);
  // }
}
