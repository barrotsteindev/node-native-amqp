{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "connection.cpp", "AMQPConsumer.cpp",
                   "Message.cpp"],
      "libraries": [ "/usr/local/lib/libSimpleAmqpClient.so" ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
