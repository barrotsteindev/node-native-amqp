const cluster = require('cluster');
const numCPUs = require('os').cpus().length;
const express = require('express');

if (cluster.isMaster) {
    console.log('init cluster');
    process.on('exit', (code) => {
      console.log('exit');
      for (const wId in cluster.workers) {
        cluster.workers[wId].disconnect();
      }
    });
    for (let i = 0; i < numCPUs; i++) {
        // Create a worker
        cluster.fork();
    }
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
    console.log('closed');
  });
  const app = express();
  const port = 3000;
  const router = require('./router');

  app.use('/', router);
  app.listen(port, '0.0.0.0', (err) => {
    if (err) {
      return console.log('something bad hrouterened', err)
    }
    console.log(`server is listening on ${port}`)
  });
}
