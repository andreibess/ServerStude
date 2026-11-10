#include "UserInterface.h"
#include "Config.h"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

Params UserInterface::parse(int argc, char** argv) {
    Params params;
    params.dbPath = Config::DEFAULT_DB_PATH;
    params.logPath = Config::DEFAULT_LOG_PATH;
    params.port = Config::DEFAULT_PORT;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Produce help message")
        ("db,d", po::value<std::string>(&params.dbPath), "Path to client DB")
        ("log,l", po::value<std::string>(&params.logPath), "Path to log file")
        ("port,p", po::value<int>(&params.port), "Listening port");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    
    if (vm.count("help")) {
        std::cout << desc << "\n";
        std::exit(0);
    }
    po::notify(vm);

    return params;
}
