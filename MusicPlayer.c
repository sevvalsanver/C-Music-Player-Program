#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char logs[1024][1024];
int logCount = 0;



struct Song {
    char name[50];
    struct Song* next;
    struct Song* prev;
};

struct Playlist {
    int size;
    struct Song* playingSong;
    struct Song* head;
    struct Song* tail;
};


void AddLog(char* log) {
    strcpy(logs[logCount++], log);
}

struct Playlist* createPlaylist() {
    struct Playlist* l = (struct Playlist*)malloc(sizeof(struct Playlist));
    l->size = 0;
    l->head = NULL;
    l->playingSong = NULL;
    l->tail = NULL;
    return l;
}
void InsertSong(struct Playlist* plist, char type, char* songName) {
    // type : H = add initial to list
    // type : T = add end to list

    struct Song* song = (struct Song*)malloc(sizeof(struct Song));
    strcpy(song->name, songName);
    song->next = NULL;
    song->prev = NULL;


    if (type == 'H') {
        if (plist->head == NULL) {
            plist->head = song;
            plist->tail = song;
        }
        else {
            song->next = plist->head;
            plist->head->prev = song;


            plist->head = song;
        }

    }
    else if (type == 'T') {
        if (plist->tail == NULL) {
            plist->head = song;
            plist->tail = song;
        }
        else {
            plist->tail->next = song;
            song->prev = plist->tail;
            plist->tail = song;

        }

    }

    plist->size++;


}
void MoveSong(struct Playlist* plist, char type, char* moveSongName, char* refSongName) {


    struct Song* song = plist->head;
    while (song != NULL) {

        if (strcmp(song->name, moveSongName) == 0)//the song is in list
        {

            if (plist->head == song) {

                plist->head = song->next;
                plist->head->prev = NULL;


                if (plist->tail == song) {
                    plist->tail = song->prev;
                    plist->tail->next = NULL;
                }


            }
            else
                if (plist->tail == song) {

                    plist->tail = song->prev;
                    plist->tail->next = NULL;
                }
                else {

                    song->prev->next = song->next;
                    song->next->prev = song->prev;
                }
            break;
        }

        song = song->next;
    }

    if (song) {
        // type A : sonrasına
        // type B : öncesine

        struct Song* refSong = plist->head;
        while (refSong != NULL) {

            if (strcmp(refSong->name, refSongName) == 0)//girilen sarki listede varsa
            {
                if (type == 'A') {
                    song->next = refSong->next;

                    if(refSong->next)
                        refSong->next->prev = song;

                    if (plist->tail == refSong)
                        plist->tail = song;

                    refSong->next = song;
                    song->prev = refSong;

                }
                else {
                    if(refSong->prev)
                        refSong->prev->next = song;

                    if (plist->head == refSong)
                        plist->head = song;

                    song->prev = refSong->prev;

                    refSong->prev = song;
                    song->next = refSong;

                }



                return;
            }
            refSong = refSong->next;

        }
    }

}
void PlaySong(struct Playlist* plist, char type) {
    if (plist->size == 0) {
        AddLog("No Songs To Play");
        return;
    }

    if (plist->playingSong == NULL) {
        if (type == 'N')
            plist->playingSong = plist->head;
        else
            plist->playingSong = plist->tail;
    }
    else {
        if (type == 'N')
            plist->playingSong = plist->playingSong->next;
        else
            plist->playingSong = plist->playingSong->prev;

    }
    if (plist->playingSong == NULL) {

        if (type == 'N')
            plist->playingSong = plist->head;
        else
            plist->playingSong = plist->tail;

    }



    char log[128];
    sprintf(log, "Playing\t%s", plist->playingSong->name);
    AddLog(log);
}

void PrintPlaylist(struct Playlist* plist, char type) {
    // type R - reverse
    // type F - normal

    if (plist->size == 0) {
        AddLog("No Songs To Print");
        return;
    }

    struct Song* song = plist->head;
    if (type == 'R')
        song = plist->tail;
    while (song != NULL) {

        AddLog(song->name);

        if (type == 'R')
            song = song->prev;
        else
            song = song->next;

    }

}

void RemoveSong(struct Playlist* plist, char* songName) {


    struct Song* song = plist->head;
    while (song != NULL) {

        if (strcmp(song->name, songName) == 0)//girilen sarki listede varsa
        {
            if (plist->playingSong == song) {
                AddLog("Cannot Remove The Playing Song");
                AddLog("*****");
                return;
            }


            if (plist->head == song) {
                plist->head = song->next;

                if(plist->head)
                    plist->head->prev = NULL;



                if (plist->tail == song) {
                    plist->tail = song->prev;

                    if(plist->tail)
                        plist->tail->next = NULL;


                }


            }
            else if (plist->tail == song) {
                plist->tail = song->prev;
                if(plist->tail)
                    plist->tail->next = NULL;
            }else {
                song->prev->next = song->next;
                song->next->prev = song->prev;
            }







            free(song);
            plist->size--;
            return;
        }

        song = song->next;
    }


}

struct Playlist* ReversePlaylist(struct Playlist* plist) {
    struct Playlist* reversed = createPlaylist();

    struct Song* song = plist->head;
    while (song != NULL) {
        InsertSong(reversed, 'H', song->name);
        song = song->next;
    }


    if (plist->playingSong) {
        song = reversed->head;
        while (song != NULL) {
            if (strcmp(song->name, plist->playingSong->name) == 0) {
                reversed->playingSong = song;
                break;
            }


            song = song->next;
        }
    }




    return reversed;
}

int main()
{
    struct Playlist* playlist = createPlaylist();
    FILE* f = fopen("input.txt", "r");
    if (!f) {
        printf("Failed to load file\n");
        return 0;
    }

    while (!feof(f)) {
        char param0[64], param1, param2[10], param3[10];
        fscanf(f, "%s", param0);
        if (strcmp(param0, "ReversePlaylist") == 0) {
            playlist = ReversePlaylist(playlist);
        }
        else if (strcmp(param0, "InsertSong") == 0) {
            fscanf(f, "\t%c\t%s ", &param1, param2);
            InsertSong(playlist, param1, param2);
        }
        else if (strcmp(param0, "PrintPlaylist") == 0) {
            fscanf(f, "\t%c ", &param1);
            PrintPlaylist(playlist, param1);
            AddLog("*****");

        }
        else if (strcmp(param0, "PlaySong") == 0) {
            fscanf(f, "\t%c ", &param1);
            PlaySong(playlist, param1);
            AddLog("*****");
        }
        else if (strcmp(param0, "MoveSong") == 0) {
            fscanf(f, "\t%c\t%s\t%s ", &param1, param2, param3);
            MoveSong(playlist, param1, param2, param3);
        }
        else if (strcmp(param0, "RemoveSong") == 0) {
            fscanf(f, "\t%s ", param2);
            RemoveSong(playlist, param2);
        }


    }


    FILE* o = fopen("output.txt", "w");
    int k = 0;
    if (logCount > 0) {
        for (k = 0; k < logCount; k++) {
            fprintf(o, "%s\n", logs[k]);
        }
    }
    fclose(f);
    fclose(o);

    return 0;
}
