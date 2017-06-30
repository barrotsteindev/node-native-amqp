{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "connection.cpp", "AMQPConsumer.cpp",
                   "Message.cpp", "AsyncConsumer.cpp"],
      "libraries": [ "/usr/local/lib/libSimpleAmqpClient.so" ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ]
    }
  ]
}
