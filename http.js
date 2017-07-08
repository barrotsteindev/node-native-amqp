const cluster = require('cluster');
const numCPUs = require('os').cpus().length;
const addon = require('./build/Release/addon.node');
const express = require('express');

if (cluster.isMaster) {
    for (var i = 0; i < numCPUs; i++) {
        // Create a worker
        cluster.fork();
    }
} else {
  const app = express();
  const port = 3000;

  const consumer = new addon.Consumer({ 'queue': 'jobs',
                                        'routingKey': 'jobs',
                                        'timeOut': 250 });

  app.all('/', function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    next();
   });

  app.get('/', (request, response) => {
    consumer.getMessage(function(err, msg) {
      if (err) {
        response.send(`error: ${err}\n`);
      } else {
        msg.ack();
        response.send(`${msg.value()}\n`);
      }
    });
  });

  app.listen(port, '0.0.0.0', (err) => {
    if (err) {
      return console.log('something bad happened', err)
    }

    console.log(`server is listening on ${port}`)
  });
}
