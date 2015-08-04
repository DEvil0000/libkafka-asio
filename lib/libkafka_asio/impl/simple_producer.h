//
// impl/simple_producer.h
// ----------------------
//
// Copyright (c) 2015 Alexander Binzberger
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef SIMPLE_PRODUCER_H_418E27F6_3046_11E5_A151_FEFF819CDC9F
#define SIMPLE_PRODUCER_H_418E27F6_3046_11E5_A151_FEFF819CDC9F

#include <boost/timer/timer.hpp>
#include <libkafka_asio/produce_request.h>

namespace libkafka_asio
{
    
inline SimpleProducer::SimpleProducer(Connection& connection, boost::asio::io_service& io_service, const Configuration& configuration) :
  connection_(connection),
  io_service_(io_service),
  configuration_(configuration),
  strand_(io_service_),
  deadline_(io_service_, boost::posix_time::milliseconds(configuration_.queue_buffering_max_ms)),
  shutdown_(false)
{
    //configure timer
    deadline_.async_wait(strand_.wrap(boost::bind(&SimpleProducer::handleDeadline, this)));
}

inline void SimpleProducer::produce(const String& value, const String& topic, const Int32& partition)
{
    produce("", value, topic, partition);
}

inline void SimpleProducer::produce(const Bytes& value, const String& topic, const Int32& partition)
{
    produce("", value, topic, partition);
}

inline void SimpleProducer::produce(const String& key, const String& value, const String& topic, const Int32& partition)
{
    //create message
    Message message;
    if (key.size() > 0)
    {
        message.mutable_key().reset(
          new Bytes::element_type(key.begin(), key.end()));
    }
    message.mutable_value().reset(
      new Bytes::element_type(value.begin(), value.end()));
    strand_.dispatch(boost::bind(&SimpleProducer::produceMessage, this, message, topic, partition));
}

inline void SimpleProducer::produce(const String& key, const Bytes& value, const String& topic, const Int32& partition)
{
    //create message
    Message message;
    if (key.size() > 0)
    {
        message.mutable_key().reset(
          new Bytes::element_type(key.begin(), key.end()));
    }
    message.mutable_value() = value;
    strand_.dispatch(boost::bind(&SimpleProducer::produceMessage, this, message, topic, partition));
}


inline void SimpleProducer::produce(const Bytes& key, const Bytes& value, const String& topic, const Int32& partition)
{
    //create message
    Message message;
    if (key->size() > 0)
    {
        message.mutable_key() = key;
    }
    message.mutable_value() = value;
    strand_.dispatch(boost::bind(&SimpleProducer::produceMessage, this, message, topic, partition));
}

inline void SimpleProducer::produceMessage(const Message& message, const String& topic, const Int32& partition)
{
    if (buffering_queue_.size() >= configuration_.queue_buffering_max_messages)
    {
        //TODO handle queue full
        handleDeadline();
    }
    buffering_queue_.push_back(MessageTopicPartition(message, topic, partition));
}

inline void SimpleProducer::handleRequest(const Connection::ErrorCodeType& err,
                                          const ProduceResponse::OptionalType& response)
{
  if (err)
  {
    std::cerr
      << "Error: " << boost::system::system_error(err).what()
      << std::endl;
    return;
  }
  std::cout << "Successfully produced message!" << std::endl;
}

inline void SimpleProducer::handleDeadline()
{
    while (!buffering_queue_.empty())
    {
        //setup the request
        ProduceRequest request;
        Int32 messagesInBatch = 0;
        while (messagesInBatch < configuration_.batch_max_messages && !buffering_queue_.empty())
        {
            MessageTopicPartition mtp = buffering_queue_.front();
            buffering_queue_.pop_front();
            request.AddMessage(mtp.message_, mtp.topic_, mtp.partition_);
            messagesInBatch++;
        }
        //send request if it contains data
        if (messagesInBatch)
        {
            connection_.AsyncRequest(request, boost::bind(&SimpleProducer::handleRequest, this, ::_1, ::_2));
            //the same but with io_service
            //io_service_.post(boost::bind(&Connection::AsyncRequest<ProduceRequest>, &connection_, request, (const Connection::Handler<ProduceRequest>::Type) boost::bind(&SimpleProducer::handleRequest, this, ::_1, ::_2)));
        }
    }
    
    if (!shutdown_)
    {
        // Reschedule the timer for X ms in the future:
        deadline_.expires_from_now(boost::posix_time::milliseconds(configuration_.queue_buffering_max_ms));
        // Posts the timer event
        deadline_.async_wait(strand_.wrap(boost::bind(&SimpleProducer::handleDeadline, this)));
    }
}

inline void SimpleProducer::flush()
{
    //TODO
}

inline void SimpleProducer::close(Int64 timeout)
{
    //TODO timeout + finish all stuff on io
    shutdown_ = true;
    boost::system::error_code ec;
    deadline_.cancel(ec);
}

} // namespace libkafka_asio

#endif  // SIMPLE_PRODUCER_H_418E27F6_3046_11E5_A151_FEFF819CDC9F
