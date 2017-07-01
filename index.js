const addon = require('./build/Release/addon.node');

let consumer = new addon.Consumer({ 'queue': 'jobs',
                                    'routingKey': 'jobs',
                                    'timeOut': 250 });

function consumeAsync() {
  consumer.getMessage(function(err, msg) {
    if (err) {
      console.log('err: ' + err);
    } else {
      console.log(msg.value());
      msg.ack();
    }
  });
}

for(let i = 0; i < 10000; i++) {
  setImmediate(consumeAsync);
}
