/*
 * Audio Playlist Management System using Linked List
 * This program demonstrates a music playlist where songs can be added, removed,
 * and played in sequence using a doubly linked list structure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a song in the playlist
typedef struct Song {
    char title[100];
    char artist[100];
    int duration; // Duration in seconds
    struct Song* next;
    struct Song* prev;
} Song;

// Structure to represent the playlist
typedef struct Playlist {
    Song* head;
    Song* tail;
    Song* current; // Currently playing song
    int totalSongs;
} Playlist;

// Function to create a new playlist
Playlist* createPlaylist() {
    Playlist* playlist = (Playlist*)malloc(sizeof(Playlist));
    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->current = NULL;
    playlist->totalSongs = 0;
    return playlist;
}

// Function to create a new song node
Song* createSong(char* title, char* artist, int duration) {
    Song* newSong = (Song*)malloc(sizeof(Song));
    strcpy(newSong->title, title);
    strcpy(newSong->artist, artist);
    newSong->duration = duration;
    newSong->next = NULL;
    newSong->prev = NULL;
    return newSong;
}

// Function to add a song to the end of the playlist
void addSong(Playlist* playlist, char* title, char* artist, int duration) {
    Song* newSong = createSong(title, artist, duration);
    
    if (playlist->head == NULL) {
        // First song in the playlist
        playlist->head = newSong;
        playlist->tail = newSong;
        playlist->current = newSong;
    } else {
        // Add to the end
        playlist->tail->next = newSong;
        newSong->prev = playlist->tail;
        playlist->tail = newSong;
    }
    
    playlist->totalSongs++;
    printf("Added: '%s' by %s to the playlist\n", title, artist);
}

// Function to remove a song from the playlist by title
void removeSong(Playlist* playlist, char* title) {
    Song* current = playlist->head;
    
    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            // Found the song to remove
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                playlist->head = current->next;
            }
            
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                playlist->tail = current->prev;
            }
            
            // Update current playing song if necessary
            if (playlist->current == current) {
                playlist->current = current->next ? current->next : playlist->head;
            }
            
            printf("Removed: '%s' from the playlist\n", title);
            free(current);
            playlist->totalSongs--;
            return;
        }
        current = current->next;
    }
    
    printf("Song '%s' not found in the playlist\n", title);
}

// Function to display all songs in the playlist
void displayPlaylist(Playlist* playlist) {
    if (playlist->head == NULL) {
        printf("Playlist is empty\n");
        return;
    }
    
    printf("\n=== PLAYLIST ===\n");
    Song* current = playlist->head;
    int index = 1;
    
    while (current != NULL) {
        printf("%d. %s - %s (%d:%02d)", 
               index, current->title, current->artist,
               current->duration / 60, current->duration % 60);
        
        if (current == playlist->current) {
            printf(" [NOW PLAYING]");
        }
        printf("\n");
        
        current = current->next;
        index++;
    }
    printf("Total songs: %d\n\n", playlist->totalSongs);
}

// Function to play the next song
void playNext(Playlist* playlist) {
    if (playlist->current == NULL) {
        printf("No songs in the playlist\n");
        return;
    }
    
    if (playlist->current->next != NULL) {
        playlist->current = playlist->current->next;
    } else {
        printf("End of playlist reached. Starting from the beginning.\n");
        playlist->current = playlist->head;
    }
    
    printf("Now playing: '%s' by %s\n", 
           playlist->current->title, playlist->current->artist);
}

// Function to play the previous song
void playPrevious(Playlist* playlist) {
    if (playlist->current == NULL) {
        printf("No songs in the playlist\n");
        return;
    }
    
    if (playlist->current->prev != NULL) {
        playlist->current = playlist->current->prev;
    } else {
        printf("Beginning of playlist reached. Going to the end.\n");
        playlist->current = playlist->tail;
    }
    
    printf("Now playing: '%s' by %s\n", 
           playlist->current->title, playlist->current->artist);
}

// Function to shuffle the playlist (simple random swap)
void shufflePlaylist(Playlist* playlist) {
    if (playlist->totalSongs < 2) {
        printf("Need at least 2 songs to shuffle\n");
        return;
    }
    
    // Convert to array for easy shuffling
    Song** songs = (Song**)malloc(playlist->totalSongs * sizeof(Song*));
    Song* current = playlist->head;
    
    for (int i = 0; i < playlist->totalSongs; i++) {
        songs[i] = current;
        current = current->next;
    }
    
    // Simple shuffle algorithm
    for (int i = playlist->totalSongs - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Song* temp = songs[i];
        songs[i] = songs[j];
        songs[j] = temp;
    }
    
    // Rebuild the linked list
    playlist->head = songs[0];
    playlist->head->prev = NULL;
    
    for (int i = 0; i < playlist->totalSongs - 1; i++) {
        songs[i]->next = songs[i + 1];
        songs[i + 1]->prev = songs[i];
    }
    
    playlist->tail = songs[playlist->totalSongs - 1];
    playlist->tail->next = NULL;
    playlist->current = playlist->head;
    
    free(songs);
    printf("Playlist shuffled!\n");
}

// Function to free the playlist memory
void freePlaylist(Playlist* playlist) {
    Song* current = playlist->head;
    while (current != NULL) {
        Song* next = current->next;
        free(current);
        current = next;
    }
    free(playlist);
}

// Main function with menu-driven interface
int main() {
    Playlist* myPlaylist = createPlaylist();
    int choice;
    char title[100], artist[100];
    int duration;
    
    printf("Welcome to Audio Playlist Manager!\n");
    
    // Add some sample songs
    addSong(myPlaylist, "Bohemian Rhapsody", "Queen", 355);
    addSong(myPlaylist, "Hotel California", "Eagles", 391);
    addSong(myPlaylist, "Imagine", "John Lennon", 183);
    addSong(myPlaylist, "Stairway to Heaven", "Led Zeppelin", 482);
    
    do {
        printf("\n=== PLAYLIST MENU ===\n");
        printf("1. Display playlist\n");
        printf("2. Add song\n");
        printf("3. Remove song\n");
        printf("4. Play next\n");
        printf("5. Play previous\n");
        printf("6. Shuffle playlist\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                displayPlaylist(myPlaylist);
                break;
                
            case 2:
                printf("Enter song title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0; // remove newline
                
                printf("Enter artist name: ");
                fgets(artist, sizeof(artist), stdin);
                artist[strcspn(artist, "\n")] = 0; // remove newline
                
                printf("Enter duration (seconds): ");
                scanf("%d", &duration);
                
                addSong(myPlaylist, title, artist, duration);
                break;
                
            case 3:
                printf("Enter song title to remove: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0; // remove newline
                
                removeSong(myPlaylist, title);
                break;
                
            case 4:
                playNext(myPlaylist);
                break;
                
            case 5:
                playPrevious(myPlaylist);
                break;
                
            case 6:
                shufflePlaylist(myPlaylist);
                break;
                
            case 7:
                printf("Thank you for using Playlist Manager!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);
    
    freePlaylist(myPlaylist);
    return 0;
}