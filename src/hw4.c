#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT1 2201
#define PORT2 2202 
#define BUFFER_SIZE 1024
#define MAX_SHIPS 5

//Structs
typedef struct {
    int width;
    int height;
    char **grid;
    int ship_count;
} Board;

typedef struct {
    int id;
    int ships_remaining;
    Board board;
} Player;

typedef struct {
    int type;
    int rotation;
    int column;
    int row;
} Piece;

Board *create_board(int width, int height) {
    Board *board = (Board*)malloc(sizeof(Board));
    board->width = width;
    board->height = height;
    board->ship_count = 0;

    board->grid= (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        board->grid[i] = (char *)malloc(width * sizeof(char));
    }
    return board;
}

void display_board(Board *board, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d ", board->grid[i][j]);
        }
    }
}

Board *begin(Board *board, int width, int height) {
    //Set all cells to 0
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board->grid[i][j] = 0;
        }
    }
    display_board(board, width, height);
    return board;
}

void delete_board(Board *board){
    for (int i = 0; i < board->height; i++) {
        free(board->grid[i]);
    }
    free(board->grid);
    free(board);
}

//Checks if Valid Ship Placement
int is_within_board(Board *board, int row, int column) {
    return (row >= 0 && row < board->height && column >=0 && column < board->width);
}

