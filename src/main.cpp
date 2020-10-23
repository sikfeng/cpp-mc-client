#include <easylogging++.h>
#include <string>
#include <iostream>
#include <csignal>
#include <atomic>

#include "networkclient.hpp"
#include "event.hpp"
#include "packet.hpp"


void signalHandler(int signum) {
  LOG(INFO) << "Interrupt signal (" << signum << ") received.";
	PUSH_EVENT("Disconnect", "User Interupt");
	PUSH_EVENT("Exit", "User Interupt");
	exit(0);
}

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
	el::Helpers::setThreadName("client");
	el::Loggers::reconfigureAllLoggers(loggerConfiguration);
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
	LOG(INFO) << "Logger is configured";    
}

void handleInput(std::atomic<bool>& isRunning) {
	std::string buffer;
	while (isRunning.load()) {
		std::getline(std::cin, buffer);
    if (buffer[0] == '!') {
      if (buffer == "!useitem") {
        auto packet = std::static_pointer_cast<Packet>(std::make_shared<PacketUseItem>(0));
        PUSH_EVENT("SendPacket", packet);
        LOG(INFO) << "UseItem(0)";
      } else {
        LOG(INFO) << "Unknown command!";
      }
    } else {
      PUSH_EVENT("SendChatMessage", buffer);
      LOG(INFO) << "Sent message: " << buffer;
    }
	}
}

void doStuff(std::atomic<bool>& run) {
  // autofish by spamming UseItem at afkfishing station
  // or detect fishing bob splashing sound to fish
	while (run.load()) {
    auto packet = std::static_pointer_cast<Packet>(std::make_shared<PacketUseItem>(0));
    PUSH_EVENT("SendPacket", packet);
	}
}

int main() {
  srand(time(0));
  initLogger();

  std::string server_address = "therealorange.com";
  int port = 25565;
  std::string username = "derpderpmoo";

  std::unique_ptr<NetworkClient> network_client = std::make_unique<NetworkClient>(server_address, port);
  EventListener listener;
  // network_client->ServerListPing();
  network_client->Connect(username);

	std::atomic<bool> isRunning(true);

  {
    listener.RegisterHandler("Exit", [&isRunning](const Event&) {
      isRunning.store(false);
    });

    listener.RegisterHandler("Disconnect", [&](const Event& eventData) {
      auto data = eventData.get<std::string>();
      LOG(INFO) << "Disconnected: " << data;
      network_client.reset();
    });
    
    listener.RegisterHandler("ReceivedChatMessage", [](const Event& eventData) {
      auto data = eventData.get<std::string>();
      LOG(INFO) << "CHAT: " << data;
    });

    listener.RegisterHandler("SoundEffect", [](const Event& eventData) {
      auto data = eventData.get<int>();
      LOG(INFO) << "Sound ID: " << data;
      switch(data) {
        case 73:
          // entity.fishing_bobber.splash
          LOG(INFO) << "entity.fishing_bobber.splash";
          auto packet = std::static_pointer_cast<Packet>(std::make_shared<PacketUseItem>(0));
          PUSH_EVENT("SendPacket", packet);
      }
    });
  }

  std::thread handleInputThread(handleInput, std::ref(isRunning));

	while (isRunning.load()) {
		listener.HandleAllEvents();
	}

  isRunning.store(false);
  handleInputThread.join();

  return 0;
}
