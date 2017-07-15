sudo docker-compose up --force-recreate -d
echo waiting for services to startup...
sleep 45s
cd e2e/http-tester
echo $PWD
npm install
node index.js
rm -rf node_modules