//Place the Ship on the Board
int initialize(Board *board, char* buffer, int width, int height) {
    int piece_type, piece_rotation, piece_column, piece_row;

    int pieces_index = 0;
    int space = 0;
    int pieces[20]; 
    int buffer_index = 2;

    while (sscanf(&buffer[buffer_index], "%d ", &pieces[pieces_index], &space) == 1) {
        pieces_index++;
        buffer_index += space; 
        if (pieces_index >= 20) {break;}
    }

    if (pieces_index < 20) {
        printf("Error");
        return 302;
    }

    for (int i = 0; i < pieces_index; i += 4) {
        int piece_type = pieces[i];
        int piece_rotation = pieces[i + 1];
        int piece_column = pieces[i + 2];
        int piece_row = pieces[i + 3];
    
        switch (piece_type) {
            case 1: //Shape 1
                if (piece_rotation == 1 || piece_rotation == 2 || piece_rotation == 3 || piece_rotation == 4) {
                    if (board->grid[piece_row][piece_column] == 1) {return -1;} 
                    if (is_within_board(board, piece_row, piece_column) == 1) {board->grid[piece_row][piece_column] = 1;} 
                    else {return -1;}

                    if (board->grid[piece_row][piece_column + 1] == 1) {return -1;} 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}

                    if (board->grid[piece_row + 1][piece_column + 0] == 1) {return -1;} 
                    if (is_within_board(board, piece_row + 1, piece_column + 0) == 1) {board->grid[piece_row + 1][piece_column + 0] = 1;}   
                    else {return -1;}

                    if (board->grid[piece_row + 1][piece_column + 1] == 1) {return -1;} 
                    if (is_within_board(board, piece_row + 1, piece_column + 1) == 1) {board->grid[piece_row + 1][piece_column + 1] = 1;}           
                    else {return -1;}
                }
                
            case 2: //Shape 2
                if (piece_rotation == 1 || piece_rotation == 3) {
                    
                    if (board->grid[piece_row][piece_column] == 1) {return -1;} 
                    if (is_within_board(board, piece_row, piece_column) == 1) {board->grid[piece_row][piece_column] = 1;} 
                    else {return -1;}

                    if (board->grid[piece_row + 1][piece_column] == 1) {return -1;} 
                    if (is_within_board(board, piece_row + 1, piece_column) == 1) {board->grid[piece_row + 1][piece_column] = 1;}    
                    else {return -1;}

                    if (board->grid[piece_row + 2][piece_column] == 1) {return -1;} 
                    if (is_within_board(board, piece_row + 2, piece_column) == 1) {board->grid[piece_row + 2][piece_column] = 1;}   
                    else {return -1;}

                    if (board->grid[piece_row + 3][piece_column] == 1) {return -1;} 
                    if (is_within_board(board, piece_row + 3, piece_column) == 1) {board->grid[piece_row + 3][piece_column] = 1;}           
                    else {return -1;}
                }
                
                else if (piece_rotation == 2 || piece_rotation == 4) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row, piece_column + 1) == 1) {board->grid[piece_row][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row, piece_column + 2) == 1) {board->grid[piece_row][piece_column + 2] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row, piece_column + 3) == 1) {board->grid[piece_row][piece_column + 3] = 1;}           
                    else {return -1;}
                }

            case 3: //Shape 3
                if (piece_rotation == 1 || piece_rotation == 3) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row - 1, piece_column + 1) == 1) {board->grid[piece_row - 1][piece_column + 1] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row - 1, piece_column + 2) == 1) {board->grid[piece_row - 1][piece_column + 2] = 1;}           
                    else {return -1;}
                }
                
                else if (piece_rotation == 2 || piece_rotation == 4) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 1, piece_column + 0) == 1) {board->grid[piece_row + 1][piece_column + 0] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 1) == 1) {board->grid[piece_row + 1][piece_column + 1] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 2, piece_column + 1) == 1) {board->grid[piece_row + 2][piece_column + 1] = 1;}           
                    else {return -1;}
                }

            case 4: //Shape 4
                if (piece_rotation == 1) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 1, piece_column + 0) == 1) {board->grid[piece_row + 1][piece_column + 0] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 2, piece_column + 0) == 1) {board->grid[piece_row + 2][piece_column + 0] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 2, piece_column + 1) == 1) {board->grid[piece_row + 2][piece_column + 1] = 1;}           
                    else {return -1;}
                }

                else if (piece_rotation == 2) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 0, piece_column + 2) == 1) {board->grid[piece_row + 0][piece_column + 2] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 0) == 1) {board->grid[piece_row + 1][piece_column + 0] = 1;}           
                    else {return -1;}
                }

                else if (piece_rotation == 3) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 1) == 1) {board->grid[piece_row + 1][piece_column + 1] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 2, piece_column + 1) == 1) {board->grid[piece_row + 2][piece_column + 1] = 1;}           
                    else {return -1;}
                }

                else if (piece_rotation == 4) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 0, piece_column + 2) == 1) {board->grid[piece_row + 0][piece_column + 2] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row - 1, piece_column + 2) == 1) {board->grid[piece_row - 1][piece_column + 2] = 1;}           
                    else {return -1;}
                }

            case 5: //Shape 5
                if (piece_rotation == 1 || piece_rotation == 3) {

                    board->grid[piece_row][piece_column] = 1; 

                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 1) == 1) {board->grid[piece_row + 1][piece_column + 1] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 2) == 1) {board->grid[piece_row + 1][piece_column + 2] = 1;}           
                    else {return -1;}
                }
                
                else if (piece_rotation == 2 || piece_rotation == 4) {

                    board->grid[piece_row][piece_column] = 1; 

                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 1) == 1) {board->grid[piece_row + 1][piece_column + 1] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row - 1, piece_column + 0) == 1) {board->grid[piece_row - 1][piece_column + 0] = 1;}           
                    else {return -1;}
                }

            case 6: //Shape 6
                if (piece_rotation == 1) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row - 1, piece_column + 1) == 1) {board->grid[piece_row - 1][piece_column + 1] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row - 2, piece_column + 1) == 1) {board->grid[piece_row - 2][piece_column + 1] = 1;}           
                    else {return -1;}
                }

                else if (piece_rotation == 2) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 1, piece_column + 0) == 1) {board->grid[piece_row + 1][piece_column + 0] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 1) == 1) {board->grid[piece_row + 1][piece_column + 1] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 2) == 1) {board->grid[piece_row + 1][piece_column + 2] = 1;}           
                    else {return -1;}
                }

                else if (piece_rotation == 3) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 0) == 1) {board->grid[piece_row + 1][piece_column + 0] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 2, piece_column + 0) == 1) {board->grid[piece_row + 2][piece_column + 0] = 1;}           
                    else {return -1;}
                }

                else if (piece_rotation == 4) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 0, piece_column + 2) == 1) {board->grid[piece_row + 0][piece_column + 2] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 2) == 1) {board->grid[piece_row + 1][piece_column + 2] = 1;}           
                    else {return -1;}
                }

            case 7: //Shape 7
                if (piece_rotation == 1) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 0, piece_column + 2) == 1) {board->grid[piece_row + 0][piece_column + 2] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 1) == 1) {board->grid[piece_row + 1][piece_column + 1] = 1;}           
                    else {return -1;}
                }

                else if (piece_rotation == 2) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row - 1, piece_column + 1) == 1) {board->grid[piece_row - 1][piece_column + 1] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 1) == 1) {board->grid[piece_row + 1][piece_column + 1] = 1;}           
                    else {return -1;}
                }

                else if (piece_rotation == 3) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 0, piece_column + 1) == 1) {board->grid[piece_row + 0][piece_column + 1] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 0, piece_column + 2) == 1) {board->grid[piece_row + 0][piece_column + 2] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row - 1, piece_column + 1) == 1) {board->grid[piece_row - 1][piece_column + 1] = 1;}           
                    else {return -1;}
                }

                else if (piece_rotation == 4) {
                    board->grid[piece_row][piece_column] = 1; 
                    if (is_within_board(board, piece_row + 1, piece_column + 0) == 1) {board->grid[piece_row + 1][piece_column + 0] = 1;}    
                    else {return -1;}
                    if (is_within_board(board, piece_row + 2, piece_column + 0) == 1) {board->grid[piece_row + 2][piece_column + 0] = 1;}   
                    else {return -1;}
                    if (is_within_board(board, piece_row + 1, piece_column + 1) == 1) {board->grid[piece_row + 1][piece_column + 1] = 1;}           
                    else {return -1;}
                }
        }
    }
    
    //switch statement (nested) one for Shape and one for Rotation
    //check if piece overlaps

}

