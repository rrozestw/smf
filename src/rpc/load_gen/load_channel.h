// Copyright 2017 Alexander Gallego
//
#pragma once

#include <core/shared_ptr.hh>

#include "platform/log.h"
#include "platform/macros.h"
#include "rpc/filters/lz4_filter.h"
#include "rpc/filters/zstd_filter.h"
#include "rpc/load_gen/generator_duration.h"
#include "rpc/rpc_client.h"
#include "rpc/rpc_envelope.h"

namespace smf {
namespace load_gen {

template <typename ClientService>
struct load_channel {
  using func_t =
    std::function<seastar::future<>(ClientService *, smf::rpc_envelope &&)>;
  using generator_t = std::function<smf::rpc_envelope(
    const boost::program_options::variables_map &)>;

  load_channel(uint64_t                    id,
               const char *                ip,
               uint16_t                    port,
               uint64_t                    mem,
               smf::rpc::compression_flags compression)
    : channel_id_(id) {
    smf::rpc_client_opts opts{};
    opts.server_addr             = seastar::ipv4_addr{ip, port};
    opts.memory_avail_for_client = mem;
    client                       = ClientService::make_shared(std::move(opts));
    client->enable_histogram_metrics();
    if (compression == smf::rpc::compression_flags::compression_flags_zstd) {
      client->incoming_filters().push_back(smf::zstd_decompression_filter());
      client->outgoing_filters().push_back(smf::zstd_compression_filter(1024));
    } else if (compression ==
               smf::rpc::compression_flags::compression_flags_lz4) {
      client->incoming_filters().push_back(smf::lz4_decompression_filter());
      client->outgoing_filters().push_back(smf::lz4_compression_filter(1024));
    }
  }

  SMF_DISALLOW_COPY_AND_ASSIGN(load_channel);

  load_channel(load_channel<ClientService> &&o) : client(std::move(o.client)) {}

  ~load_channel() {}

  inline auto
  get_histogram() const {
    return client->get_histogram();
  }

  seastar::future<>
  connect() {
    return client->connect();
  }
  seastar::future<>
  stop() {
    return client->stop();
  }

  seastar::future<>
  invoke(uint32_t                                     reqs,
         const boost::program_options::variables_map &opts,
         seastar::lw_shared_ptr<generator_duration>   stats,
         generator_t                                  gen,
         func_t                                       func) {
    LOG_THROW_IF(reqs == 0, "bad number of requests");
    LOG_INFO("Channel: {}. Launching {} serial reqs", channel_id_, reqs);
    // explicitly make copies of opts, gen, and func
    // happens once per reqs call
    //
    return seastar::do_for_each(
      boost::counting_iterator<uint32_t>(0),
      boost::counting_iterator<uint32_t>(reqs),
      [this, opts, stats, gen, func](uint32_t i) mutable {
        auto e = gen(opts);
        stats->total_bytes += e.size();
        return func(client.get(), std::move(e));
      });
  }
  uint64_t                           channel_id_ = 0;
  seastar::shared_ptr<ClientService> client;
};

}  // namespace load_gen
}  // namespace smf
