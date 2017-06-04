///RabbitMQ nodejs library
var amqp = require('amqp');

var hostName = process.argv[2] || "localhost";

///Create a connection to your RabbitMQ
var connection = amqp.createConnection({host: hostName});

connection.on('error', function(e) {
  console.log(e);
});

///Connection is ready
connection.on('ready', function() {
  console.log('Connection established');
  var i = 0;
  while (i < 1000) {
      connection.publish('jobs', 'test', {}, function(err) {
          console.log(i);
          if(i == 998) {
              process.exit();
          }
      });
      i++;
  }
});
