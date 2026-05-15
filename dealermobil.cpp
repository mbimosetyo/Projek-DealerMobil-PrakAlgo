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
void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
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
int strCmpCI(const char* a, const char* b) {
    char la[MAX_STR], lb[MAX_STR];
    toLowerStr(a, la);
    toLowerStr(b, lb);
    return strcmp(la, lb);
}
int containsStr(const char* haystack, const char* needle) {
    char h[MAX_STR], n[MAX_STR];
    toLowerStr(haystack, h);
    toLowerStr(needle, n);
    return strstr(h, n) != NULL;
}
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
void printBorder(int lebar) {
    for (int i = 0; i < lebar; i++) printf("=");
    printf("\n");
}
void printLine(int lebar) {
    for (int i = 0; i < lebar; i++) printf("-");
    printf("\n");
}
void printTitle(const char* judul, int lebar) {
    printBorder(lebar);
    int panjangJudul = strlen(judul);
    int padding = (lebar - panjangJudul) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", judul);
    printBorder(lebar);
}
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
// ============================================================
//  MENU SORTING
// ============================================================
void menuSorting() {
    int pil, pilArah;

    while (1) {
        clearScreen();
        printTitle("  SORTING DATA MOBIL  ", 60);
        printf("  Urutkan berdasarkan:\n\n");
        printf("  1. Nama Mobil   (Bubble Sort)\n");
        printf("  2. Tipe Mobil   (Selection Sort)\n");
        printf("  0. Kembali\n");
        printBorder(60);
        printf("  Pilihan: ");

        if (!bacaMenu(&pil)) { pauseScreen(); continue; }
        if (pil == 0) return;
        if (pil < 1 || pil > 2) {
            printf("  [!] Pilihan harus antara 0-2!\n");
            pauseScreen();
            continue;
        }

        clearScreen();
        printTitle("  ARAH PENGURUTAN  ", 60);
        printf("  1. Ascending  (A-Z / Kecil ke Besar)\n");
        printf("  2. Descending (Z-A / Besar ke Kecil)\n");
        printf("  0. Kembali\n");
        printBorder(60);
        printf("  Pilihan: ");

        if (!bacaMenu(&pilArah)) { pauseScreen(); continue; }
        if (pilArah == 0) continue;
        if (pilArah < 1 || pilArah > 2) {
            printf("  [!] Pilihan harus 1 atau 2!\n");
            pauseScreen();
            continue;
        }

        bool asc   = (pilArah == 1);
        int  field = pil - 1;

        const char* fieldNama[] = {"Nama","Tipe","Warna","Tahun","Harga","Stok"};
        const char* metNama[]   = {"Bubble Sort","Selection Sort","Bubble Sort",
                                    "Selection Sort","Bubble Sort","Selection Sort"};
        const char* arahNama   = asc ? "Ascending" : "Descending";

        // Salin ke linked list sementara agar data asli tidak berubah
        NodeMobil* tmp = copyListMobil(headMobil);

        switch (pil) {
            case 1: bubbleSort   (tmp, jumlahMobil, field, asc); break;
            case 2: selectionSort(tmp, jumlahMobil, field, asc); break;
        }

        clearScreen();
        char judulBuf[120];
        sprintf(judulBuf, "HASIL %s by %s [%s]", metNama[field], fieldNama[field], arahNama);
        printTitle(judulBuf, 86);
        
        cetakHeaderMobil();
        NodeMobil* curr = tmp;
        while(curr) {
            cetakBarisMobil(curr->data);
            curr = curr->next;
        }
        printBorder(86);
        printf("  Total: %d unit\n", jumlahMobil);
        
        freeListMobil(tmp); // Hapus list temp dari memori
        pauseScreen();
    }
}

// ============================================================
//  FUNGSI-FUNGSI SEARCHING (PENCARIAN)
// ============================================================
int sequentialSearch(const char* kata_kunci, NodeMobil*& hasilHead) {
    int count = 0;
    NodeMobil* curr = headMobil;
    while (curr) {
        if (containsStr(curr->data.nama, kata_kunci)) {
            insertLastMobil(hasilHead, curr->data);
            count++;
        }
        curr = curr->next;
    }
    return count;
}

