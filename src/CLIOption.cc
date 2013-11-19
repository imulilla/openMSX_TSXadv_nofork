#include "CLIOption.hh"
#include "MSXException.hh"
#include <algorithm>

using std::string;
using std::deque;

namespace openmsx {

// class CLIOption

string CLIOption::getArgument(const string& option, deque<string>& cmdLine) const
{
	if (cmdLine.empty()) {
		throw FatalError("Missing argument for option \"" + option + '\"');
	}
	string argument = std::move(cmdLine.front());
	cmdLine.pop_front();
	return argument;
}

string CLIOption::peekArgument(const deque<string>& cmdLine) const
{
	return cmdLine.empty() ? "" : cmdLine.front();
}

} // namespace openmsx
