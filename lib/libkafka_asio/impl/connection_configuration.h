#ifndef CONNECTION_CONFIGURATION_H_0435D34D_97AC_4D67_8E3A_DEBAAC218C4D
#define CONNECTION_CONFIGURATION_H_0435D34D_97AC_4D67_8E3A_DEBAAC218C4D

#include <libkafka_asio/constants.h>
#include <boost/lexical_cast.hpp>

namespace libkafka_asio
{

inline ConnectionConfiguration::ConnectionConfiguration() :
  message_max_bytes(constants::kDefaultMessageMaxBytes),
  socket_timeout(constants::kDefaultSocketTimeout),
  client_id(constants::DefaultClientId()),
  auto_connect(constants::DefaultConnectionAutoConnect())
{
}

inline void ConnectionConfiguration::AddBrokerFromString(const std::string& str)
{
  BrokerAddress broker(str);
  broker_list.push_back(broker);
}

template< typename T >
inline void ConnectionConfiguration::AddBroker(const T& broker)
{
  AddBroker(broker.host, broker.port);
}

template<typename Tx, typename Ty>
inline void ConnectionConfiguration::AddBroker(const Tx& hostname,
                                           const Ty& service)
{
  BrokerAddress broker_address(boost::lexical_cast<String>(hostname), boost::lexical_cast<String>(service));
  AddBroker(broker_address);
}

inline void ConnectionConfiguration::AddBroker(
  const BrokerAddress& broker_address)
{
  broker_list.push_back(broker_address);
}

}  // namespace libkafka_asio

#endif  // CONNECTION_CONFIGURATION_H_0435D34D_97AC_4D67_8E3A_DEBAAC218C4D