//Shoot
//Query
//Forfeit
//Error
//Halt
//Acknowledgment

int main() {
    int p1_listen_fd, p1_conn_fd, p2_listen_fd, p2_conn_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    Board *game_board = NULL;

    // Create socket for Player 1
    if ((p1_listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed for Player 1");
        exit(EXIT_FAILURE);
    }

    // Set socket options for Player 1
    if (setsockopt(p1_listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
        exit(EXIT_FAILURE);
    }

    // Bind socket for Player 1
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT1);
    if (bind(p1_listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("[Server] bind() failed for Player 1");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections for Player 1
    if (listen(p1_listen_fd, 3) < 0) {
        perror("[Server] listen() failed for Player 1.");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Running on port %d for Player 1\n", PORT1);

    // Create socket for Player 2
    if ((p2_listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed for Player 2");
        exit(EXIT_FAILURE);
    }

    // Set socket options for Player 2
    if (setsockopt(p2_listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
        exit(EXIT_FAILURE);
    }

    // Bind socket for Player 2
    address.sin_port = htons(PORT2);
    if (bind(p2_listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("[Server] bind() failed for Player 2");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections for Player 2
    if (listen(p2_listen_fd, 3) < 0) {
        perror("[Server] listen() failed for Player 2.");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Running on port %d for Player 2\n", PORT2);

    // Accept incoming connection for Player 1
    if ((p1_conn_fd = accept(p1_listen_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("[Server] accept() failed for Player 1.");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection for Player 2
    if ((p2_conn_fd = accept(p1_listen_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("[Server] accept() failed for Player 2.");
        exit(EXIT_FAILURE);
    }

    //Receive message from Player 1
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int nbytes = read(p1_conn_fd, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed for Player 1.");
            exit(EXIT_FAILURE);
        }

        int width, height;
        if (sscanf(buffer, "B, %d %d", width, height) == 2) {
            game_board = create_board(width, height);
            if(!game_board) {
                perror("Failure to create board");
            }
            else {
                perror("[Server] invalid board dimensions");             
            }
        }
    }

    //Receive message from Player 2
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int nbytes2 = read(p2_conn_fd, buffer, BUFFER_SIZE);
        if (nbytes2 <= 0) {
            perror("[Server] read() failed for Player 2.");
            exit(EXIT_FAILURE);
        }

        buffer[strlen(buffer)-1] = '\0';
        if (strcmp(buffer, "B") == 0) {
            printf("[Server] Player 2 has joined the game!");
        }
        else {
            printf("[Server] Invalid join message from Player 2");
        }
            
    }

    printf("[Server] Shutting down.\n");
    close(p1_conn_fd);
    close(p1_listen_fd);
    close(p2_conn_fd);
    close(p2_listen_fd);

    return EXIT_SUCCESS;
}