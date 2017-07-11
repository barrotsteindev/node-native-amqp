const addon = require('../../build/Release/addon.node');

let consumer = new addon.Consumer({ 'queue': 'jobs',
                                    'routingKey': 'jobs',
                                    'timeOut': 250 });

function consumeAsync(i) {
  consumer.getMessage(function(err, msg) {
    if (err) {
      console.log('err: ' + err);
    } else {
      console.log(msg.value().toString());
      msg.ack();
      if(i === 9999) {
        console.log('done');
      }
    }
  });
}

for(let i = 0; i < 10000; i++) {
  setImmediate(consumeAsync, i);
}