int binarySearch(const char* kata_kunci, NodeMobil*& hasilHead) {
    NodeMobil* tmpHead = copyListMobil(headMobil);
    bubbleSort(tmpHead, jumlahMobil, 1, true); 

    char kw[MAX_STR], fieldStr[MAX_STR];
    toLowerStr(kata_kunci, kw);

    int lo = 0, hi = jumlahMobil - 1, tengah = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        toLowerStr(getNodeAt(tmpHead, mid)->data.tipe, fieldStr);
        int cmp = strcmp(fieldStr, kw);
        if      (cmp == 0) { tengah = mid; break; }
        else if (cmp < 0)    lo = mid + 1;
        else                 hi = mid - 1;
    }

    if (tengah == -1) {
        freeListMobil(tmpHead);
        return 0;
    }

    int count = 0;
    insertLastMobil(hasilHead, getNodeAt(tmpHead, tengah)->data);
    count++;

    // Cari ke kiri
    for (int i = tengah - 1; i >= 0; i--) {
        toLowerStr(getNodeAt(tmpHead, i)->data.tipe, fieldStr);
        if (strcmp(fieldStr, kw) == 0) { insertLastMobil(hasilHead, getNodeAt(tmpHead, i)->data); count++; }
        else break;
    }
    // Cari ke kanan
    for (int i = tengah + 1; i < jumlahMobil; i++) {
        toLowerStr(getNodeAt(tmpHead, i)->data.tipe, fieldStr);
        if (strcmp(fieldStr, kw) == 0) { insertLastMobil(hasilHead, getNodeAt(tmpHead, i)->data); count++; }
        else break;
    }
    
    freeListMobil(tmpHead);
    return count;
}

// ============================================================
//  MENU SEARCHING
// ============================================================
void menuSearching() {
    int pil, pilArah;

    while (1) {
        clearScreen();
        printTitle("  SEARCH DATA MOBIL  ", 60);
        printf("  Cari berdasarkan:\n\n");
        printf("  1. Nama Mobil  (Sequential Search)\n");
        printf("  2. Tipe Mobil  (Binary Search)\n");
        printf("  0. Kembali\n");
        printBorder(60);
        printf("  Pilihan: ");

        if (!bacaMenu(&pil)) { pauseScreen(); continue; }
        if (pil == 0) return;
        if (pil < 1 || pil > 2) {
            printf("  [!] Pilihan harus 0, 1, atau 2!\n");
            pauseScreen();
            continue;
        }

        clearScreen();
        printTitle("  URUTAN HASIL PENCARIAN  ", 60);
        printf("  1. Ascending  (A-Z)\n");
        printf("  2. Descending (Z-A)\n");
        printf("  0. Kembali\n");
        printBorder(60);
        printf("  Pilihan: ");

        if (!bacaMenu(&pilArah)) { pauseScreen(); continue; }
        if (pilArah == 0) continue;
        if (pilArah < 1 || pilArah > 2) {
            printf("  [!] Pilihan harus 1 atau 2!\n");
            pauseScreen();
            continue;
        }

        bool asc = (pilArah == 1);
        const char* arahNama = asc ? "Ascending" : "Descending";

        char kata_kunci[MAX_STR];
        printf("\n  Masukkan kata kunci: ");
        while (!bacaString(kata_kunci, MAX_STR, "Kata kunci")) {
            printf("  Masukkan kata kunci: ");
        }

        clearScreen();
        char judulBuf[80];
        
        NodeMobil* hasilHead = NULL;

        if (pil == 1) {
            sprintf(judulBuf, "HASIL Sequential Search by Nama [%s]", arahNama);
            printTitle(judulBuf, 86);
            printf("  Keyword: \"%s\" (case insensitive)\n\n", kata_kunci);

            int count = sequentialSearch(kata_kunci, hasilHead);

            if (count == 0) {
                printf("  Data tidak ditemukan untuk: \"%s\"\n", kata_kunci);
            } else {
                bubbleSort(hasilHead, count, 0, asc); // Menggunakan Bubble Sort
                cetakHeaderMobil();
                NodeMobil* curr = hasilHead;
                while(curr) { cetakBarisMobil(curr->data); curr = curr->next; }
                printBorder(86);
                printf("  Ditemukan %d data.\n", count);
            }

        } else {
            sprintf(judulBuf, "HASIL Binary Search by Tipe [%s]", arahNama);
            printTitle(judulBuf, 86);
            printf("  Keyword: \"%s\" (case insensitive)\n\n", kata_kunci);

            int count = binarySearch(kata_kunci, hasilHead);

            if (count == 0) {
                printf("  Data tidak ditemukan untuk: \"%s\"\n", kata_kunci);
                printf("  Catatan: Binary Search membutuhkan kata kunci TEPAT (contoh: \"SUV\")\n");
            } else {
                selectionSort(hasilHead, count, 1, asc); // Menggunakan Selection Sort
                cetakHeaderMobil();
                NodeMobil* curr = hasilHead;
                while(curr) { cetakBarisMobil(curr->data); curr = curr->next; }
                printBorder(86);
                printf("  Ditemukan %d data.\n", count);
            }
        }
        
        freeListMobil(hasilHead);
        pauseScreen();
    }
}

