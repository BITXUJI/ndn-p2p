#include <opendht.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include "ndnserver.h"
int main(int argc, char **argv)
{
  try
  {
    dht::DhtRunner node;
    node.run(4222, dht::crypto::generateIdentity(), true);
    std::string prefixhead = "/example";
    std::vector<std::string> prefixChoice = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    std::srand((unsigned)time(NULL));
    int pos = rand() % 10;
    //std::string prefix=prefixhead+prefixChoice[pos];
    std::string prefix = prefixhead + prefixChoice[0];
    std::cout << "The prefix serverd by the Producer is :" << prefix << std::endl;
    std::string IPAddress = "192.168.152.140";
    //default port is 6363;
    //question:
    // from dhtrunnertester.cpp <start>
    //auto key =dht::InfoHash::get(prefix);
    dht::Value the_dht_value{(const uint8_t *)IPAddress.data(), IPAddress.size()};
    node.put(prefix, std::move(the_dht_value));
    // from dhtrunnertester.cpp <end>
    node.join();
    NdnServer producer(prefix);
    producer.run();

    return 0;
  }
  catch (const std::exception &e)
  {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
}
