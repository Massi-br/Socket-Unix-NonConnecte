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

    while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
        printf("Entrer une chaine:\n");
        // Supprime le caractère de nouvelle ligne de la saisie
        buffer[strcspn(buffer, "\n")] = '\0';

        if (sendto(socket_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&addr_serv, sizeof(addr_serv)) == -1) {
            perror("sendTo");
            exit(EXIT_FAILURE);
        }

        // Attente de la réponse du serveur
        ssize_t n = recvfrom(socket_fd, buffer, BUF_SIZE, 0, NULL, NULL);
        if (n == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        // Affichage de la réponse du serveur
        printf("Server: %s\n", buffer);
    }

    // FERMETURE DE LA SOCKET
    if (close(socket_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
