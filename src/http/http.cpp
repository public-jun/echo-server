#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define SIZE (5 * 1024)

int          httpServer(int);
int          recvRequestMessage(int, char*, unsigned int);
int          parseRequestMessage(char*, char*, char*);
int          getPorcessing(char*, char*);
int          createResponseMessage(char*, int, char*, char*, unsigned int);
int          sendResponseMessage(int, char*, unsigned int);
unsigned int getFileSize(const char*);

/* ファイルサイズを取得する */
unsigned int getFileSize(const char* path)
{
    int   size, read_size;
    char  read_buf[SIZE];
    FILE* f;

    f = fopen(path, "rb"); // バイナリファイルの読み込み
    if (f == NULL)
    {
        return 0;
    }

    size = 0;
    do
    {
        read_size = fread(read_buf, 1, SIZE, f);
        size += read_size;
    } while (read_size != 0);

    fclose(f);

    return size;
}

/*
 * リクエストメッセージを受信する
 * sock: 接続済みのソケット
 * request_message : リクエストメッセージを格納するバッファへのアドレス
 * buf_size: そのバッファサイズ
 * 戻り値: 受信したデータサイズ(バイト長)
 */
int recvRequestMessage(int sock, char* request_message, unsigned int buf_size)
{
    int recv_size;

    recv_size = recv(sock, request_message, buf_size, 0);

    return recv_size;
}

int parseRequestMessage(char* method, char* target, char* request_message)
{
    char* line;
    char* tmp_method;
    char* tmp_target;

    // リクエストメッセージの1行目のみ取得
    line = strtok(request_message, "\n");

    // " "までの文字列を取得しmethodにコピー
    tmp_method = strtok(line, " ");
    if (tmp_method == NULL)
    {
        std::cout << "get method error" << std::endl;
        return -1;
    }
    strcpy(method, tmp_method);

    // 次の" "までの文字列を取得しtargetにコピー
    tmp_target = strtok(NULL, " ");
    if (tmp_target == NULL)
    {
        std::cout << "get target error" << std::endl;
        return -1;
    }
    strcpy(target, tmp_target);

    return 0;
}

/*
 * リクエストに対する処理を行う(今回はGETのみ)
 * body: ボディを格納するバッファへのアドレス
 * file_path: リクエストターゲットに対応するファイルへのパス
 * 戻り値: ステータスコード(ファイルがない場合は404)
 */
int getPorcessing(char* body, char* file_path)
{
    FILE* f;
    int   file_size;

    // ファイルサイズを取得
    file_size = getFileSize(file_path);
    if (file_size == 0)
    {
        // ファイルサイズが0やファイルが存在しない場合は404を返す
        std::cout << "getFileSize error" << std::endl;
        return 404;
    }

    // ファイルを読み込んでボディとする
    f = fopen(file_path, "r");
    fread(body, 1, file_size, f);
    fclose(f);

    return 200;
}

/*
 * レスポンスメッセージを作成する
 * response_message: レスポンスメッセージを格納するバッファへのアドレス
 * status: ステータスコード
 * header: ヘッダーフィールドを格納したバッファへのアドレス
 * body:  ボディを格納したバッファへのアドレス
 * body_size: ボディのサイズ
 * 戻り値: レスポンスメッセージのデータサイズ(バイト長)
 */
int createResponseMessage(char* response_message, int status, char* header,
                          char* body, unsigned int body_size)
{
    unsigned int no_body_len;
    unsigned int body_len;

    response_message[0] = '\0';

    if (status == 200)
    {
        // レスポンス行とヘッダーフィールドの文字列を作成
        sprintf(response_message, "HTTP/1.1 200  OK\r\n%s\r\n", header);

        no_body_len = strlen(response_message);
        body_len    = body_size;

        // ヘッダーフィールドの後ろにボディをコピー
        memcpy(&response_message[no_body_len], body, body_len);
    }
    else if (status == 404)
    {
        // レスポンス行とヘッダーフィールドの文字列を作成
        sprintf(response_message, "HTTP/1.1 404 NotFound\r\n%s\r\n", header);

        no_body_len = strlen(response_message);
        body_len    = 0;
    }
    else
    {
        // status が202,404以外は非サポート
        std::cout << "Not support status" << status << std::endl;
        return -1;
    }

    return no_body_len + body_len;
}

