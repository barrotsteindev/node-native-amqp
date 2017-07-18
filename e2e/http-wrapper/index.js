'use strict';

const cluster = require('cluster');
const numCPUs = require('os').cpus().length;
const express = require('express');
const amqpHost = process.argv[2] || 'localhost';
const consumer = require('./consumer');

if (cluster.isMaster) {
    for (var i = 0; i < numCPUs; i++) {
        // Create a worker
        cluster.fork();
    }
    let exitWorkers = 0
    for (const id in cluster.workers) {
      cluster.workers[id].on('message', (msg) => {
        if (msg.command == 'exit') {
          for (const wId in cluster.workers) {
            cluster.workers[wId].disconnect();
          }
        }
    });
  }
} else {
  process.on('disconnect', () => {
    console.log('closing');
    router.consumer.close();
    setTimeout(() => {console.log('after');}, 10000);
    console.log('closed');
  });
  const app = express();
  const port = 3000;
  const router = require('./router');
  router.consumer = consumer.getConsumer(amqpHost);
  app.use('/', router);
  app.listen(port, '0.0.0.0', (err) => {
    if (err) {
      return console.log('something bad happened router ended', err)
    }
    console.log(`server is listening on ${port}`)
  });
}
