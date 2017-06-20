const addon = require('./build/Release/addon.node');

let cons = new addon.Consumer();

console.log(cons.getHostname());

let msg;
for(let i=0; i<10; i++) {
  msg = cons.getMessage();
  console.log(msg.value());
}
console.log(msg.ack());
