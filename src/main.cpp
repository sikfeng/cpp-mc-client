#include <easylogging++.h>
#include <string>
#include <iostream>

#include "networkclient.hpp"

const char *getTimeSinceProgramStart(void) {
	static auto initialTime = std::chrono::steady_clock().now();
	auto now = std::chrono::steady_clock().now();
	std::chrono::duration<double> seconds = now - initialTime;
	static char buffer[30];
	sprintf(buffer, "%.2f", seconds.count());
	return buffer;
}

INITIALIZE_EASYLOGGINGPP

void initLogger() {
	el::Configurations loggerConfiguration;
	el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%startTime", std::bind(getTimeSinceProgramStart)));
	std::string format = "[%startTime][%level][%thread][%fbase]: %msg";
	loggerConfiguration.set(el::Level::Info, el::ConfigurationType::Format, format);
	loggerConfiguration.set(el::Level::Error, el::ConfigurationType::Format, format);
	loggerConfiguration.set(el::Level::Fatal, el::ConfigurationType::Format, format);
	loggerConfiguration.set(el::Level::Warning, el::ConfigurationType::Format, format);
	el::Helpers::setThreadName("Render");
	el::Loggers::reconfigureAllLoggers(loggerConfiguration);
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
	LOG(INFO) << "Logger is configured";    
}

int main() {
  srand(time(0));
  initLogger();
  try {
    std::string server_address = "therealorange.com";
    int port = 25565;
    std::string username = "derpderpmoo";
    std::unique_ptr<NetworkClient> network_client = std::make_unique<NetworkClient>(server_address, port);
    network_client->ServerListPing();
    std::cout << "yay?" << std::endl;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
