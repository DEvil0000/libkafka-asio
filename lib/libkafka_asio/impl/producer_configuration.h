//
// impl/producer_configuration.h
// ------------------
//
// Copyright (c) 2015 Alexander Binzberger
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef PRODUCER_CONFIGURATION_H_AB0CBB7A_2FBF_11E5_A151_FEFF819CDC9F
#define PRODUCER_CONFIGURATION_H_AB0CBB7A_2FBF_11E5_A151_FEFF819CDC9F

#include <libkafka_asio/constants.h>

namespace libkafka_asio
{

inline ProducerConfiguration::ProducerConfiguration() :
  required_acks(constants::kDefaultProduceRequiredAcks),
  produce_timeout(constants::kDefaultProduceTimeout),
  max_retries(constants::kDefaultProducerMaxRetries),
  retry_backoff_time_ms(constants::kDefaultProducerRetryBackoffTime),
  queue_buffering_max_messages(constants::kDefaultProducerQueueBufferingMaxMessages),
  queue_buffering_max_ms(constants::kDefaultProducerQueueBufferingMaxMs),
  batch_max_messages(constants::kDefaultProducerBatchMaxMessages),
  delivery_report_errors_only(constants::kDefaultProducerDeliveryReportErrorsOnly),
  compression_type(constants::kCompressionNone)
{
}

}  // namespace libkafka_asio

#endif  // PRODUCER_CONFIGURATION_H_AB0CBB7A_2FBF_11E5_A151_FEFF819CDC9F
