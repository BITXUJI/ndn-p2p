how to design a p2p-prefix distribution system?
1.generate a random prefix.
2.put the prefix to the DHT.
3.retrieve the prefix and IP from the DHT.
DHT model: prefix -IP 
because forwarder or gateway needs the next hop to forward interests and retrieve data. it needs the information of faceID for the interest name prefix to establish channel.

Server/Serverless



example:
g++ main.cpp -std=c++14 -lopendht -lgnutls -lndn-cxx -lboost_system -lboost_program_options -lboost_chrono -lboost_date_time -lboost_filesystem -lboost_thread -lboost_log -lboost_stacktrace _backtrace -o main
