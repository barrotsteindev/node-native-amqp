FROM node:8
RUN apt-get update
WORKDIR /opt/http-tester
Add ./ ./
RUN npm install
ENTRYPOINT ["sh", "entry-point.sh"]
