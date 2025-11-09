#include <iostream>
#include <cstdio>
#include <string>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main() {
    string service = "Client.testuuid";
    string socket_path = "/tmp/.socket.VS." + service;
    
    cout << "Creating socket: " << socket_path << endl;
    
    int handle = socket(AF_UNIX, SOCK_STREAM, 0);
    if (handle == -1) {
        cerr << "Socket creation failed" << endl;
        return 1;
    }
    
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path.c_str(), sizeof(addr.sun_path) - 1);
    
    unlink(socket_path.c_str());
    if (bind(handle, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        cerr << "Bind failed" << endl;
        close(handle);
        return 1;
    }
    
    cout << "Socket created successfully: " << socket_path << endl;
    
    unlink(socket_path.c_str());
    close(handle);
    return 0;
}
