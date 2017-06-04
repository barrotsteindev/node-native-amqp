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
  connection.queue('jobs', {durable: true, autoDelete: false}, function(q) {
    q.bind("#");
    let i = 0;
    q.subscribe(function(message) {
        i++;
        console.log(message.data.toString());
        if (i === 999) {
          process.exit();
        }
    });
  });

});
