#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>

using namespace std;

bool is_int(string s)
{
    size_t i = 0;
    while (s[i])
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
        ++i;
    }
    return true;
}

int main(int ac, char **av)
{
    if (ac != 2 || !is_int(static_cast<string>(av[1])))
    {
        cerr << "[ERROR] Port is not provided via command line parameters!" << endl;
        return -1;
    }

    // ソケットを作る
    int sock_listener = socket(AF_INET, SOCK_STREAM, 0);
}