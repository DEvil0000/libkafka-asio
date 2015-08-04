#ifndef BROKER_ADDRESS_H_52126CBC_3AB0_11E5_A151_FEFF819CDC9F
#define BROKER_ADDRESS_H_52126CBC_3AB0_11E5_A151_FEFF819CDC9F

#include <exception>
#include <libkafka_asio/constants.h>

namespace libkafka_asio
{
    
BrokerAddress::BrokerAddress(const std::string& str)
{
  if (str.empty())
  {
      throw std::exception();
  }
  std::string::size_type delimiter_position = str.find(':');
  if (delimiter_position != std::string::npos &&
    delimiter_position > 0 &&
    delimiter_position < str.size() - 1)
  {
    hostname = str.substr(0, delimiter_position);
    service = str.substr(delimiter_position + 1);
  }
  else
  {
    hostname = str.substr(0, delimiter_position);
    service = constants::DefaultKafkaService();
  }
  if (hostname.empty() || service.empty())
  {
    throw std::exception();
  }
}

BrokerAddress::BrokerAddress(const std::string& hostname, const std::string& service) :
  hostname(hostname),
  service(service)
{
}
  
}  // namespace libkafka_asio

#endif  // BROKER_ADDRESS_H_52126CBC_3AB0_11E5_A151_FEFF819CDC9F
