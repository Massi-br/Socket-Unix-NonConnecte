#include "header.h"
#define BUF_SIZE 1024

int main() {
    // CRÉATION DE LA SOCKET
    int socket_fd;

    if ((socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("création");
        exit(EXIT_FAILURE);
    }

    // AFFECTATION D'ADRESSE
    struct sockaddr_un addr_serv;
    memset(&addr_serv, '\0', sizeof(struct sockaddr_un));
    addr_serv.sun_family = AF_UNIX;
    strncpy(addr_serv.sun_path, ADRESSE_SERVEUR, sizeof(addr_serv.sun_path) - 1);

    if (bind(socket_fd, (struct sockaddr*)&addr_serv, sizeof(addr_serv)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // ÉCHANGE DE DONNÉES
    char buffer[BUF_SIZE];
    struct sockaddr_un addr_from;
    memset(&addr_from, '\0', sizeof(struct sockaddr_un));
    socklen_t addr_from_len;

    while (1) {
        ssize_t n;
        n = recvfrom(socket_fd, buffer, BUF_SIZE, 0, (struct sockaddr*)&addr_from, &addr_from_len);
        if (n == (ssize_t)-1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        if (n > 0) {
            printf("Client: %s\n", buffer);

            // Echo le message au client
            if (sendto(socket_fd, buffer, (size_t)n, 0, (struct sockaddr*)&addr_from, addr_from_len) == -1) {
                perror("sendto");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("Saisie vide, veuillez entrer du texte.\n");
        }
    }

    // FERMETURE DE LA SOCKET
    if (close(socket_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    if (unlink(ADRESSE_SERVEUR) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
