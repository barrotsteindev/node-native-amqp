{
  "targets": [
    {
      "target_name": "native-amqp",
      "sources": [ "<!@(ls -1 src/*.cpp)", ],
      "libraries": [ "-lSimpleAmqpClient" ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")",
        "<(module_root_dir)/",
        "/usr/lib",
        "/usr/local/lib"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ]
    }
  ]
}
