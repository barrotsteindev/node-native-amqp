const addon = require('./build/Release/addon.node');

let consumer = new addon.Consumer({ 'queue': 'jobs',
                                    'routingKey': 'jobs',
                                    'timeOut': 1000 });

console.log(consumer.getHostname());

for(let i=0; true; i++) {
  // if (i === 10) {
  //   consumer.close();
  //   process.exit();
  // }
  consumer.getMessage(function(err, msg) {
    console.log("callback");
    // if (err) {
    //   console.log("err: " + err);
    // } else {
    //   console.log(msg.value());
    // }
    console.log(msg.value());
  });
  // let msg = consumer.getMessageSync();
  // try {
  //   console.log(msg.value());
  //   msg.reject();
  //   //msg.ack();
  // } catch (e) {
  //   console.log(msg);
  // }
}
