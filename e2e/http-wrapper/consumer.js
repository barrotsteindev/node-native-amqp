'use strict';

const nativeAmqp = require('./build/Release/native-amqp.node');
const channel = new nativeAmqp.Channel({});
let consumers = {};
let hostName = null

function getConsumer(hostName) {
  if (!(hostName in consumers)) {
    consumers.hostName = channel.Consumer({
                                            'hostName': hostName,
                                            'queue': 'jobs',
                                            'routingKey': 'jobs',
                                            'timeOut': 250
                                           });
  }
  return consumers.hostName;
}

module.exports.getConsumer = getConsumer;
