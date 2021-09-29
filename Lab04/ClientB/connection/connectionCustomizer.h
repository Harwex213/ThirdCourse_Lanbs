namespace connectionCustomizer
{
    bool EstablishConnection(SOCKET clientSocket,
                             SOCKADDR_IN clientSocketInfo,
                             char* serverCallSign,
                             SOCKADDR_IN& serverSocketInfo);

    bool EstablishConnectionViaBroadcast(short port,
                                         char* serverCallSign,
                                         SOCKADDR_IN& serverSocketInfo);

    bool EstablishConnectionViaHostname(char* hostname,
                                        short port,
                                        char* serverCallSign,
                                        SOCKADDR_IN& serverSocketInfo);
}