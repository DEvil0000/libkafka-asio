//
// producer_configuration.h
// ------------------
//
// Copyright (c) 2015 Alexander Binzberger
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef PRODUCER_CONFIGURATION_H_88BD4CB0_2187_11E5_B5F7_727283247C7F
#define PRODUCER_CONFIGURATION_H_88BD4CB0_2187_11E5_B5F7_727283247C7F

#include <libkafka_asio/primitives.h>
#include <libkafka_asio/constants.h>

namespace libkafka_asio {

    // Producer configuration data structure

    struct ProducerConfiguration {
        /*
         * MAYBE ADDED LATER:
         * delivery report callback(s)
         * enforce.isr.cnt
         * request.timeout.ms
         * message.timeout.ms
         * produce.offset.report
         * partitioner
         */

        // Number of acknowledgments that need to be received by the server before
        // the response for this request is sent.
        // If zero (0) is specified for this parameter, the server will no wait for
        // acknowledgements. In this case, no response will be sent by the server.
        // Default value is 1
        Int16 required_acks;
        
        // Time out (in milliseconds) to wait for required acknowledgements
        // Default value is 10000
        Int32 produce_timeout;
        
        // Set max retries for sending messages.
        // Default value is 2
        Int32 max_retries;
        
        // Set retry backoff time (in milliseconds).
        // Default value is 100
        Int32 retry_backoff_time_ms;
        
        // Set max value for message buffering.
        // Default value is 100000
        Int32 queue_buffering_max_messages;
        
        // Set max wait time for message buffering.
        // Default value is 500
        Int32 queue_buffering_max_ms;
        
        // Set maximum number of messages batched in one MessageSet.
        // Default value is 1000
        Int32 batch_max_messages;
        
        // If set to true the delivery callback will only be called in case of a error.
        // Default value is true
        bool delivery_report_errors_only;
        
        // Set compression mode.
        // Default is None
        constants::Compression compression_type;
        
        // Construct using default values
        ProducerConfiguration();
    };

} // namespace libkafka_asio

#include <libkafka_asio/impl/producer_configuration.h>

#endif  // PRODUCER_CONFIGURATION_H_88BD4CB0_2187_11E5_B5F7_727283247C7F
