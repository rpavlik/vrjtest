/**
        @file
        @brief Main file - entry point.

        @date 2014

        @author
        Ryan Pavlik
        <rpavlik@iastate.edu> and <abiryan@ryand.net>
        http://academic.cleardefinition.com/
        Iowa State University Virtual Reality Applications Center
        Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "TestApp.h"

// Library/third-party includes
#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>

#include <vrj/Kernel/Kernel.h>

// Standard includes
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::endl;

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    vrj::Kernel *kernel = vrj::Kernel::instance();

    po::options_description options("VRJ Test App Options");
    options.add_options()("help,h", "display basic help message")(
        "jconf", po::value<vector<string> >(), "VR Juggler JConf file(s) to "
                                               "load.  If none specified, "
                                               "standalone.jconf is used.");

    /// Options to initialize the VR Juggler kernel.
    options.add(kernel->getClusterOptions());

    /// Claim all positional options as lua files
    po::positional_options_description pd;
    pd.add("jconf", -1);

    /// Parse command line
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(options)
                  .positional(pd)
                  .run(),
              vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << "VR Juggler Test Application" << endl;
        cout << "Usage: " << argv[0] << " [OPTIONS]..." << endl;

        cout << options << endl;

        cout << endl;
        cout << "vrjtest is maintained by Ryan Pavlik "
                "(<ryan.pavlik@gmail.com> <abiryan@ryand.net>)." << endl;
        return 1;
    }

    /// Initialize the kernel.
    kernel->init(vm);

    /// Load jconf files.
    if (vm.count("jconf")) {
        cout << "JConf files passed on command line." << endl;

        vector<string> jconfs = vm["jconf"].as<vector<string> >();

        for (vector<string>::const_iterator it = jconfs.begin(),
                                            e = jconfs.end();
             it != e; ++it) {
            // This annoying data copy required because loadConfigFile takes by
            // non-const value.
            std::string fn = *it;
            cout << "Loading " << fn << endl;
            kernel->loadConfigFile(fn);
        }
    } else {
        cout << "No JConf files passed on command line, using standalone.jconf "
                "by default" << endl << endl;
        kernel->loadConfigFile("standalone.jconf");
    }

    /// Create the app object.
    boost::scoped_ptr<TestApp> app(new TestApp());

    // Start the kernel running
    kernel->start();

    // Give the kernel an application to execute
    kernel->setApplication(app.get());

    // Keep thread alive and waiting
    kernel->waitForKernelStop();

    return 0;
}
