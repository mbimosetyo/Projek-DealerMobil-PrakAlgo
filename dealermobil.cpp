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

// ============================================================
//  ERROR HANDLING INPUT
// ============================================================
int bacaInt(int* hasil) {
    char buf[MAX_STR];
    if (!fgets(buf, MAX_STR, stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) == 0) return 0;
    int mulai = 0;
    if (buf[0] == '-') mulai = 1;
    if (mulai == 1 && strlen(buf) == 1) return 0;
    for (int i = mulai; buf[i] != '\0'; i++) {
        if (buf[i] < '0' || buf[i] > '9') {
            printf("  [!] Input tidak valid! Harap masukkan angka.\n");
            return 0;
        }
    }
    *hasil = atoi(buf);
    return 1;
}

int bacaDouble(double* hasil) {
    char buf[MAX_STR];
    if (!fgets(buf, MAX_STR, stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) == 0) return 0;
    int titikCount = 0;
    int mulai = 0;
    if (buf[0] == '-') mulai = 1;
    if (mulai == 1 && strlen(buf) == 1) return 0;
    for (int i = mulai; buf[i] != '\0'; i++) {
        if (buf[i] == '.') {
            titikCount++;
            if (titikCount > 1) {
                printf("  [!] Input tidak valid! Format angka salah.\n");
                return 0;
            }
        } else if (buf[i] < '0' || buf[i] > '9') {
            printf("  [!] Input tidak valid! Harap masukkan angka.\n");
            return 0;
        }
    }
    *hasil = atof(buf);
    return 1;
}

int bacaMenu(int* hasil) {
    char buf[MAX_STR];
    if (!fgets(buf, MAX_STR, stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) == 0) {
        printf("  [!] Pilihan tidak boleh kosong!\n");
        return 0;
    }
    for (int i = 0; buf[i] != '\0'; i++) {
        if (buf[i] < '0' || buf[i] > '9') {
            printf("  [!] Pilihan tidak valid! Masukkan angka menu saja.\n");
            return 0;
        }
    }
    *hasil = atoi(buf);
    return 1;
}

int bacaString(char* hasil, int maxLen, const char* labelField) {
    fgets(hasil, maxLen, stdin);
    hasil[strcspn(hasil, "\n")] = '\0';
    if (strlen(hasil) == 0) {
        printf("  [!] %s tidak boleh kosong!\n", labelField);
        return 0;
    }
    return 1;
}

void bacaStringOpsional(char* hasil, int maxLen) {
    fgets(hasil, maxLen, stdin);
    hasil[strcspn(hasil, "\n")] = '\0';
}

// ============================================================
//  FUNGSI SIMPAN DAN MUAT FILE 
// ============================================================
void simpanMobil() {
    FILE* f = fopen("mobil.dat", "w");
    if (!f) return;
    NodeMobil* curr = headMobil;
    while (curr) {
        fprintf(f, "%s|%s|%s|%s|%d|%.0f|%d\n",
                curr->data.id, curr->data.nama, curr->data.tipe, 
                curr->data.warna, curr->data.tahun, curr->data.harga, curr->data.stok);
        curr = curr->next;
    }
    fclose(f);
}

void muatMobil() {
    FILE* f = fopen("mobil.dat", "r");
    if (!f) return;
    freeListMobil(headMobil);
    jumlahMobil = 0;
    
    char baris[512];
    while (fgets(baris, sizeof(baris), f)) {
        int panjang = strlen(baris);
        if (panjang > 0 && baris[panjang - 1] == '\n') baris[panjang - 1] = '\0';
        Mobil m;
        char* tok = strtok(baris, "|"); if (!tok) continue; strcpy(m.id,    tok);
        tok = strtok(NULL, "|");        if (!tok) continue; strcpy(m.nama,  tok);
        tok = strtok(NULL, "|");        if (!tok) continue; strcpy(m.tipe,  tok);
        tok = strtok(NULL, "|");        if (!tok) continue; strcpy(m.warna, tok);
        tok = strtok(NULL, "|");        if (!tok) continue; m.tahun = atoi(tok);
        tok = strtok(NULL, "|");        if (!tok) continue; m.harga = atof(tok);
        tok = strtok(NULL, "|");        if (!tok) continue; m.stok  = atoi(tok);
        insertLastMobil(headMobil, m);
        jumlahMobil++;
    }
    fclose(f);
}

