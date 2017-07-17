sudo docker-compose up --force-recreate -d
echo waiting for services to startup...
sleep 45s
cd e2e/http-tester
npm install
echo testing http api
node index.js
echo closing http api
curl -XDELETE http://localhost:3000/
rm -rf node_modules
sudo docker-compose down
# sudo docker ps -a | grep nodenativeamqp_ | awk '{ print $1} '| xargs sudo docker rm -f
