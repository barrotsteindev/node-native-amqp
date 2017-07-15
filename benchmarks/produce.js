///RabbitMQ nodejs library
const amqp = require('amqp');

const hostName = process.argv[2] || "localhost";
const numOfMsgs = process.argv[3] || 1000;
let connected = false;

///Create a connection to your RabbitMQ
const connection = amqp.createConnection({host: hostName});
let i = 0;

function publishRandomMsg(exc) {
  console.log('publishing');
  let randMsg = Math.random().toString(36).substr(2, 10);
  exc.publish('jobs', 'test',
                     {confirm: true, mandatory: true}, function() {
                       i++;
                       console.log('sending msg ' + Math.round(i/2))
                       if (i === (numOfMsgs * 2)) { setTimeout(() => {console.log('closing connection');
                                                    connection.disconnect();}, 1000) }

  });
}

connection.on('error', function(e) {
  console.log(e);
  if(connected) {
    setTimeout(() => {
      console.log('closing connection');
      connection.disconnect(); process.exit(0);
    }, 1000);
  }
});

///Connection is ready
connection.on('ready', function() {
  connected = true;
  connection.exchange('', {confirm: true}, function(exchange) {
    connection.queue('jobs', {'durable': true, 'autoDelete': false}, function(queue) {
      console.log('Connection established');
      while (i < numOfMsgs) {
          setImmediate(publishRandomMsg, exchange);
          i++;
      }
    });
  });
});
