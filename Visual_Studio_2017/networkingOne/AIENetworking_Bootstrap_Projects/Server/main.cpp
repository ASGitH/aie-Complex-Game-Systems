#include <iostream>
#include <string>

// RakNet
#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>	// Contains - RakNetPeerInterface class, singleton which controls how RakNet operates (connection / disconnection packets).

void handleNetworkMessages(RakNet::RakPeerInterface* _pPeerInterface) {
	RakNet::Packet* packet = nullptr;

	while (true) {
		for (packet = _pPeerInterface->Receive(); packet; _pPeerInterface->DeallocatePacket(packet), packet = _pPeerInterface->Receive()) {
			switch (packet->data[0]) {
			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "A connection is incoming.\n";
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "A client has disconnected.\n";
				break;
			case ID_CONNECTION_LOST:
				std::cout << "A client lost the connection.\n";
				break;
			default:
				std::cout << "Received a message with a unknown id: " << packet->data[0];
				break;
			}
		}
	}
}

int main()
{
	const unsigned short port = 5456;
	RakNet::RakPeerInterface* pPeerInterface = nullptr;

	// Startup the server, and start it listening to clients
	std::cout << "Starting up the server..." << std::endl;

	// Initialize the RakNet peer interface first
	pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	// Create a socket descriptor to describe this connection
	RakNet::SocketDescriptor sd(port, 0);

	// Now call startup - max of 32 connections, on the assigned port
	pPeerInterface->Startup(32, &sd, 1);
	pPeerInterface->SetMaximumIncomingConnections(32);

	handleNetworkMessages(pPeerInterface);
	// return 0;
}