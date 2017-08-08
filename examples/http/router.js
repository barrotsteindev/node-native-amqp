const express = require('express');
const router = express.Router();
const nativeAmqp = require('../../lib/index.js');
const channel = new nativeAmqp.Channel({});
const consumer = channel.Consumer({ 'queue': 'jobs',
                                    'routingKey': 'jobs',
                                    'timeOut': 250 });
router.consumer = consumer;

router.all('/', (req, res, next) => {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "X-Requested-With");
  next();
 });

router.delete('/', (req, res) => {
  res.send('consumer closed\n');
  process.send({'command': 'exit'});
});

router.get('/', (request, response) => {
  consumer.getMessage((err, msg) => {
    if (err) {
      response.send(`error: ${err}\n`);
    } else {
      msg.ack();
      response.send(`${msg.value()}\n`);
    }
  });
});

module.exports = router;
