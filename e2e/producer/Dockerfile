FROM node:8
RUN apt-get update
WORKDIR /opt/amqp-producer
Add ./ ./
RUN npm install
ENTRYPOINT ["sh", "entry-point.sh"]
