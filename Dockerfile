FROM node:8
RUN apt-get update
RUN apt-get install -y librabbitmq-dev libboost-all-dev cmake git make g++
ENV LD_LIBRARY_PATH /usr/lib:/usr/local/lib
WORKDIR /opt/build
Add ./e2e/install_amqplib.sh ./
RUN chmod +x ./install_amqplib.sh
Run ./install_amqplib.sh

WORKDIR /opt/node-native-amqp/http-wrapper
ADD ./e2e/http-wrapper/package.json /opt/node-native-amqp/http-wrapper/
ADD ./*.cpp /opt/node-native-amqp/http-wrapper/
ADD ./*.h /opt/node-native-amqp/http-wrapper/
ADD ./binding.gyp /opt/node-native-amqp/http-wrapper/
RUN npm install --unsafe-perm
ADD benchmarks/produce.js /opt/node-native-amqp/http-wrapper
ADD e2e/http-wrapper /opt/node-native-amqp/http-wrapper
ADD ./e2e/entry-point.sh .
ENTRYPOINT ["sh", "entry-point.sh"]
