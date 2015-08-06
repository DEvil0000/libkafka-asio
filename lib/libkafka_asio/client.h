//
// client.h
// --------
//
// Copyright (c) 2015 Alexander Binzberger
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CLIENT_H_B95449B8_3A9C_11E5_A151_FEFF819CDC9F
#define CLIENT_H_B95449B8_3A9C_11E5_A151_FEFF819CDC9F

#include <string>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/connection.h>
#include <libkafka_asio/connection_configuration.h>

namespace libkafka_asio
{

class Client :
  private boost::noncopyable
{
public:
  // Stored connection type
  typedef boost::shared_ptr<Connection> SharedConnectionType;
  typedef std::map<std::string, SharedConnectionType> ConnectionMappingType;
  typedef std::map<Int32, SharedConnectionType> ConnectionLeaderPartitionMappingType;
  typedef std::map<std::string, ConnectionLeaderPartitionMappingType > ConnectionLeaderMappingType;
    
  // Configuration type
  typedef ConnectionConfiguration Configuration;
    
  Client(IOServiceType& io_service, const Configuration& configuration = Configuration());
  
  ~Client();
    
  // Closes all the connections to the Kafka servers.
  void Close();
protected:
  SharedConnectionType getConnection(const std::string& connectionStr);
  
  SharedConnectionType getConnection(const std::string& topic, const Int32 partition);
    
private:
  void setupConnections();
    
  //TODO should get called on leader errors
  void triggerLeadershipUpdate();
  
  void updateLeadership(const Connection::ErrorCodeType& err,
                        const MetadataResponse::OptionalType& response);
  
  Configuration configuration_;
  IOServiceType& io_service_;
  ConnectionMappingType connection_mapping_;//host:port -> connection
  ConnectionLeaderMappingType connection_leader_mapping_; //topic -> partition -> connection
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/client.h>

#endif  // CLIENT_H_B95449B8_3A9C_11E5_A151_FEFF819CDC9F
