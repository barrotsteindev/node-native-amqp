const addon = require('./build/Release/addon.node');

let cons = new addon.Consumer();

console.log(cons.getHostname());

for(let i=0; i < 20; i++) {
  let msg = cons.getMessage();
  try {
    console.log(msg.value());
    msg.reject();
  } catch (e) {
    console.log(msg);
  }

}
