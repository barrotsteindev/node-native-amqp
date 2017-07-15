FROM node:8
RUN apt-get update
WORKDIR /opt/amqp-producer
Add ./package.json ./
RUN npm install
Add ./ ./
ENTRYPOINT ["sh", "entry-point.sh"]
