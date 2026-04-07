#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

struct Alamat
{
    string jalan, kota, provinsi;
};

struct Pelanggan
{
    string nama, noTelp;
    Alamat alamat;
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

void inputInt(const string &prompt, int &hasil)
{
    while (true)
    {
        cout << prompt;
        cin >> hasil;
        if (cin.fail())
        {
            cout << "Input harus berupa angka. Coba lagi.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
}

void inputDouble(const string &prompt, double &hasil)
{
    while (true)
    {
        cout << prompt;
        cin >> hasil;
        if (cin.fail())
        {
            cout << "Input harus berupa angka. Coba lagi.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
}

void garis()
{
    for (int i = 0; i < 80; i++)
        cout << "=";
    cout << endl;
}

void garis(int n, char c)
{
    for (int i = 0; i < n; i++)
        cout << c;
    cout << "\n";
}

void header(const string &judul)
{
    garis();
    cout << setw(40 + judul.size() / 2) << judul << "\n";
    garis();
}

void jeda()
{
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void hitungTotal(Pemesanan *p)
{
    p->totalHarga = p->paket.harga * p->jumlahOrang;
}

void updateStatus(Pemesanan *p, const string &statusBaru)
{
    p->status = statusBaru;
}

void inputDataPelanggan(Pelanggan &pel)
{
    cout << "Nama        : ";
    getline(cin, pel.nama);
    cout << "No. Telepon : ";
    getline(cin, pel.noTelp);
    cout << "Jalan       : ";
    getline(cin, pel.alamat.jalan);
    cout << "Kota        : ";
    getline(cin, pel.alamat.kota);
    cout << "Provinsi    : ";
    getline(cin, pel.alamat.provinsi);
}

int cariPaketRekursif(int idCari, int index)
{
    if (index >= JML_PAKET)
        return -1;
    if (pakets[index].id == idCari)
        return index;
    return cariPaketRekursif(idCari, index + 1);
}

int cariOrder(int no)
{
    for (int i = 0; i < jmlOrder; i++)
        if (orders[i].noPesan == no)
            return i;
    return -1;
}

bool bolehAkses(int idx, User *u)
{
    return u->role == "admin" || orders[idx].nimPemilik == u->nim;
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

    int p, idx;
    do
    {
        inputInt("Pilih No. Paket (ID): ", p);
        idx = cariPaketRekursif(p, 0);
        if (idx == -1)
            cout << "[!] ID Paket tidak valid!\n";
    } while (idx == -1);

    return pakets[idx];
}

void tambah(User *u)
{
    header(" TAMBAH PEMESANAN ");
    if (jmlOrder >= MAX_PESAN)
    {
        cout << "Data penuh!\n";
        jeda();
        return;
    }

    Pemesanan p;
    p.noPesan = (jmlOrder == 0) ? 1 : orders[jmlOrder - 1].noPesan + 1;
    p.nimPemilik = u->nim;
    p.status = "Pending";

    cout << "=== DATA PELANGGAN ===\n";
    inputDataPelanggan(p.pelanggan);

    cout << "\n=== PILIH PAKET ===\n";
    p.paket = pilihPaket();

    cout << "\n=== DETAIL PEMESANAN ===\n";
    cout << "Tanggal Pergi : ";
    getline(cin, p.tanggalPergi);

    inputInt("Jumlah Orang  : ", p.jumlahOrang);

    hitungTotal(&p);

    orders[jmlOrder++] = p;
    cout << "[+] Pemesanan berhasil ditambahkan! Total: Rp "
         << (long long)p.totalHarga << "\n";
    jeda();
}

void ubah(User *u)
{
    header(" UBAH PEMESANAN ");
    int no;
    inputInt("No. Pemesanan: ", no);
    int i = cariOrder(no);

    if (i == -1)
    {
        cout << "Tidak ditemukan!\n";
        jeda();
        return;
    }
    if (!bolehAkses(i, u))
    {
        cout << "Akses ditolak!\n";
        jeda();
        return;
    }

    Pemesanan *p = &orders[i];

    cout << "\n1. Paket Wisata\n2. Tanggal & Jumlah Orang\n";
    if (u->role == "admin")
        cout << "3. Status [ADMIN]\n";
    cout << "0. Batal\n";

    int pil;
    inputInt("Pilihan: ", pil);

    switch (pil)
    {
    case 1:
        p->paket = pilihPaket();
        hitungTotal(p);
        break;
    case 2:
        cout << "Tanggal baru: ";
        getline(cin, p->tanggalPergi);
        inputInt("Jumlah orang: ", p->jumlahOrang);
        hitungTotal(p);
        break;
    case 3:
        if (u->role == "admin")
        {
            string statusBaru;
            cout << "Status baru (Pending/Confirmed/Cancelled): ";
            getline(cin, statusBaru);
            updateStatus(p, statusBaru);
        }
        break;
    default:
        return;
    }
    cout << "Data diperbarui!\n";
    jeda();
}

void hapus(User *u)
{
    header(" HAPUS PEMESANAN ");
    string filter = (u->role == "admin") ? "" : u->nim;
    tampilTabel(filter);

    int no;
    inputInt("No. Pemesanan yang akan dihapus: ", no);
    int i = cariOrder(no);

    if (i == -1)
    {
        cout << "Tidak ditemukan!\n";
        jeda();
        return;
    }
    if (!bolehAkses(i, u))
    {
        cout << "Akses ditolak!\n";
        jeda();
        return;
    }

    char k;
    cout << "Yakin hapus? (y/n): ";
    cin >> k;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (k == 'y' || k == 'Y')
    {
        for (int j = i; j < jmlOrder - 1; j++)
            orders[j] = orders[j + 1];
        jmlOrder--;
        cout << "Data berhasil dihapus!\n";
    }
    jeda();
}

void daftarUser()
{
    header(" DAFTAR USER [ADMIN] ");
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
        header(" MENU UTAMA ");
        cout << "Login: " << aktif.nama << " [" << aktif.role << "]\n";
        cout << "1. Tambah Pesanan\n2. Lihat Pesanan\n3. Ubah Pesanan\n4. Hapus Pesanan\n";
        if (aktif.role == "admin")
            cout << "5. Daftar User\n";
        cout << "0. Logout\n";

        inputInt("Pilihan: ", pil);

        switch (pil)
        {
        case 1:
            tambah(&aktif);
            break;
        case 2:
            tampilTabel(aktif.role == "admin" ? "" : aktif.nim);
            jeda();
            break;
        case 3:
            ubah(&aktif);
            break;
        case 4:
            hapus(&aktif);
            break;
        case 5:
            if (aktif.role == "admin")
                daftarUser();
            break;
        }
    } while (pil != 0);
}

int main()
{
    users[jmlUser++] = {"arya", "022", "admin"};

    int pil;
    do
    {
        header(" AGEN TRAVEL NUSANTARA ");
        cout << "1. Login\n2. Registrasi\n0. Keluar\n";
        inputInt("Pilihan: ", pil);

        if (pil == 1)
        {
            string n, p;
            cout << "Nama: ";
            cin >> n;
            cout << "NIM : ";
            cin >> p;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool loginOk = false;
            for (int i = 0; i < jmlUser; i++)
            {
                if (users[i].nama == n && users[i].nim == p)
                {
                    aktif = users[i];
                    loginOk = true;
                    menuUtama();
                    break;
                }
            }
            if (!loginOk)
            {
                cout << "[-] Login Gagal! Nama/NIM tidak ditemukan.\n";
                jeda();
            }
        }
        else if (pil == 2)
        {
            User u;
            cout << "Nama: ";
            cin >> u.nama;
            cout << "NIM : ";
            cin >> u.nim;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            u.role = "user";
            users[jmlUser++] = u;
            cout << " Registrasi Berhasil! Silakan login.\n";
            jeda();
        }
    } while (pil != 0);

    cout << "Terima kasih telah menggunakan Agen Travel Nusantara!\n";
    return 0;
}