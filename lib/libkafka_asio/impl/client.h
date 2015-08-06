//
// impl/client.h
// -------------
//
// Copyright (c) 2015 Alexander Binzberger
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CLIENT_H_B954462A_3A9C_11E5_A151_FEFF819CDC9F
#define CLIENT_H_B954462A_3A9C_11E5_A151_FEFF819CDC9F


namespace libkafka_asio
{
Client::Client(IOServiceType& io_service, const Configuration& configuration) :
  configuration_(configuration),
  io_service_(io_service)
{
  setupConnections();
  triggerLeadershipUpdate();
}

Client::~Client()
{
  Close();
}

void Client::Close()
{
  connection_leader_mapping_.clear();
  ConnectionMappingType::iterator it = connection_mapping_.begin();
  for (; it != connection_mapping_.end(); it++)
  {
    it->second->Close();
  }
}

Client::SharedConnectionType Client::getConnection(const std::string& connectionStr)
{
    ConnectionMappingType::iterator it = connection_mapping_.find(connectionStr);
  if (it == connection_mapping_.end())
  {
      return SharedConnectionType();
  }
  return it->second;
}

Client::SharedConnectionType Client::getConnection(const std::string& topic, const Int32 partition)
{
  ConnectionLeaderMappingType::iterator it = connection_leader_mapping_.find(topic);
  if (it == connection_leader_mapping_.end())
  {
      return SharedConnectionType();
  }
  ConnectionLeaderPartitionMappingType::iterator itPartition = it->second.find(partition);
  if (itPartition == it->second.end())
  {
      return SharedConnectionType();
  }
  return itPartition->second;
}

void Client::setupConnections()
{
  BrokerList& brokerList = configuration_.broker_list;
  BrokerList::iterator it = brokerList.begin();
  for (; it != brokerList.end(); it++)
  {
    //TODO improve this in the future
    Configuration connection_configuration(configuration_);
    connection_configuration.auto_connect = true; //force auto connect
    connection_configuration.broker_list.clear();
    connection_configuration.AddBroker(*it);
    SharedConnectionType connection(new SharedConnectionType::element_type(io_service_, connection_configuration));
    //TODO resolve it here first to make sure the key will match??
    connection_mapping_[ it->toString() ] = connection;
  }
}

void Client::triggerLeadershipUpdate()
{
  static ConnectionMappingType::const_iterator connection_iterator = connection_mapping_.begin();
  MetadataRequest request;
  connection_iterator->second->AsyncRequest(request, boost::bind(&Client::updateLeadership, this, ::_1, ::_2));
  //if we reach the end start form the first again
  if ( ++connection_iterator == connection_mapping_.end() )
  {
      connection_iterator = connection_mapping_.begin();
  }
}

void Client::updateLeadership(const Connection::ErrorCodeType& err,
                      const MetadataResponse::OptionalType& response)
{
  if (err || !response)
  {
    //try again
    triggerLeadershipUpdate();
    return;
  }

  //clear old data
  connection_leader_mapping_.clear();

  //TODO improve the MetadataResponse API to make it less painfull and less slow to get all relevant information
  const MetadataResponse::BrokerVector& brokers = response->brokers();
  const MetadataResponse::TopicVector& topics = response->topics();

  MetadataResponse::TopicVector::const_iterator topic_iter = topics.begin();
  for (; topic_iter != topics.end(); topic_iter++)
  {
    MetadataResponse::Topic::PartitionVector::const_iterator partition_iter = topic_iter->partitions.begin();
    for (; partition_iter != topic_iter->partitions.end(); partition_iter++)
    {
      MetadataResponse::BrokerVector::const_iterator broker_iter = detail::FindBrokerById(partition_iter->leader, brokers);
      MetadataResponse::Broker::OptionalType leader = *broker_iter;
      if (leader)
      {
          std::stringstream ss;
          ss << leader->host << ':' << leader->port;
          //get matching connection
          SharedConnectionType connection = getConnection(ss.str());
          //save it as leader
          connection_leader_mapping_[topic_iter->topic_name][partition_iter->partition] = connection;
      }
    }
  }
}
  
}  // namespace libkafka_asio

#endif  // CLIENT_H_B954462A_3A9C_11E5_A151_FEFF819CDC9F
