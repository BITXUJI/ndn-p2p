

// #include <ndn-cxx/encoding/block.hpp>
// #include <ndn-cxx/encoding/tlv.hpp>
// #include <ndn-cxx/encoding/tlv-nfd.hpp>
// #include <ndn-cxx/face.hpp>
// #include <ndn-cxx/net/face-uri.hpp>
// #include <ndn-cxx/mgmt/nfd/control-command.hpp>
// #include <ndn-cxx/mgmt/control-response.hpp>
// #include <ndn-cxx/mgmt/nfd/controller.hpp>
// #include <ndn-cxx/security/key-chain.hpp>

#include <iostream>


#include "ndnclient.h"

NdnClient::NdnClient():
	m_name("/example0")
{
	//m_name=std::string("/example0");
}
NdnClient::NdnClient(const std::string& name):
	m_name(name)
{
	//m_name=name;
}

void NdnClient::run()
{
	ndn::Name interestName(m_name+std::string("/randomData"));
	interestName.appendVersion();
	ndn::Interest interest(interestName);
	interest.setCanBePrefix(false);
	interest.setMustBeFresh(true);
	interest.setInterestLifetime(ndn::time::milliseconds(6000)); // The default is 4 seconds

	std::cout << "Sending ndn::Interest " << interest << std::endl;
	m_face.expressInterest(interest,
	                       bind(&NdnClient::onData, this,  _1, _2),
	                       bind(&NdnClient::onNack, this, _1, _2),
	                       bind(&NdnClient::onTimeout, this, _1));

	// processEvents will block until the requested data is received or a timeout occurs
	m_face.processEvents();
}


void NdnClient::onData(const ndn::Interest&, const ndn::Data& data) const
{
	std::cout << "Received ndn::Data " << data << std::endl;
}

void NdnClient::onNack(const ndn::Interest&, const ndn::lp::Nack& nack) const
{
	std::cout << "Received Nack with reason " << nack.getReason() << std::endl;
}

void NdnClient::onTimeout(const ndn::Interest& interest) const
{
	std::cout << "Timeout for " << interest << std::endl;
}

