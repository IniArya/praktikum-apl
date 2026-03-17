#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ===================== STRUCT =====================

struct Alamat
{
    string jalan, kota, provinsi;
};

struct Pelanggan
{
    string nama, noTelp;
    Alamat alamat; // nested struct
};

struct Paket
{
    int id, durasi;
    double harga;
    string destinasi, fasilitas;
};

struct Pemesanan
{
    int noPesan, jumlahOrang;
    double totalHarga;
    string nimPemilik, tanggalPergi, status;
    Pelanggan pelanggan;
    Paket paket;
};

struct User
{
    string nama, nim, role;
};

const int MAX_USER = 50, MAX_PESAN = 100, JML_PAKET = 5;

User users[MAX_USER];
int jmlUser = 0;
Pemesanan orders[MAX_PESAN];
int jmlOrder = 0;
User aktif;

Paket pakets[JML_PAKET] = {
    {1, 3, 1500000, "Samarinda", "Hotel 3*, Transport, Wisata Kota, Makan 2x/hari"},
    {2, 4, 2500000, "Balikpapan", "Hotel 4*, Transport, Pantai Manggar, Makan 3x/hari"},
    {3, 3, 2000000, "Bontang", "Hotel 3*, Transport, Pulau Beras Basah, Makan 2x/hari"},
    {4, 5, 3000000, "Sangatta", "Hotel 3*, Transport, Taman Nasional Kutai, Makan 3x/hari"},
    {5, 6, 4500000, "Berau", "Hotel 4*, Transport, Kepulauan Derawan, Makan 3x/hari"}};

void garis(int n = 80, char c = '=')
{
    for (int i = 0; i < n; i++)
        cout << c;
    cout << "\n";
}

void header(const string &judul)
{
    garis();
    cout << setw(42 + judul.size() / 2) << judul << "\n";
    garis();
}

void jeda()
{
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore();
    cin.get();
}

int cariOrder(int no)
{
    for (int i = 0; i < jmlOrder; i++)
        if (orders[i].noPesan == no)
            return i;
    return -1;
}

bool bolehAkses(int idx)
{
    return aktif.role == "admin" || orders[idx].nimPemilik == aktif.nim;
}

void tabelHeader()
{
    garis(105, '-');
    cout << left << setw(5) << "No" << setw(16) << "Pelanggan" << setw(13) << "No.Telp"
         << setw(13) << "Destinasi" << setw(8) << "Durasi" << setw(12) << "Tgl Pergi"
         << setw(5) << "Org" << setw(16) << "Total Harga" << "Status\n";
    garis(105, '-');
}

void tabelBaris(const Pemesanan &p)
{
    cout << left << setw(5) << p.noPesan << setw(16) << p.pelanggan.nama
         << setw(13) << p.pelanggan.noTelp << setw(13) << p.paket.destinasi
         << setw(8) << (to_string(p.paket.durasi) + " hr")
         << setw(12) << p.tanggalPergi << setw(5) << p.jumlahOrang
         << setw(16) << ("Rp " + to_string((long long)p.totalHarga)) << p.status << "\n";
}

void tampilTabel(const string &nimFilter)
{
    int n = 0;
    tabelHeader();
    for (int i = 0; i < jmlOrder; i++)
        if (nimFilter.empty() || orders[i].nimPemilik == nimFilter)
        {
            tabelBaris(orders[i]);
            n++;
        }
    garis(105, '-');
    cout << "Total: " << n << " pemesanan\n";
}

Paket pilihPaket()
{
    garis(90, '-');
    cout << left << setw(4) << "No" << setw(13) << "Destinasi" << setw(9) << "Durasi"
         << setw(16) << "Harga/Orang" << "Fasilitas\n";
    garis(90, '-');
    for (int i = 0; i < JML_PAKET; i++)
        cout << left << setw(4) << pakets[i].id << setw(13) << pakets[i].destinasi
             << setw(9) << (to_string(pakets[i].durasi) + " hr")
             << setw(16) << ("Rp " + to_string((long long)pakets[i].harga))
             << pakets[i].fasilitas << "\n";
    garis(90, '-');
    int p = 0;
    while (p < 1 || p > JML_PAKET)
    {
        cout << "Pilih No. Paket (1-" << JML_PAKET << "): ";
        cin >> p;
        if (p < 1 || p > JML_PAKET)
            cout << "Pilihan tidak valid!\n";
    }
    return pakets[p - 1];
}

