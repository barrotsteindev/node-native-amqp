const addon = require('./build/Release/addon.node');

let cons = new addon.Consumer();

console.log(cons.getHostname());


for(let i=0; i<10; i++) {
  console.log(cons.getMessage());
}
