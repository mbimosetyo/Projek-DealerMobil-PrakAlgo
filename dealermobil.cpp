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

// Membersihkan sisa input yang tertinggal di buffer keyboard
void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Mengubah semua huruf besar menjadi huruf kecil
// Contoh: "TOYOTA" -> "toyota"
void toLowerStr(const char* src, char* dst) {
    int i = 0;
    while (src[i] != '\0') {
        if (src[i] >= 'A' && src[i] <= 'Z')
            dst[i] = src[i] + 32;
        else
            dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

// Bandingkan dua string tanpa membedakan huruf besar/kecil
// Mengembalikan 0 jika sama, nonzero jika berbeda
int strCmpCI(const char* a, const char* b) {
    char la[MAX_STR], lb[MAX_STR];
    toLowerStr(a, la);
    toLowerStr(b, lb);
    return strcmp(la, lb);
}

// Mengecek apakah teks 'needle' ada di dalam 'haystack'
// Pencarian tidak membedakan huruf besar/kecil
int containsStr(const char* haystack, const char* needle) {
    char h[MAX_STR], n[MAX_STR];
    toLowerStr(haystack, h);
    toLowerStr(needle, n);
    return strstr(h, n) != NULL;
}

// Memformat angka menjadi format Rupiah
// Contoh: 200000000 -> "Rp 200.000.000"
void formatRupiah(double angka, char* hasil) {
    char buf[64];
    sprintf(buf, "%.0f", angka);

    int panjang = strlen(buf);
    int hitungTitik = 0;
    int j = 0;
    char balik[64];

    for (int i = panjang - 1; i >= 0; i--) {
        if (hitungTitik > 0 && hitungTitik % 3 == 0)
            balik[j++] = '.';
        balik[j++] = buf[i];
        hitungTitik++;
    }
    balik[j] = '\0';

    char out[64];
    int panjangBalik = strlen(balik);
    for (int i = 0; i < panjangBalik; i++)
        out[i] = balik[panjangBalik - 1 - i];
    out[panjangBalik] = '\0';

    sprintf(hasil, "Rp %s", out);
}

// Mencetak garis pembatas
void printBorder(int lebar) {
    for (int i = 0; i < lebar; i++) printf("=");
    printf("\n");
}

// Mencetak garis tipis
void printLine(int lebar) {
    for (int i = 0; i < lebar; i++) printf("-");
    printf("\n");
}

// Mencetak judul yang rata tengah dengan garis di atas dan bawah
void printTitle(const char* judul, int lebar) {
    printBorder(lebar);
    int panjangJudul = strlen(judul);
    int padding = (lebar - panjangJudul) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", judul);
    printBorder(lebar);
}

// Membuat ID otomatis untuk mobil baru
// Contoh: MB1001, MB1002, MB1003, dst.
void generateID(char* hasil) {
    sprintf(hasil, "MB%04d", jumlahMobil + 1001);
}

int main(){

    return 0;
}