#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket. 
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFF_SIZE 4096
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
/* Chức năng chat */
void send_data_to_server_func(int server_fd, char* argv[])
{
    ssize_t total_bytes_passed = 0;
    ssize_t bytes_written = 0;
    char buffer[BUFF_SIZE];
    int input_file_fd;

    memset(buffer, '0', BUFF_SIZE);

    input_file_fd = open(argv[1],O_RDONLY);
    if(input_file_fd == -1)
    {
        handle_error("open()");
    }
    printf("Open %s successfully\n",argv[1]);

        
    for (;;)
    {
        ssize_t bytes_read = read(input_file_fd, buffer, BUFF_SIZE);
        if (bytes_read <= 0)
        {
            break;
        }

        bytes_written = write(server_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            printf("bytes_written != bytes_read failed\n");
            printf("bytes_read: %zd\n", bytes_read);
            printf("bytes_written: %zd\n", bytes_written);
            close(input_file_fd);
            exit(EXIT_FAILURE);
        }

        total_bytes_passed += bytes_written;
    }

     printf("File Copy Successful, %zd bytes copied\n", total_bytes_passed);
    close(input_file_fd);

    close(server_fd); 
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int server_fd;

    struct sockaddr_in serv_addr;
 
    char buff[20];
	
	
/**
 * @brief Usage for app 
 */

    if (argc < 2) {
        printf("Usage: ./client <file-name>\n");
        exit(1);
    }

/**
 * @brief Allocate memory using memset 
 */
    memset(&serv_addr, '0',sizeof(serv_addr));
    
	
/**
 * @brief declare some specify the socket cridentials
 */

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = 5000;

    ret = inet_pton(AF_INET, "192.168.153.128", &serv_addr.sin_addr);
    if ( ret == -1)
    {
        handle_error("inet_pton()");
    }

/**
 * @brief Create slave socket using sock_stream
 */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        handle_error("socket()");
        fprintf(stderr,"the server is down\n");
        fprintf(stderr, "ERROR on socket() : %s\n", strerror(errno));
    }
    printf("Slave socket created\n");
	
/**
 * @brief Connect slave to master socket
 */
    ret = connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if ( ret == -1)
    {
        handle_error("connect()");
    }

    send_data_to_server_func(server_fd,argv);

    return 0;
}
