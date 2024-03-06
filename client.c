#include "header.h"
#define BUF_SIZE 1024

int main() {
    // CRÉATION DE LA SOCKET
    int socket_fd;

    if ((socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("création");
        exit(EXIT_FAILURE);
    }

    // INITIALISATION DE L'ADRESSE
    struct sockaddr_un addr_serv;
    memset(&addr_serv, '\0', sizeof(struct sockaddr_un));
    addr_serv.sun_family = AF_UNIX;
    strncpy(addr_serv.sun_path, ADRESSE_SERVEUR, sizeof(addr_serv.sun_path) - 1);

    // ÉCHANGE DE DONNÉES
    char buffer[BUF_SIZE];
    printf("Entrer une chaine de caractères:\n");

    if (fgets(buffer, BUF_SIZE, stdin) != NULL) {
        if (sendto(socket_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&addr_serv, sizeof(addr_serv)) == -1) {
            perror("sendTo");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "fgets\n");
        exit(EXIT_FAILURE);
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