void registrasi()
{
    header("  REGISTRASI  ");
    if (jmlUser >= MAX_USER)
    {
        cout << "Kapasitas penuh!\n";
        jeda();
        return;
    }

    User u;
    cout << "Nama : ";
    cin.ignore();
    getline(cin, u.nama);
    cout << "NIM  : ";
    getline(cin, u.nim);

    for (int i = 0; i < jmlUser; i++)
        if (users[i].nim == u.nim)
        {
            cout << "[!] NIM sudah terdaftar!\n";
            jeda();
            return;
        }

    u.role = "user";
    users[jmlUser++] = u;
    cout << "[+] Registrasi berhasil!\n";
    jeda();
}

bool login()
{
    header("  LOGIN  ");
    for (int coba = 1; coba <= 3; coba++)
    {
        string nama, nim;
        cout << "\nPercobaan " << coba << "/3\n";
        cout << "Username (Nama) : ";
        cin.ignore();
        getline(cin, nama);
        cout << "Password (NIM)  : ";
        getline(cin, nim);

        for (int i = 0; i < jmlUser; i++)
            if (users[i].nama == nama && users[i].nim == nim)
            {
                aktif = users[i];
                cout << "[+] Selamat datang, " << aktif.nama << "!\n";
                jeda();
                return true;
            }

        int sisa = 3 - coba;
        if (sisa > 0)
            cout << "[-] Salah! Sisa percobaan: " << sisa << "\n";
        else
        {
            cout << "[-] Gagal 3x. Kembali ke menu awal.\n";
            jeda();
        }
    }
    return false;
}

void tambah()
{
    header("  TAMBAH PEMESANAN  ");
    if (jmlOrder >= MAX_PESAN)
    {
        cout << "Data penuh!\n";
        jeda();
        return;
    }

    Pemesanan p;
    p.noPesan = jmlOrder + 1;
    p.nimPemilik = aktif.nim;
    p.status = "Pending";

    cout << "=== DATA PELANGGAN ===\n";
    cin.ignore();
    cout << "Nama        : ";
    getline(cin, p.pelanggan.nama);
    cout << "No. Telepon : ";
    getline(cin, p.pelanggan.noTelp);
    cout << "Jalan       : ";
    getline(cin, p.pelanggan.alamat.jalan);
    cout << "Kota        : ";
    getline(cin, p.pelanggan.alamat.kota);
    cout << "Provinsi    : ";
    getline(cin, p.pelanggan.alamat.provinsi);

    cout << "\n=== PILIH PAKET ===\n";
    p.paket = pilihPaket();

    cout << "\n=== DETAIL PEMESANAN ===\n";
    cin.ignore();
    cout << "Tanggal Pergi : ";
    getline(cin, p.tanggalPergi);
    cout << "Jumlah Orang  : ";
    cin >> p.jumlahOrang;
    p.totalHarga = p.paket.harga * p.jumlahOrang;

    orders[jmlOrder++] = p;
    cout << "[+] Pemesanan ditambahkan! Total: Rp "
         << fixed << setprecision(0) << p.totalHarga << "\n";
    jeda();
}

void lihat()
{
    string filter = (aktif.role == "admin") ? "" : aktif.nim;
    header(aktif.role == "admin" ? "  SEMUA PEMESANAN [ADMIN]  " : "  PEMESANAN SAYA  ");
    bool ada = false;
    for (int i = 0; i < jmlOrder; i++)
        if (filter.empty() || orders[i].nimPemilik == filter)
        {
            ada = true;
            break;
        }
    if (!ada)
    {
        cout << "Belum ada data pemesanan.\n";
        jeda();
        return;
    }
    tampilTabel(filter);
    jeda();
}

