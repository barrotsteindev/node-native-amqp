const addon = require('./build/Release/addon.node');

let cons = new addon.Consumer();

console.log(cons.getHostname());

let msg;
for(let i=0; i < 20; i++) {
  msg = cons.getMessage();
  try {
    console.log(msg.value());
    //msg.ack();
  } catch (e) {
    console.log(msg);
  }
}
