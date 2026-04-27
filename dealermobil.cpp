#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
using namespace std;

// ============================================================
//  KONSTANTA
// ============================================================
#define MAX_STR     100   // Panjang maksimal string/teks
#define MAX_LOGIN     3   // Maksimal percobaan login

// ============================================================
//  STRUCT DATA & LINKED LIST NODE
// ============================================================

struct Mobil {
    char id[MAX_STR];     
    char nama[MAX_STR];   
    char tipe[MAX_STR];   
    char warna[MAX_STR];  
    int  tahun;           
    double harga;         
    int  stok;            
};

struct Akun {
    char username[MAX_STR];     
    char password[MAX_STR];     
    char nama_lengkap[MAX_STR]; 
};

// Node untuk Linked List Mobil
struct NodeMobil {
    Mobil data;
    NodeMobil* next;
};

// Node untuk Linked List Akun
struct NodeAkun {
    Akun data;
    NodeAkun* next;
};

// ============================================================
//  DATA GLOBAL - LINKED LIST POINTERS
// ============================================================
NodeMobil* headMobil = NULL;
int jumlahMobil = 0;

NodeAkun* headAkun = NULL;
int jumlahAkun = 0;

// ============================================================
//  FUNGSI PEMBANTU LINKED LIST
// ============================================================

void insertLastMobil(NodeMobil*& head, Mobil m) {
    NodeMobil* newNode = new NodeMobil{m, NULL};
    if (!head) {
        head = newNode;
        return;
    }
    NodeMobil* curr = head;
    while (curr->next) {
        curr = curr->next;
    }
    curr->next = newNode;
}

void insertLastAkun(NodeAkun*& head, Akun a) {
    NodeAkun* newNode = new NodeAkun{a, NULL};
    if (!head) {
        head = newNode;
        return;
    }
    NodeAkun* curr = head;
    while (curr->next) {
        curr = curr->next;
    }
    curr->next = newNode;
}

void freeListMobil(NodeMobil*& head) {
    while (head) {
        NodeMobil* temp = head;
        head = head->next;
        delete temp;
    }
}

NodeMobil* copyListMobil(NodeMobil* head) {
    if (!head) return NULL;
    NodeMobil* newHead = new NodeMobil{head->data, NULL};
    NodeMobil* curr = head->next;
    NodeMobil* tail = newHead;
    while (curr) {
        tail->next = new NodeMobil{curr->data, NULL};
        tail = tail->next;
        curr = curr->next;
    }
    return newHead;
}

// Fungsi helper agar sorting tetap bisa menggunakan index (O(N) traversal per akses)
NodeMobil* getNodeAt(NodeMobil* head, int index) {
    NodeMobil* curr = head;
    for (int i = 0; i < index && curr != NULL; i++) {
        curr = curr->next;
    }
    return curr;
}

// ============================================================
//  FUNGSI-FUNGSI PEMBANTU UTILITIES
// ============================================================
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    printf("\n  Tekan Enter untuk melanjutkan...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

int main(){

    return 0;
}