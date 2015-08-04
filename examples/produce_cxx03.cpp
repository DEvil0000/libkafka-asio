//
// examples/produce_cxx03.cpp
// --------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//
// ----------------------------------
//
// This example shows how to prepare a 'Produce' request, connect to a Kafka 
// server and send the produce request. Errors will be reported to stderr.
//

#include <iostream>
#include <boost/asio.hpp>
#include <libkafka_asio/libkafka_asio.h>

using libkafka_asio::Connection;
using libkafka_asio::ProduceRequest;
using libkafka_asio::ProduceResponse;
using libkafka_asio::SimpleProducer;
using libkafka_asio::Bytes;

int main(int argc, char **argv)
{
  Connection::Configuration configuration;
  configuration.auto_connect = true;
  configuration.client_id = "libkafka_asio_example";
  configuration.socket_timeout = 10000;
  configuration.AddBrokerFromString("192.168.8.140:9092");

  boost::asio::io_service ios;
  Connection connection(ios, configuration);

  SimpleProducer::Configuration producerConfiguration;
  SimpleProducer producer(connection, ios, producerConfiguration);
  
  Bytes one_k_message(new Bytes::element_type(1000));
  for (int i=0;i<1000;i++)
  {
      one_k_message->push_back('x');
  }
  
  for (int i=0;i<10000;i++)
  {
    producer.produce(one_k_message, "mytopic", 0);
  }

  // Let's go!
  int timeout = 1;
  ios.post(boost::bind(&SimpleProducer::close, &producer, timeout));
  ios.run();
  return 0;
}
