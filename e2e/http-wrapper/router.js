const express = require('express');
const router = express.Router();

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
  router.consumer.getMessage((err, msg) => {
    if (err) {
      response.status(500).send(`error: ${err}\n`);
    } else {
      msg.ack();
      response.send(`${msg.value()}\n`);
    }
  });
});

module.exports = router;
