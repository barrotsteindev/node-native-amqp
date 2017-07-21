wget https://github.com/alanxz/rabbitmq-c/releases/download/v0.8.0/rabbitmq-c-0.8.0.tar.gz
tar -xvf rabbitmq-c-0.8.0.tar.gz
cd rabbitmq-c-0.8.0
mkdir build
cd build
sudo cmake ..
sudo make install
