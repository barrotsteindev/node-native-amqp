/*
 * native-node-amqp - Node.js wrapper for SimpleAmqpClient C++ library

 */

 const amqpWrapper = require('../build/Release/native-amqp.node');
 const amqpConsumer = amqpWrapper.Consumer;

 module.exports = {
   Consumer: amqpConsumer
 };