void detail()
{
    header("  DETAIL PEMESANAN  ");
    int no;
    cout << "No. Pemesanan: ";
    cin >> no;
    int i = cariOrder(no);
    if (i == -1)
    {
        cout << "[-] Tidak ditemukan!\n";
        jeda();
        return;
    }
    if (!bolehAkses(i))
    {
        cout << "[-] Akses ditolak!\n";
        jeda();
        return;
    }

    Pemesanan &p = orders[i];
    garis(60, '-');
    cout << "No. Pemesanan  : " << p.noPesan
         << "\n\n--- PELANGGAN ---\n"
         << "Nama           : " << p.pelanggan.nama
         << "\nNo. Telepon    : " << p.pelanggan.noTelp
         << "\nAlamat         : " << p.pelanggan.alamat.jalan << ", "
         << p.pelanggan.alamat.kota << ", "
         << p.pelanggan.alamat.provinsi
         << "\n\n--- PAKET ---\n"
         << "Destinasi      : " << p.paket.destinasi
         << "\nDurasi         : " << p.paket.durasi << " hari"
         << "\nHarga/Orang    : Rp " << fixed << setprecision(0) << p.paket.harga
         << "\nFasilitas      : " << p.paket.fasilitas
         << "\n\n--- PEMESANAN ---\n"
         << "Tanggal Pergi  : " << p.tanggalPergi
         << "\nJumlah Orang   : " << p.jumlahOrang
         << "\nTotal Harga    : Rp " << p.totalHarga
         << "\nStatus         : " << p.status << "\n";
    garis(60, '-');
    jeda();
}

void ubah()
{
    header("  UBAH PEMESANAN  ");
    int no;
    cout << "No. Pemesanan: ";
    cin >> no;
    int i = cariOrder(no);
    if (i == -1)
    {
        cout << "[-] Tidak ditemukan!\n";
        jeda();
        return;
    }
    if (!bolehAkses(i))
    {
        cout << "[-] Akses ditolak!\n";
        jeda();
        return;
    }

    Pemesanan &p = orders[i];
    tabelHeader();
    tabelBaris(p);
    garis(105, '-');

    cout << "\n1. Data Pelanggan\n2. Paket Wisata\n3. Tanggal & Jumlah Orang\n";
    if (aktif.role == "admin")
        cout << "4. Status [ADMIN]\n";
    cout << "0. Batal\nPilihan: ";
    int pil;
    cin >> pil;
    cin.ignore();

    switch (pil)
    {
    case 1:
        cout << "Nama baru      : ";
        getline(cin, p.pelanggan.nama);
        cout << "Telepon baru   : ";
        getline(cin, p.pelanggan.noTelp);
        cout << "Jalan baru     : ";
        getline(cin, p.pelanggan.alamat.jalan);
        cout << "Kota baru      : ";
        getline(cin, p.pelanggan.alamat.kota);
        cout << "Provinsi baru  : ";
        getline(cin, p.pelanggan.alamat.provinsi);
        break;
    case 2:
        p.paket = pilihPaket();
        p.totalHarga = p.paket.harga * p.jumlahOrang;
        break;
    case 3:
        cout << "Tanggal baru   : ";
        getline(cin, p.tanggalPergi);
        cout << "Jumlah orang   : ";
        cin >> p.jumlahOrang;
        p.totalHarga = p.paket.harga * p.jumlahOrang;
        break;
    case 4:
        if (aktif.role != "admin")
        {
            cout << "[-] Akses ditolak! Hanya admin yang dapat mengubah status.\n";
            break;
        }
        cout << "Status baru (Pending/Confirmed/Cancelled): ";
        getline(cin, p.status);
        break;
    case 0:
        jeda();
        return;
    default:
        cout << "Pilihan tidak valid.\n";
    }
    cout << "[+] Data diperbarui!\n";
    jeda();
}

