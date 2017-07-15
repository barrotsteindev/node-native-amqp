const nativeAmqp = require('./build/Release/addon.node');
let consumers = {};
let hostName = null

function getConsumer(hostName) {
  if (!(hostName in consumers)) {
    consumers.hostName = new nativeAmqp.Consumer({
                                              'hostName': hostName,
                                              'queue': 'jobs',
                                              'routingKey': 'jobs',
                                              'timeOut': 250
                                             });
  }
  return consumers.hostName;
}

module.exports.getConsumer = getConsumer;