// ============================================================
//  FUNGSI DELETE (HAPUS) DATA MOBIL
// ============================================================
void deleteMobil() {
    char idCari[MAX_STR];
    char konfirmasi[MAX_STR];

    while (1) {
        clearScreen();
        printTitle("  DELETE DATA MOBIL  ", 60);

        if (jumlahMobil == 0) {
            printf("  Tidak ada data untuk dihapus.\n");
            pauseScreen();
            return;
        }

        outputMobil(headMobil, jumlahMobil);

        printf("\n  Masukkan ID yang akan dihapus (0=Kembali): ");
        fgets(idCari, MAX_STR, stdin);
        idCari[strcspn(idCari, "\n")] = '\0';

        if (strcmp(idCari, "0") == 0) return;

        if (strlen(idCari) == 0) {
            printf("  [!] ID tidak boleh kosong!\n");
            pauseScreen();
            continue;
        }

        NodeMobil* curr = headMobil;
        NodeMobil* prev = NULL;
        
        while (curr != NULL) {
            if (strCmpCI(curr->data.id, idCari) == 0) {
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        if (curr == NULL) {
            printf("  [!] ID \"%s\" tidak ditemukan!\n", idCari);
            pauseScreen();
            continue;
        }

        printf("\n  Data yang akan dihapus:\n");
        cetakHeaderMobil();
        cetakBarisMobil(curr->data);
        printBorder(86);

        printf("\n  Yakin ingin menghapus? (y/n): ");
        fgets(konfirmasi, MAX_STR, stdin);
        konfirmasi[strcspn(konfirmasi, "\n")] = '\0';

        if (strlen(konfirmasi) == 0 ||
           (konfirmasi[0] != 'y' && konfirmasi[0] != 'Y' &&
            konfirmasi[0] != 'n' && konfirmasi[0] != 'N')) {
            printf("  [!] Jawaban tidak valid! Ketik y atau n.\n");
            pauseScreen();
            continue;
        }

        if (konfirmasi[0] == 'y' || konfirmasi[0] == 'Y') {
            if (prev == NULL) {
                headMobil = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            jumlahMobil--;
            simpanMobil();
            printf("  [v] Data berhasil dihapus!\n");
        } else {
            printf("  Penghapusan dibatalkan.\n");
        }
        pauseScreen();
    }
}

// ============================================================
//  FUNGSI EDIT DATA MOBIL
// ============================================================
void editMobil() {
    char idCari[MAX_STR];
    char tmpBuf[MAX_STR];

    while (1) {
        clearScreen();
        printTitle("  EDIT DATA MOBIL  ", 60);

        if (jumlahMobil == 0) {
            printf("  Tidak ada data untuk diedit.\n");
            pauseScreen();
            return;
        }

        outputMobil(headMobil, jumlahMobil);

        printf("\n  Masukkan ID yang akan diedit (0=Kembali): ");
        fgets(idCari, MAX_STR, stdin);
        idCari[strcspn(idCari, "\n")] = '\0';

        if (strcmp(idCari, "0") == 0) return;

        if (strlen(idCari) == 0) {
            printf("  [!] ID tidak boleh kosong!\n");
            pauseScreen();
            continue;
        }

        NodeMobil* curr = headMobil;
        while (curr != NULL) {
            if (strCmpCI(curr->data.id, idCari) == 0) {
                break;
            }
            curr = curr->next;
        }

        if (curr == NULL) {
            printf("  [!] ID \"%s\" tidak ditemukan!\n", idCari);
            pauseScreen();
            continue;
        }

        Mobil& m = curr->data;

        printf("\n  Data saat ini:\n");
        cetakHeaderMobil();
        cetakBarisMobil(m);
        printBorder(86);
        printf("\n  (Langsung Enter = tidak diubah)\n\n");

        printf("  Nama Mobil  [%s]: ", m.nama);
        bacaStringOpsional(tmpBuf, MAX_STR);
        if (strlen(tmpBuf) > 0) strcpy(m.nama, tmpBuf);

        printf("  Tipe        [%s]: ", m.tipe);
        bacaStringOpsional(tmpBuf, MAX_STR);
        if (strlen(tmpBuf) > 0) strcpy(m.tipe, tmpBuf);

        printf("  Warna       [%s]: ", m.warna);
        bacaStringOpsional(tmpBuf, MAX_STR);
        if (strlen(tmpBuf) > 0) strcpy(m.warna, tmpBuf);

        printf("  Tahun       [%d]: ", m.tahun);
        bacaStringOpsional(tmpBuf, MAX_STR);
        if (strlen(tmpBuf) > 0) {
            int thn = 0, valid = 1;
            for (int i = 0; tmpBuf[i] != '\0'; i++) {
                if (tmpBuf[i] < '0' || tmpBuf[i] > '9') { valid = 0; break; }
            }
            if (!valid) {
                printf("  [!] Tahun tidak valid, tetap menggunakan nilai lama.\n");
            } else {
                thn = atoi(tmpBuf);
                if (thn < 1900 || thn > 2100)
                    printf("  [!] Tahun harus 1900-2100, tetap menggunakan nilai lama.\n");
                else
                    m.tahun = thn;
            }
        }

        printf("  Harga (Rp)  [%.0f]: ", m.harga);
        bacaStringOpsional(tmpBuf, MAX_STR);
        if (strlen(tmpBuf) > 0) {
            int valid = 1, titikCount = 0;
            for (int i = 0; tmpBuf[i] != '\0'; i++) {
                if (tmpBuf[i] == '.') { titikCount++; if (titikCount > 1) { valid = 0; break; } }
                else if (tmpBuf[i] < '0' || tmpBuf[i] > '9') { valid = 0; break; }
            }
            if (!valid || atof(tmpBuf) <= 0)
                printf("  [!] Harga tidak valid, tetap menggunakan nilai lama.\n");
            else
                m.harga = atof(tmpBuf);
        }

        printf("  Stok        [%d]: ", m.stok);
        bacaStringOpsional(tmpBuf, MAX_STR);
        if (strlen(tmpBuf) > 0) {
            int valid = 1;
            for (int i = 0; tmpBuf[i] != '\0'; i++) {
                if (tmpBuf[i] < '0' || tmpBuf[i] > '9') { valid = 0; break; }
            }
            if (!valid || atoi(tmpBuf) < 0)
                printf("  [!] Stok tidak valid, tetap menggunakan nilai lama.\n");
            else
                m.stok = atoi(tmpBuf);
        }

        simpanMobil();
        printf("\n  [v] Data berhasil diperbarui!\n");
        pauseScreen();
    }
}

// ============================================================
//  MENU UTAMA
// ============================================================
void mainMenu(const char* namaUser) {
    int pil;
    while (1) {
        clearScreen();
        printf("\n");
        printTitle("  SISTEM DEALER MOBIL NUSANTARA  ", 60);
        printf("  Selamat datang, %s\n\n", namaUser);
        printf("  1. Input Data Mobil\n");
        printf("  2. Output Data Mobil\n");
        printf("  3. Sorting Data Mobil\n");
        printf("  4. Search Data Mobil\n");
        printf("  5. Delete Data Mobil\n");
        printf("  6. Edit Data Mobil\n");
        printf("  7. Keluar\n");
        printBorder(50);
        printf("  Masukkan Menu : ");

        if (!bacaMenu(&pil)) { pauseScreen(); continue; }
        if (pil < 1 || pil > 7) {
            printf("  [!] Pilihan harus antara 1-7!\n");
            pauseScreen();
            continue;
        }

        switch (pil) {
            case 1: inputMobil();                 break;
            case 2: outputMobil(headMobil, jumlahMobil); break;
            case 3: menuSorting();                break;
            case 4: menuSearching();              break;
            case 5: deleteMobil();                break;
            case 6: editMobil();                  break;
            case 7:
                clearScreen();
                printf("\n  Terima kasih! Sampai jumpa.\n\n");
                return;
        }
    }
}

// ============================================================
//  FUNGSI LOGIN & BUAT AKUN
// ============================================================
int usernameAda(const char* user) {
    NodeAkun* curr = headAkun;
    while (curr) {
        if (strcmp(curr->data.username, user) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

void buatAkun() {
    clearScreen();
    printTitle("  BUAT AKUN BARU  ", 60);

    Akun a;
    printf("  Nama Lengkap : ");
    while (!bacaString(a.nama_lengkap, MAX_STR, "Nama Lengkap")) {
        printf("  Nama Lengkap : ");
    }

    printf("  Username     : ");
    while (1) {
        if (!bacaString(a.username, MAX_STR, "Username")) {
            printf("  Username     : ");
            continue;
        }
        if (usernameAda(a.username)) {
            printf("  [!] Username sudah dipakai! Coba username lain.\n");
            printf("  Username     : ");
            continue;
        }
        break;
    }

    printf("  Password     : ");
    while (1) {
        if (!bacaString(a.password, MAX_STR, "Password")) {
            printf("  Password     : ");
            continue;
        }
        if ((int)strlen(a.password) < 4) {
            printf("  [!] Password minimal 4 karakter!\n");
            printf("  Password     : ");
            continue;
        }
        break;
    }

    insertLastAkun(headAkun, a);
    jumlahAkun++;
    simpanAkun();
    printf("\n  [v] Akun berhasil dibuat! Silakan login.\n");
    pauseScreen();
}

int login(char* namaUserOut) {
    int percobaan = 0;

    while (percobaan < MAX_LOGIN) {
        clearScreen();
        printTitle("  LOGIN  ", 60);
        printf("\n  Percobaan ke-%d dari %d\n\n", percobaan + 1, MAX_LOGIN);

        char user[MAX_STR], pass[MAX_STR];

        printf("  Username : ");
        if (!bacaString(user, MAX_STR, "Username")) {
            printf("  [!] Username tidak boleh kosong!\n");
            pauseScreen();
            continue;
        }

        printf("  Password : ");
        if (!bacaString(pass, MAX_STR, "Password")) {
            printf("  [!] Password tidak boleh kosong!\n");
            pauseScreen();
            continue;
        }

        NodeAkun* curr = headAkun;
        while (curr) {
            if (strCmpCI(curr->data.username, user) == 0 &&
                strcmp(curr->data.password, pass) == 0) {
                strcpy(namaUserOut, curr->data.nama_lengkap);
                return 1;
            }
            curr = curr->next;
        }

        percobaan++;
        int sisa = MAX_LOGIN - percobaan;

        if (sisa > 0) {
            printf("\n  [!] Username atau password salah!\n");
            printf("      Sisa percobaan: %d\n", sisa);
            pauseScreen();
        } else {
            clearScreen();
            printBorder(50);
            printf("  AKSES DITOLAK!\n");
            printBorder(50);
            printf("\n  Anda telah salah memasukkan login\n");
            printf("  sebanyak %d kali.\n\n", MAX_LOGIN);
            printf("  Program akan otomatis ditutup.\n\n");
            printBorder(50);
            printf("\n  Tekan Enter untuk keluar...");
            getchar();
            exit(0);
        }
    }
    return 0;
}

// ============================================================
//  FUNGSI MAIN
// ============================================================
int main(){
    muatAkun();
    muatMobil();

    if (jumlahAkun == 0) {
        Akun a;
        strcpy(a.username,     "admin");
        strcpy(a.password,     "admin123");
        strcpy(a.nama_lengkap, "Administrator");
        insertLastAkun(headAkun, a);
        jumlahAkun = 1;
        simpanAkun();
    }

    int pil;
    char namaUser[MAX_STR];

    while (1) {
        clearScreen();
        printf("\n");
        printTitle("  SISTEM DEALER MOBIL NUSANTARA  ", 60);
        printf("  1. Login\n");
        printf("  2. Buat Akun Baru\n");
        printf("  3. Keluar\n");
        printBorder(60);
        printf("  Pilihan: ");

        if (!bacaMenu(&pil)) {
            pauseScreen();
            continue;
        }
        if (pil < 1 || pil > 3) {
            printf("  [!] Pilihan harus 1, 2, atau 3!\n");
            pauseScreen();
            continue;
        }

        if (pil == 1) {
            int hasil = login(namaUser);
            if (hasil) {
                printf("\n  [v] Login berhasil! Selamat datang, %s\n", namaUser);
                printf("\n  Tekan Enter untuk masuk ke menu utama...");
                getchar();
                mainMenu(namaUser);
            }
        } else if (pil == 2) {
            buatAkun();
        } else {
            printf("\n  Sampai jumpa!\n\n");
            break;
        }
    }

    // Bebaskan memori sebelum program berakhir (Good Practice)
    freeListMobil(headMobil);
    while (headAkun) {
        NodeAkun* temp = headAkun;
        headAkun = headAkun->next;
        delete temp;
    }

    return 0;
}