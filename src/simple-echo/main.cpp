#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <iostream>

int main()
{
    int sockfd, acceptfd;
    int len;
    char buf[1024];
    struct sockaddr_in server, client;

    // ソケットを作る
    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    // ソケットを初期化する
    memset(&server, 0, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_port = htons(7000);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));

    // クライアントからの接続を待つ
    listen(sockfd, 5);

    // クライアントとの接続を確立する
    memset(&client, 0, sizeof(client));
    len = sizeof(client);
    acceptfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&len);

    // データを読み込む
    read(acceptfd, buf, sizeof(buf));
    std::cout << "accept data: " << buf << std::endl;

    // データを書き込む
    write(acceptfd, buf, strlen(buf));

    // 接続を閉じる
    close(acceptfd);
    close(sockfd);
    return 0;
}