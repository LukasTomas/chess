#include "Helper.h"

#include "../../src/terminal/Logging.h"
#include "../../src/terminal/DrawingChars.h"

using namespace std;

// CONSTRUCTORS --------------------------------------------------------------------------------------------------------
Helper::Helper( const string & name, const string & description ) : _name(name), _description(description) {
    options.push_back({"help", "will show this help", false});
}


// PUBLIC METHODS ------------------------------------------------------------------------------------------------------
void Helper::AddOption( const string & option, const string & description, bool required ) {
    if ( option.length() > longestOption)
        longestOption = option.length();

    options.push_back({option, description, required});
    realOptions++;
    if ( required )
        requiredOptions++;
}

void Helper::ShowHelp() const {
    LOG_INFO << "Usage: " << _name;
    for ( const option & o : options ) {
        LOG_INFO << " | ";
        if ( !o.required )
            LOG_INFO << "[-" << o.name << "]";
        else
            LOG_INFO << "-" << o.name;
    }
    LOG_INFO << endl;

    int tabs = longestOption / TAB_SIZE;

    for ( const option & o : options ) {
        int optionTabs = tabs - o.name.length() / TAB_SIZE + 1;
        string tabsStr(optionTabs, '\t');

        LOG_INFO << "\t-" << o.name << tabsStr << " - " << o.description << endl;
    }
}

void Helper::ShowStart() const {
    LOG_NOTICE << "\t\t" << _name << endl;
    for ( int i = 0; i < 40; i++ )
        LOG_NOTICE << boxDrwaing::LIGHT_HORIZONTAL;
    LOG_NOTICE << endl; //<< endl;
}

void Helper::Check( const vector<string> & arguments ) const {
    if ( arguments.size() < requiredOptions ) {
        LOG_ERROR << "Not enough arguments: " << arguments.size() << " - expected: " << requiredOptions << endl;
        ShowHelp();
        exit(1);
    } else if ( arguments.size() > realOptions ) {
        LOG_ERROR << "Too many arguments" << endl;
        ShowHelp();
        exit(1);
    }

    if ( arguments.size() == 1 && arguments[0] == "-help" ) {
        ShowHelp();
        exit(0);
    }

    for ( int i = 0; i < arguments.size(); i++ ) {
        string arg = arguments[i];
        if ( arg[0] == '-' ) {
            bool found = false;
            for ( const option & o : options ) {
                if ( arg.substr(1) == o.name ) {
                    found = true;
                    break;
                }
            }
            if ( !found ) {
                LOG_ERROR << "Unknown option: " << arg << endl;
                ShowHelp();
                exit(1);
            }
        } else {
            LOG_ERROR << "Unsupported option: without -" << endl;
            ShowHelp();
            exit(1);
        }
    }
}
