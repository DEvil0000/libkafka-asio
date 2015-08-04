#ifndef BROKER_ADDRESS_H_5212688E_3AB0_11E5_A151_FEFF819CDC9F
#define BROKER_ADDRESS_H_5212688E_3AB0_11E5_A151_FEFF819CDC9F

#include <string>
#include <vector>

namespace libkafka_asio
{

// Broker address configuration data structure
struct BrokerAddress
{
  std::string hostname;
  std::string service;
  
  // Create a broker address from string.
  // If the string contain a colon, the part before the colon is interpreted
  // as hostname and the part after that character is interpreted as service
  // name.
  // Example: localhost:9092
  //
  BrokerAddress(const std::string& str);
  
  // Create a broker using the given hostname and service parameter.
  // Both will be casted to string using a lexical_cast.
  BrokerAddress(const std::string& hostname, const std::string& service);
};

// List of broker address configurations
typedef std::vector<BrokerAddress> BrokerList;

}  // namespace libkafka_asio

#include <libkafka_asio/impl/broker_address.h>

#endif  // BROKER_ADDRESS_H_5212688E_3AB0_11E5_A151_FEFF819CDC9F
