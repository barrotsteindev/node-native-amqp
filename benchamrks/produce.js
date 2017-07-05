///RabbitMQ nodejs library
var amqp = require('amqp');

var hostName = process.argv[2] || "localhost";

///Create a connection to your RabbitMQ
var connection = amqp.createConnection({host: hostName});
let i = 0;

function publishRandomMsg() {
  let randMsg = Math.random().toString(36).substr(2, 10);
  connection.publish('jobs', 'test',
                     {}, function(err) {
                       console.log(i);
                       if (i + 1 === 10000) { connection.close(); }
  });
}

connection.on('error', function(e) {
  console.log(e);
});

///Connection is ready
connection.on('ready', function() {
  console.log('Connection established');
  while (i < 10000) {
      setImmediate(publishRandomMsg);
      i++;
  }
});