void simpanAkun() {
    FILE* f = fopen("akun.dat", "w");
    if (!f) return;
    NodeAkun* curr = headAkun;
    while (curr) {
        fprintf(f, "%s|%s|%s\n",
                curr->data.username,
                curr->data.password,
                curr->data.nama_lengkap);
        curr = curr->next;
    }
    fclose(f);
}

void muatAkun() {
    FILE* f = fopen("akun.dat", "r");
    if (!f) return;
    
    while(headAkun) {
        NodeAkun* temp = headAkun;
        headAkun = headAkun->next;
        delete temp;
    }
    jumlahAkun = 0;
    
    char baris[512];
    while (fgets(baris, sizeof(baris), f)) {
        int panjang = strlen(baris);
        if (panjang > 0 && baris[panjang - 1] == '\n') baris[panjang - 1] = '\0';
        Akun a;
        char* tok = strtok(baris, "|"); if (!tok) continue; strcpy(a.username,     tok);
        tok = strtok(NULL, "|");        if (!tok) continue; strcpy(a.password,      tok);
        tok = strtok(NULL, "|");        if (!tok) continue; strcpy(a.nama_lengkap,  tok);
        insertLastAkun(headAkun, a);
        jumlahAkun++;
    }
    fclose(f);
}

// ============================================================
//  FUNGSI CETAK TABEL MOBIL
// ============================================================
void cetakHeaderMobil() {
    printf("%-8s %-22s %-12s %-10s %-6s %-18s %-6s\n",
           "ID", "Nama Mobil", "Tipe", "Warna", "Tahun", "Harga", "Stok");
    printLine(86);
}

void cetakBarisMobil(const Mobil& m) {
    char rp[32];
    formatRupiah(m.harga, rp);
    printf("%-8s %-22s %-12s %-10s %-6d %-18s %-6d\n",
           m.id, m.nama, m.tipe, m.warna, m.tahun, rp, m.stok);
}

// ============================================================
//  FUNGSI INPUT MOBIL
// ============================================================
void inputMobil() {
    clearScreen();
    printTitle("  INPUT DATA MOBIL  ", 60);

    int jumlahInput = 0;
    printf("  Ingin menambah berapa data mobil? : ");

    while (!bacaInt(&jumlahInput) || jumlahInput <= 0) {
        printf("  [!] Masukkan angka positif: ");
    }

    for (int loop = 0; loop < jumlahInput; loop++) {
        clearScreen();
        printTitle("  INPUT DATA MOBIL  ", 60);
        printf("  Data ke-%d dari %d\n\n", loop + 1, jumlahInput);

        Mobil m;
        generateID(m.id);
        printf("  ID otomatis : %s\n\n", m.id);

        printf("  Nama Mobil  : ");
        while (!bacaString(m.nama, MAX_STR, "Nama Mobil")) {
            printf("  Nama Mobil  : ");
        }

        printf("  Tipe (Sedan/SUV/MPV/Truck/Hatchback): ");
        while (!bacaString(m.tipe, MAX_STR, "Tipe")) {
            printf("  Tipe: ");
        }

        printf("  Warna       : ");
        while (!bacaString(m.warna, MAX_STR, "Warna")) {
            printf("  Warna       : ");
        }

        printf("  Tahun       : ");
        while (!bacaInt(&m.tahun) || m.tahun < 1900 || m.tahun > 2100) {
            if (m.tahun < 1900 || m.tahun > 2100)
                printf("  [!] Tahun harus antara 1900-2100: ");
            else
                printf("  Tahun       : ");
        }

        printf("  Harga (Rp)  : ");
        while (!bacaDouble(&m.harga) || m.harga <= 0) {
            if (m.harga <= 0)
                printf("  [!] Harga harus lebih dari 0: ");
            else
                printf("  Harga (Rp)  : ");
        }

        printf("  Stok        : ");
        while (!bacaInt(&m.stok) || m.stok < 0) {
            if (m.stok < 0)
                printf("  [!] Stok tidak boleh negatif: ");
            else
                printf("  Stok        : ");
        }

        insertLastMobil(headMobil, m);
        jumlahMobil++;
        simpanMobil();
        printf("\n  [v] Data ke-%d berhasil ditambahkan!\n", loop + 1);

        if (loop < jumlahInput - 1) {
            printf("  Lanjut ke data berikutnya...\n");
            printf("  Tekan Enter untuk melanjutkan...");
            getchar();
        }
    }

    printf("\n  [v] Total %d data berhasil ditambahkan!\n", jumlahInput);
    pauseScreen();
}

