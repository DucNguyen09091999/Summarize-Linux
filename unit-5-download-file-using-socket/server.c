#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>

#define LISTEN_BACKLOG 1
#define BUFF_SIZE 4096
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

void get_data_from_client_func(int new_socket_fd , char *argv[])
{
    int output_file_fd;
    ssize_t total_bytes_get_from_client = 0;
    ssize_t bytes_read = 0;
    ssize_t bytes_written = 0;
    char buffer[BUFF_SIZE];
    

    output_file_fd = open(argv[1],O_WRONLY|O_CREAT|O_EXCL,S_IRUSR);
    if(output_file_fd == -1)
    {
        handle_error("open()");
    }
    printf("Open or create %s successfully\n",argv[1]);

    
    for (;;)
    {
        bytes_read = read(new_socket_fd, buffer, BUFF_SIZE);
        if (bytes_read <= 0)
        {
            break;
        }

        bytes_written = write(output_file_fd, buffer, bytes_read);
        printf("writing...\n");
        if (bytes_written != bytes_read) {
            printf("bytes_written != bytes_read. Copy program failed\n");
            printf("bytes_read: %zd\n", bytes_read);
            printf("bytes_written: %zd\n", bytes_written);
            close(output_file_fd);
            exit(EXIT_FAILURE);
        }

        total_bytes_get_from_client += bytes_written;
    }

    printf("File Copy Successful, %zd bytes passed\n", total_bytes_get_from_client);

    close(output_file_fd);

    close(new_socket_fd);

}

int main(int argc, char*argv[])
{
    int len;
    int opt;
    int ret;
    int server_fd;
    int new_socket_fd;
    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;



/**
 * @brief Usage for app 
 */

    if(argc <2)
    {
        printf("Usage: ./server <output-file-name>\n");
        exit(EXIT_FAILURE);
    }


/**
 * @brief Allocate memory using memset 
 */
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));

/**
 * @brief Create master socket using sock_stream
 */

    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd == -1)
    {
        handle_error("socket()");
        fprintf(stderr, "ERROR on socket() : %s\n", strerror(errno));
    }

    printf("Master socket created\n");

/**
 * @brief advoid "address already in us" error
 */

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        handle_error("setsockopt()");
    }

/**
 * @brief declare some specify the socket cridentials
 */

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = 5000;
    serv_addr.sin_addr.s_addr =  inet_addr("192.168.153.128"); //inet_addr("192.168.5.128"); //INADDR_ANY


/**
 * @brief assign socket to server address using blind()
 */
    ret = bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if ( ret == -1)
    {
            handle_error("bind()");
    }
    printf("Blind() socket successfully\n");

/**
 * @brief listen maximum LISTEN_BACKLOG in socket queue
 */
    ret = listen(server_fd,LISTEN_BACKLOG);
    if (ret == -1)
    {
       handle_error("listen");
        
    }
/**
 * @brief Get client's info
 */
    len = sizeof(client_addr);


// while (1) remove loop because of copy only one time
// {
    printf("waiting for accept from the client\n");
    new_socket_fd  = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t *)&len); 
	if (new_socket_fd == -1)
    {
        handle_error("accept()");
    }
    printf("Accept successfully\n");
    printf("Server : got connection \n");
    printf("start to get data from client\n");
	get_data_from_client_func(new_socket_fd,argv);
    close(server_fd);
            
// }


return 0;
}