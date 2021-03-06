// Copyright (c) 2016 Alexander Gallego. All rights reserved.
//
#include "smfb/smfb_command_line_options.h"

#include "platform/log.h"

namespace smf {

void
smfb_command_line_options::validate(
  const boost::program_options::variables_map &vm) {
  LOG_THROW_IF(!vm.count("write-ahead-log-dir"),
               "--write-ahead-log-dir not specified");
}

// Note: boost has to use std::string as it has no built in parser for
// seastar::sstring
void
smfb_command_line_options::add(
  boost::program_options::options_description_easy_init o) {
  namespace po = boost::program_options;

  o("port", po::value<uint16_t>()->default_value(11201), "rpc port");

  o("ip", po::value<std::string>()->default_value(""), "ip");

  o("write-ahead-log-dir", po::value<std::string>(), "log directory");

  o("log-level", po::value<std::string>()->default_value("info"),
    "debug | trace");

  o("print-rpc-histogram-on-exit", po::value<bool>()->default_value(true),
    "if false, no server_hdr.hgrm will be printed");

  o("developer", po::value<bool>()->default_value(false), "developer mode");
}


}  // end namespace smf