// ============================================================
//  FUNGSI OUTPUT MOBIL
// ============================================================
void outputMobil(NodeMobil* head, int n) {
    clearScreen();
    printTitle("  DATA MOBIL DEALER  ", 86);

    if (n == 0 || head == NULL) {
        printf("  Tidak ada data mobil.\n");
        pauseScreen();
        return;
    }

    cetakHeaderMobil();
    NodeMobil* curr = head;
    while(curr) {
        cetakBarisMobil(curr->data);
        curr = curr->next;
    }
    printBorder(86);
    printf("  Total: %d unit\n", n);
    pauseScreen();
}

// ============================================================
//  FUNGSI-FUNGSI SORTING (PENGURUTAN)
// ============================================================
void swapMobil(Mobil& a, Mobil& b) {
    Mobil sementara = a;
    a = b;
    b = sementara;
}

// Bandingkan dua mobil — case insensitive untuk field teks
int compareField(const Mobil& a, const Mobil& b, int field) {
    char sa[MAX_STR], sb[MAX_STR];
    switch (field) {
        case 0:
            toLowerStr(a.nama,  sa); toLowerStr(b.nama,  sb);
            return strcmp(sa, sb);
        case 1:
            toLowerStr(a.tipe,  sa); toLowerStr(b.tipe,  sb);
            return strcmp(sa, sb);
        case 2:
            toLowerStr(a.warna, sa); toLowerStr(b.warna, sb);
            return strcmp(sa, sb);
        case 3:
            return (a.tahun < b.tahun) ? -1 : (a.tahun > b.tahun) ? 1 : 0;
        case 4:
            return (a.harga < b.harga) ? -1 : (a.harga > b.harga) ? 1 : 0;
        case 5:
            return (a.stok  < b.stok)  ? -1 : (a.stok  > b.stok)  ? 1 : 0;
        default: return 0;
    }
}

bool shouldSwap(const Mobil& a, const Mobil& b, int field, bool asc) {
    int hasil = compareField(a, b, field);
    return asc ? (hasil > 0) : (hasil < 0);
}

// --- 1. Bubble Sort ---
void bubbleSort(NodeMobil* head, int n, int field, bool asc) {
    for (int i = 0; i < n - 1; i++) {
        NodeMobil* curr = head;
        for (int j = 0; j < n - i - 1; j++) {
            if (shouldSwap(curr->data, curr->next->data, field, asc)) {
                swapMobil(curr->data, curr->next->data);
            }
            curr = curr->next;
        }
    }
}

// --- 2. Selection Sort ---
void selectionSort(NodeMobil* head, int n, int field, bool asc) {
    NodeMobil* curr = head;
    while (curr != NULL) {
        NodeMobil* minNode = curr;
        NodeMobil* temp = curr->next;
        while (temp != NULL) {
            if (shouldSwap(minNode->data, temp->data, field, asc)) {
                minNode = temp;
            }
            temp = temp->next;
        }
        swapMobil(curr->data, minNode->data);
        curr = curr->next;
    }
}

int main(){

    return 0;
}