/*
 * レスポンスメッセージを送信する
 * sock: 接続済みのソケット
 * response_message: 送信するレスポンスメッセージへのアドレス
 * message_size: 送信するメッセージのサイズ
 * 戻り値: 送信したデータサイズ(バイト長)
 */
int sendResponseMessage(int sock, char* response_message,
                        unsigned int message_size)
{
    int send_size;

    send_size = send(sock, response_message, message_size, 0);
    std::cout << "send Response" << std::endl;
    return send_size;
}

void showMessage(char* msg, unsigned int size)
{
    // unsigned int i;

    // std::cout << "Show msg\n" << std::endl;

    // for (i = 0; i < size; ++i)
    //     std::cout << msg[i];

    // std::cout << std::endl;
}

/*
 * HTTPサーバの処理を行う関数
 * sock: 接続ずみのソケット
 * 戻り値: 0
 */
int httpServer(int sock)
{
    int          request_size, response_size;
    char         request_message[SIZE];
    char         response_message[SIZE];
    char         method[SIZE];
    char         target[SIZE];
    char         header_field[SIZE];
    char         body[SIZE];
    int          status;
    unsigned int file_size;

    // リクエストメッセージを受信
    request_size = recvRequestMessage(sock, request_message, SIZE);
    if (request_size == -1)
    {
        std::cout << "recvRequestMessage error" << std::endl;
        return 0;
    }

    if (request_size == 0)
    {
        // 受信サイズが0の場合は相手が接続を閉じていると判定
        std::cout << "connection ended" << std::endl;
        return 0;
    }

    // 受信した文字列を表示
    showMessage(request_message, request_size);

    // 受信した文字列を解析してメソッドやリクエストターゲットを取得
    if (parseRequestMessage(method, target, request_message) == -1)
    {
        std::cout << "parseRequestMessage error" << std::endl;
        return 0;
    }

    // メソッドがGET以外のステータスコードは404にする
    if (strcmp(method, "GET") == 0)
    {
        if (strcmp(target, "/") == 0)
        {
            // "/"が指定された時は/index.htmlに書き換え
            strcpy(target, "/index.html");
        }

        // GETの応答をするために必要な処理を行う
        status = getPorcessing(body, &target[1]);
    }
    else
    {
        status = 404;
    }

    // ヘッダーフィールド作成(今回はContent-Lengthのみ)
    file_size = getFileSize(&target[1]);
    sprintf(header_field, "Content-Length:%u\r\n", file_size);

    // レスポンスメッセージを作成
    response_size = createResponseMessage(response_message, status,
                                          header_field, body, file_size);
    if (response_size == -1)
    {
        std::cout << "createResponseMessage error" << std::endl;
        return 0;
    }

    // 送信するメッセージを表示
    showMessage(response_message, response_size);

    // レスポンスメッセージを送信する
    sendResponseMessage(sock, response_message, response_size);

    return 0;
}

int main(void)
{
    int                w_addr, c_sock;
    struct sockaddr_in a_addr;

    // ソケットを作成
    w_addr = socket(AF_INET, SOCK_STREAM, 0);
    if (w_addr == -1)
    {
        std::cout << "socket error" << std::endl;
        return -1;
    }

    // 構造体を全て0にリセット
    memset(&a_addr, 0, sizeof(struct sockaddr_in));

    // サーバのIPアドレスとポートの情報を設定
    a_addr.sin_family      = AF_INET;
    a_addr.sin_port        = htons((unsigned short)SERVER_PORT);
    a_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // ソケットに情報を設定
    if (bind(w_addr, (const struct sockaddr*)&a_addr, sizeof(a_addr)) == -1)
    {
        std::cout << "bind error" << std::endl;
        close(w_addr);
        return -1;
    }

    // ソケットを接続待ちに設定
    if (listen(w_addr, 3) == -1)
    {
        std::cout << "listen error" << std::endl;
        close(w_addr);
        return -1;
    }

    while (true)
    {
        // 接続要求の受け付け（接続要求くるまで待ち)
        std::cout << "Waiting connect...." << std::endl;
        c_sock = accept(w_addr, NULL, NULL);
        if (c_sock == -1)
        {
            std::cout << "accept error" << std::endl;
            close(w_addr);
            return -1;
        }
        std::cout << "Connected!" << std::endl;

        // 接続済みのソケットでデータのやり取り
        httpServer(c_sock);

        // ソケット通信をクローズ
        close(c_sock);

        // 次の接続要求の受け付けに移る
    }

    // 接続待ちソケットをクローズ
    close(w_addr);

    return 0;
}