//
// simple_producer.h
// ------------------
//
// Copyright (c) 2015 Alexander Binzberger
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef SIMPLE_PRODUCER_H_78B6BC58_2186_11E5_B5F7_727283247C7F
#define SIMPLE_PRODUCER_H_78B6BC58_2186_11E5_B5F7_727283247C7F

#include <deque>
#include <libkafka_asio/connection.h>
#include <libkafka_asio/producer_configuration.h>
#include <libkafka_asio/message.h>

namespace libkafka_asio
{

// Kafka Simple Producer API implementation
class SimpleProducer
{
protected:
    struct MessageTopicPartition
    {
        const Message message_;
        const String topic_;
        const Int32 partition_;

        MessageTopicPartition(const Message& message, const String& topic, const Int32& partition) :
            message_(message),
            topic_(topic),
            partition_(partition)
        {
        }
    };
    
public:
  typedef ProducerConfiguration Configuration;
  
  SimpleProducer(Connection& connection, IOServiceType& io_service, const Configuration& configuration = Configuration());

  /**
   * ??
   */
  void produce(const String& value, const String& topic, const Int32& partition);

  void produce(const Bytes& value, const String& topic, const Int32& partition);

  void produce(const String& key, const String& value, const String& topic, const Int32& partition);

  void produce(const String& key, const Bytes& value, const String& topic, const Int32& partition);

  void produce(const Bytes& key, const Bytes& value, const String& topic, const Int32& partition);

  
  /**
   * Flush any accumulated records from the producer. Blocks until all sends are complete.
   */
  void flush();
  
  /**
   * Tries to close the producer cleanly within the specified timeout. If the close does not complete within the
   * timeout (in milliseconds), fail any pending send requests and force close the producer.
   * 
   * Use a timeout of 0 to close immediately .
   * Default timeout is 1000.
   */
  void close(Int64 timeout);
  
  
protected:
    void produceMessage(const Message& message, const String& topic, const Int32& partition);

private:
    void handleRequest(const Connection::ErrorCodeType& err,
                       const ProduceResponse::OptionalType& response);
    
    void handleDeadline();
    
    Configuration configuration_;
    Connection& connection_;
    IOServiceType& io_service_;
    StrandType strand_;
    DeadlineTimerType deadline_;
    std::deque<MessageTopicPartition> buffering_queue_;
    bool shutdown_;
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/simple_producer.h>

#endif  // SIMPLE_PRODUCER_H_78B6BC58_2186_11E5_B5F7_727283247C7F
