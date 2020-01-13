#include <vector>
#include <string>
#include <opendht.h>
#include <iostream>
#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/encoding/tlv.hpp>
#include <ndn-cxx/encoding/tlv-nfd.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/net/face-uri.hpp>
#include <ndn-cxx/mgmt/nfd/control-command.hpp>
#include <ndn-cxx/mgmt/control-response.hpp>
#include <ndn-cxx/mgmt/nfd/controller.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include "ndnclient.h"

void OnFibAddNextHopSuccess (const ndn::nfd::ControlParameters parameters) {
	printf("Adding next hop in fib succeeded \n");
}

void OnFibAddNextHopFailure (const ndn::mgmt::ControlResponse response) {
	printf("Adding next hop in fib failed \n");
}

// Get faceId from response and add fib entries for each prefix this IP serves
void OnFaceCreateSuccess (const ndn::nfd::ControlParameters parameters, const std::string& PrefixNameStr) {
	if (parameters.hasFaceId()) {
		unsigned long long faceId = parameters.getFaceId();
		printf("New face created. Id: %llu \n", faceId);

		//prefixList.parse();
		//for (auto prefixIterator = prefixList.elements_begin(); prefixIterator < prefixList.elements_end(); prefixIterator++) {
			// Control parameters include name and FaceId
			ndn::nfd::ControlParameters parameters;
			parameters.setFaceId(faceId);
			//ndn::Name prefixName = ndn::Name(reinterpret_cast<const char*>(prefixIterator->value()));
			ndn::Name prefixName = ndn::Name(PrefixNameStr);
			parameters.setName(prefixName);

			ndn::Face faceToNFD;
			ndn::KeyChain appKeyChain;
			ndn::nfd::Controller controller(faceToNFD, appKeyChain);
			controller.start<ndn::nfd::FibAddNextHopCommand>(parameters, &OnFibAddNextHopSuccess, &OnFibAddNextHopFailure);
			faceToNFD.processEvents();
		//}
	}
	else {
		// Assert
	}
}

void OnFaceCreateFailure (const ndn::mgmt::ControlResponse response) {
	unsigned int statusCode = response.getCode();
	if (statusCode == 409) {
		printf("Face already exists for this Uri: ");
		ndn::nfd::ControlParameters parameters;
		try {
		    parameters.wireDecode(response.getBody());
		    if (parameters.hasFaceId()) {
				unsigned long long faceId = parameters.getFaceId();
				printf("Id = %llu \n", faceId);
		    } else {
		    	// assert
		    }
		}
		catch (const ndn::tlv::Error& e) {
			printf("Error processing response \n");
		}
	} else {
		printf("Creating face failed with code %u \n", statusCode);
		std::cout<<response.getText()<<std::endl;
	}
}


int main(int argc, char** argv)
{
  try {
  	dht::DhtRunner node;
    node.run(4222,dht::crypto::generateIdentity(),true);
    //need modified 
    node.bootstrap("127.0.0.1","4222");//TODO
	node.bootstrap("localhost","4222");//TODO
    //
    //node.bootstrap();
    std::string name="/example0";
    //TODO 
    //NdnClient consumer(name);
    //TODO to check whether the face for this prefix exists.
    node.get(name,[&](const std::vector<std::shared_ptr<dht::Value>>& IPaddrs){
	for(const auto& IPaddr : IPaddrs)
		{
			//add IP to the prefix-faceID list.
			std::cout<<"found IPaddr:"<<*IPaddr<<std::endl;
			// std::string IPaddrStr {IPaddr->data.begin(),IPaddr->data.end()};
			// ndn::Face faceToNFD;
			// ndn::KeyChain appKeyChain;
			// ndn::nfd::Controller controller(faceToNFD,appKeyChain);
			// ndn::nfd::ControlParameters parameters;
			// parameters.setUri(IPaddrStr);
			// controller.start<ndn::nfd::FaceCreateCommand>(parameters,std::bind(&OnFaceCreateSuccess,_1,name),&OnFaceCreateFailure);
			// faceToNFD.processEvents();
		}
		return true;
	});
	node.join();
    // consumer.run();
    
    return 0;
  }
  catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
}
