/*
 * native-node-amqp - Node.js wrapper for SimpleAmqpClient C++ library

 */

'use strict';

 const amqpWrapper = require('../build/Release/native-amqp.node');
 const amqpChannel = amqpWrapper.Channel;

 module.exports = {
   Channel: amqpChannel
 };