void hapus()
{
    header("  HAPUS PEMESANAN  ");
    string filter = (aktif.role == "admin") ? "" : aktif.nim;

    int ada = 0;
    tabelHeader();
    for (int i = 0; i < jmlOrder; i++)
        if (filter.empty() || orders[i].nimPemilik == filter)
        {
            tabelBaris(orders[i]);
            ada++;
        }
    garis(105, '-');
    if (ada == 0)
    {
        cout << "Tidak ada data yang bisa dihapus.\n";
        jeda();
        return;
    }

    int no;
    cout << "No. Pemesanan yang dihapus: ";
    cin >> no;
    int i = cariOrder(no);
    if (i == -1)
    {
        cout << "[-] Tidak ditemukan!\n";
        jeda();
        return;
    }
    if (!bolehAkses(i))
    {
        cout << "[-] Akses ditolak!\n";
        jeda();
        return;
    }

    cout << "Yakin hapus? (y/n): ";
    char k;
    cin >> k;
    if (k == 'y' || k == 'Y')
    {
        for (int j = i; j < jmlOrder - 1; j++)
            orders[j] = orders[j + 1];
        jmlOrder--;
        cout << "[+] Dihapus!\n";
    }
    else
        cout << "Dibatalkan.\n";
    jeda();
}

void daftarUser()
{
    header("  DAFTAR USER [ADMIN]  ");
    garis(50, '-');
    cout << left << setw(4) << "No" << setw(20) << "Nama" << setw(15) << "NIM" << "Role\n";
    garis(50, '-');
    for (int i = 0; i < jmlUser; i++)
        cout << left << setw(4) << (i + 1) << setw(20) << users[i].nama
             << setw(15) << users[i].nim << users[i].role << "\n";
    garis(50, '-');
    jeda();
}

void menuUtama()
{
    int pil;
    do
    {
        header("  MANAJEMEN AGEN TRAVEL NUSANTARA  ");
        cout << "Login: " << aktif.nama << " [" << aktif.role << "]\n";
        garis(40, '-');
        cout << "  1. Tambah Pemesanan\n"
             << "  2. " << (aktif.role == "admin" ? "Lihat Semua Pemesanan" : "Lihat Pemesanan Saya") << "\n"
             << "  3. Detail Pemesanan\n"
             << "  4. Ubah Pemesanan\n"
             << "  5. Hapus Pemesanan\n";
        if (aktif.role == "admin")
            cout << "  6. Daftar User\n";
        cout << "  0. Keluar\n";
        garis(40, '-');
        cout << "Pilihan: ";
        cin >> pil;

        switch (pil)
        {
        case 1:
            tambah();
            break;
        case 2:
            lihat();
            break;
        case 3:
            detail();
            break;
        case 4:
            ubah();
            break;
        case 5:
            hapus();
            break;
        case 6:
            if (aktif.role == "admin")
                daftarUser();
            else
            {
                cout << "[-] Akses ditolak!\n";
                jeda();
            }
            break;
        case 0:
            cout << "Sampai jumpa, " << aktif.nama << "!\n";
            break;
        default:
            cout << "[-] Pilihan tidak valid!\n";
            jeda();
        }
    } while (pil != 0);
}

int main()
{
    users[0] = {"arya", "022", "admin"};
    jmlUser = 1;

    int pil;
    do
    {
        header("  SELAMAT DATANG - AGEN TRAVEL NUSANTARA  ");
        cout << "  1. Login\n  2. Registrasi\n  0. Keluar\n";
        garis(40, '-');
        cout << "Pilihan: ";
        cin >> pil;

        if (pil == 1)
        {
            if (login())
                menuUtama();
        }
        else if (pil == 2)
            registrasi();
        else if (pil == 0)
            cout << "Sampai jumpa!\n";
        else
        {
            cout << "[-] Pilihan tidak valid!\n";
            jeda();
        }
    } while (pil != 0);

    return 0;
}