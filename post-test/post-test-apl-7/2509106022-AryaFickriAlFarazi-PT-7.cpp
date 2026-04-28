#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <stdexcept>
using namespace std;

class KesalahanTravel : public runtime_error
{
public:
    explicit KesalahanTravel(const string &msg)
        : runtime_error("[Kesalahan Travel] " + msg) {}
};

class KesalahanInput : public KesalahanTravel
{
public:
    explicit KesalahanInput(const string &msg)
        : KesalahanTravel("[Input] " + msg) {}
};

class DataTidakDitemukan : public KesalahanTravel
{
public:
    explicit DataTidakDitemukan(const string &msg)
        : KesalahanTravel("[Tidak Ditemukan] " + msg) {}
};

class KapasitasPenuh : public KesalahanTravel
{
public:
    explicit KapasitasPenuh(const string &msg)
        : KesalahanTravel("[Kapasitas] " + msg) {}
};

class AksesDitolak : public KesalahanTravel
{
public:
    explicit AksesDitolak(const string &msg)
        : KesalahanTravel("[Akses Ditolak] " + msg) {}
};

class StatusTidakValid : public KesalahanTravel
{
public:
    explicit StatusTidakValid(const string &msg)
        : KesalahanTravel("[Status Tidak Valid] " + msg) {}
};

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
        if (!(cin >> hasil))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            try
            {
                throw KesalahanInput("Input harus berupa bilangan bulat.");
            }
            catch (const KesalahanInput &e)
            {
                cout << e.what() << " Coba lagi.\n";
            }
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
    }
}

void inputDouble(const string &prompt, double &hasil)
{
    while (true)
    {
        cout << prompt;
        if (!(cin >> hasil))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            try
            {
                throw KesalahanInput("Input harus berupa angka desimal.");
            }
            catch (const KesalahanInput &e)
            {
                cout << e.what() << " Coba lagi.\n";
            }
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
    }
}

void inputString(const string &prompt, string &hasil, const string &namaField)
{
    cout << prompt;
    getline(cin, hasil);
    if (hasil.empty())
    {
        throw KesalahanInput(namaField + " tidak boleh kosong.");
    }
}

void inputJumlahOrang(int &hasil)
{
    inputInt("Jumlah Orang  : ", hasil);
    if (hasil <= 0)
    {
        throw KesalahanInput("Jumlah orang harus lebih dari 0.");
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
    cout << "\nTekan Enter untuk melanjutkan";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void hitungTotal(Pemesanan *p)
{
    p->totalHarga = p->paket.harga * p->jumlahOrang;
}

void updateStatus(Pemesanan *p, const string &statusBaru)
{
    if (statusBaru != "Pending" &&
        statusBaru != "Confirmed" &&
        statusBaru != "Cancelled")
    {
        throw StatusTidakValid(
            "Status '" + statusBaru +
            "' tidak valid. Gunakan: Pending / Confirmed / Cancelled.");
    }
    p->status = statusBaru;
}

void inputDataPelanggan(Pelanggan &pel)
{
    inputString("Nama        : ", pel.nama, "Nama pelanggan");
    inputString("No. Telepon : ", pel.noTelp, "No. telepon");
    inputString("Jalan       : ", pel.alamat.jalan, "Jalan");
    inputString("Kota        : ", pel.alamat.kota, "Kota");
    inputString("Provinsi    : ", pel.alamat.provinsi, "Provinsi");
}

int cariPaketRekursif(int idCari, int index)
{
    if (index >= JML_PAKET)
        throw DataTidakDitemukan("Paket dengan ID " + to_string(idCari) + " tidak ditemukan.");
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

void periksaAkses(int idx, User *u)
{
    if (u->role != "admin" && orders[idx].nimPemilik != u->nim)
    {
        throw AksesDitolak(
            "Anda tidak memiliki izin untuk mengakses pemesanan ini.");
    }
}

void tabelHeader()
{
    garis(105, '-');
    cout << left << setw(5) << "No"
         << setw(16) << "Pelanggan"
         << setw(13) << "No.Telp"
         << setw(13) << "Destinasi"
         << setw(8) << "Durasi"
         << setw(12) << "Tgl Pergi"
         << setw(5) << "Org"
         << setw(16) << "Total Harga"
         << "Status\n";
    garis(105, '-');
}

void tabelBaris(const Pemesanan &p)
{
    cout << left << setw(5) << p.noPesan
         << setw(16) << p.pelanggan.nama
         << setw(13) << p.pelanggan.noTelp
         << setw(13) << p.paket.destinasi
         << setw(8) << (to_string(p.paket.durasi) + " hr")
         << setw(12) << p.tanggalPergi
         << setw(5) << p.jumlahOrang
         << setw(16) << ("Rp " + to_string((long long)p.totalHarga))
         << p.status << "\n";
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

void sortNamaDescending(Pemesanan arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        Pemesanan kunci = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].pelanggan.nama < kunci.pelanggan.nama)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = kunci;
    }
}
void sortHargaAscending(Pemesanan arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int idxMin = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j].totalHarga < arr[idxMin].totalHarga)
                idxMin = j;
        if (idxMin != i)
        {
            Pemesanan t = arr[i];
            arr[i] = arr[idxMin];
            arr[idxMin] = t;
        }
    }
}
void sortJumlahOrangAscending(Pemesanan arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        bool sw = false;
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j].jumlahOrang > arr[j + 1].jumlahOrang)
            {
                Pemesanan t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
                sw = true;
            }
        if (!sw)
            break;
    }
}

void tampilHasilSorting(Pemesanan temp[], int n, const string &judulSort)
{
    header(judulSort);
    if (n == 0)
    {
        cout << "Tidak ada data pemesanan.\n";
        jeda();
        return;
    }
    tabelHeader();
    for (int i = 0; i < n; i++)
        tabelBaris(temp[i]);
    garis(105, '-');
    cout << "Total: " << n << " pemesanan\n";
    jeda();
}

void menuSorting(User *u)
{
    int pil;
    do
    {
        header(" MENU SORTING PEMESANAN ");
        cout << "1. Urutkan Nama Pelanggan (Z -> A) [Insertion Sort - Descending]\n";
        cout << "2. Urutkan Total Harga   (Termurah -> Termahal) [Selection Sort - Ascending]\n";
        cout << "3. Urutkan Jumlah Orang  (Sedikit -> Banyak) [Bubble Sort - Ascending]\n";
        cout << "0. Kembali\n";
        inputInt("Pilihan: ", pil);

        Pemesanan temp[MAX_PESAN];
        int n = 0;
        string nimFilter = (u->role == "admin") ? "" : u->nim;
        for (int i = 0; i < jmlOrder; i++)
            if (nimFilter.empty() || orders[i].nimPemilik == nimFilter)
                temp[n++] = orders[i];

        switch (pil)
        {
        case 1:
            sortNamaDescending(temp, n);
            tampilHasilSorting(temp, n, " SORTING NAMA PELANGGAN (Z -> A) ");
            break;
        case 2:
            sortHargaAscending(temp, n);
            tampilHasilSorting(temp, n, " SORTING TOTAL HARGA (Ascending) ");
            break;
        case 3:
            sortJumlahOrangAscending(temp, n);
            tampilHasilSorting(temp, n, " SORTING JUMLAH ORANG (Ascending) ");
            break;
        }
    } while (pil != 0);
}

Paket pilihPaket()
{
    garis(90, '-');
    cout << left << setw(4) << "No" << setw(13) << "Destinasi"
         << setw(9) << "Durasi" << setw(16) << "Harga/Orang" << "Fasilitas\n";
    garis(90, '-');
    for (int i = 0; i < JML_PAKET; i++)
        cout << left << setw(4) << pakets[i].id
             << setw(13) << pakets[i].destinasi
             << setw(9) << (to_string(pakets[i].durasi) + " hr")
             << setw(16) << ("Rp " + to_string((long long)pakets[i].harga))
             << pakets[i].fasilitas << "\n";
    garis(90, '-');

    int p;
    while (true)
    {
        inputInt("Pilih No. Paket (ID): ", p);
        try
        {
            int idx = cariPaketRekursif(p, 0);
            return pakets[idx];
        }
        catch (const DataTidakDitemukan &e)
        {
            cout << e.what() << " Coba lagi.\n";
        }
    }
}

void tambah(User *u)
{
    header(" TAMBAH PEMESANAN ");

    if (jmlOrder >= MAX_PESAN)
    {
        throw KapasitasPenuh("Data pemesanan sudah mencapai batas maksimum (" +
                             to_string(MAX_PESAN) + ").");
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
    inputString("Tanggal Pergi (YYYY-MM-DD): ", p.tanggalPergi, "Tanggal pergi");

    inputJumlahOrang(p.jumlahOrang);

    hitungTotal(&p);
    orders[jmlOrder++] = p;
    cout << "Pemesanan berhasil! Total: Rp " << (long long)p.totalHarga << "\n";
    jeda();
}

void ubah(User *u)
{
    header(" UBAH PEMESANAN ");

    int no;
    inputInt("No. Pemesanan: ", no);

    int i = cariOrder(no);
    if (i == -1)
        throw DataTidakDitemukan("Pemesanan No. " + to_string(no) + " tidak ditemukan.");

    periksaAkses(i, u);

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
        inputString("Tanggal baru (YYYY-MM-DD): ", p->tanggalPergi, "Tanggal pergi");
        inputJumlahOrang(p->jumlahOrang);
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
    cout << "Data berhasil diperbarui.\n";
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
        throw DataTidakDitemukan("Pemesanan No. " + to_string(no) + " tidak ditemukan.");

    periksaAkses(i, u);

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
    cout << left << setw(4) << "No" << setw(20) << "Nama"
         << setw(15) << "NIM" << "Role\n";
    garis(50, '-');
    for (int i = 0; i < jmlUser; i++)
        cout << left << setw(4) << (i + 1)
             << setw(20) << users[i].nama
             << setw(15) << users[i].nim
             << users[i].role << "\n";
    garis(50, '-');
    jeda();
}

int binarySearchNoPesan(Pemesanan *arr, int n, int *target)
{
    int kiri = 0, kanan = n - 1;
    while (kiri <= kanan)
    {
        int tengah = kiri + (kanan - kiri) / 2;
        if (arr[tengah].noPesan == *target)
            return tengah;
        else if (arr[tengah].noPesan < *target)
            kiri = tengah + 1;
        else
            kanan = tengah - 1;
    }
    return -1;
}

int linearSearchNama(Pemesanan *arr, int n, string *keyword, int mulaiDari)
{
    string kwLower = *keyword;
    transform(kwLower.begin(), kwLower.end(), kwLower.begin(), ::tolower);
    for (int i = mulaiDari; i < n; i++)
    {
        string namaLower = arr[i].pelanggan.nama;
        transform(namaLower.begin(), namaLower.end(), namaLower.begin(), ::tolower);
        if (namaLower.find(kwLower) != string::npos)
            return i;
    }
    return -1;
}

void menuSearching(User *u)
{
    int pil;
    do
    {
        header(" MENU SEARCHING PEMESANAN ");
        cout << "1. Cari Pesanan by No. Pemesanan [Binary Search]\n";
        cout << "2. Cari Pesanan by Nama Pelanggan [Linear Search]\n";
        cout << "0. Kembali\n";
        inputInt("Pilihan: ", pil);

        Pemesanan temp[MAX_PESAN];
        int n = 0;
        string nimFilter = (u->role == "admin") ? "" : u->nim;
        for (int i = 0; i < jmlOrder; i++)
            if (nimFilter.empty() || orders[i].nimPemilik == nimFilter)
                temp[n++] = orders[i];

        if (pil == 1)
        {
            header(" CARI PESANAN - BINARY SEARCH (No. Pemesanan) ");

            for (int i = 1; i < n; i++)
            {
                Pemesanan kunci = temp[i];
                int j = i - 1;
                while (j >= 0 && temp[j].noPesan > kunci.noPesan)
                {
                    temp[j + 1] = temp[j];
                    j--;
                }
                temp[j + 1] = kunci;
            }

            int noTarget;
            inputInt("Masukkan No. Pemesanan yang dicari: ", noTarget);

            if (noTarget < 1)
                throw KesalahanInput("No. Pemesanan harus berupa angka positif.");

            int hasil = binarySearchNoPesan(temp, n, &noTarget);

            cout << "\n";
            garis(80, '-');
            cout << "[ Binary Search ] Mencari No. Pemesanan: " << noTarget << "\n";
            garis(80, '-');

            if (hasil == -1)
                throw DataTidakDitemukan("Pemesanan No. " + to_string(noTarget) +
                                         " tidak ditemukan.");

            cout << "Pemesanan ditemukan!\n\n";
            tabelHeader();
            tabelBaris(temp[hasil]);
            garis(105, '-');
            cout << "\n--- Detail Lengkap ---\n";
            cout << "Nama Pelanggan : " << temp[hasil].pelanggan.nama << "\n";
            cout << "No. Telepon    : " << temp[hasil].pelanggan.noTelp << "\n";
            cout << "Alamat         : " << temp[hasil].pelanggan.alamat.jalan
                 << ", " << temp[hasil].pelanggan.alamat.kota
                 << ", " << temp[hasil].pelanggan.alamat.provinsi << "\n";
            cout << "Paket Wisata   : " << temp[hasil].paket.destinasi << "\n";
            cout << "Fasilitas      : " << temp[hasil].paket.fasilitas << "\n";
            cout << "Status         : " << temp[hasil].status << "\n";
            jeda();
        }
        else if (pil == 2)
        {
            header(" CARI PESANAN - LINEAR SEARCH (Nama Pelanggan) ");

            string keyword;
            inputString("Masukkan nama / kata kunci: ", keyword, "Kata kunci pencarian");

            cout << "\n";
            garis(80, '-');
            cout << "[ Linear Search ] Mencari nama yang mengandung: \"" << keyword << "\"\n";
            garis(80, '-');

            int ditemukan = 0, posisi = 0;
            tabelHeader();
            while (true)
            {
                int hasil = linearSearchNama(temp, n, &keyword, posisi);
                if (hasil == -1)
                    break;
                tabelBaris(temp[hasil]);
                ditemukan++;
                posisi = hasil + 1;
            }
            garis(105, '-');

            if (ditemukan == 0)
                throw DataTidakDitemukan(
                    "Tidak ada pelanggan dengan nama yang mengandung \"" + keyword + "\".");

            cout << "Ditemukan " << ditemukan << " pemesanan.\n";
            jeda();
        }
    } while (pil != 0);
}

void menuUtama()
{
    int pil;
    do
    {
        header(" MENU UTAMA ");
        cout << "Login: " << aktif.nama << " [" << aktif.role << "]\n";
        cout << "1. Tambah Pesanan\n2. Lihat Pesanan\n3. Ubah Pesanan\n4. Hapus Pesanan\n";
        cout << "5. Sorting Pesanan\n6. Searching Pesanan\n";
        if (aktif.role == "admin")
            cout << "7. Daftar User\n";
        cout << "0. Logout\n";
        inputInt("Pilihan: ", pil);

        try
        {
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
                menuSorting(&aktif);
                break;
            case 6:
                menuSearching(&aktif);
                break;
            case 7:
                if (aktif.role == "admin")
                    daftarUser();
                break;
            }
        }
        catch (const KesalahanInput &e)
        {
            cout << "\n[!] " << e.what() << "\n";
            jeda();
        }
        catch (const DataTidakDitemukan &e)
        {
            cout << "\n[!] " << e.what() << "\n";
            jeda();
        }
        catch (const KapasitasPenuh &e)
        {
            cout << "\n[!] " << e.what() << "\n";
            jeda();
        }
        catch (const AksesDitolak &e)
        {
            cout << "\n[!] " << e.what() << "\n";
            jeda();
        }
        catch (const StatusTidakValid &e)
        {
            cout << "\n[!] " << e.what() << "\n";
            jeda();
        }
        catch (const KesalahanTravel &e)
        {
            cout << "\n[!] " << e.what() << "\n";
            jeda();
        }
        catch (const exception &e)
        {
            cout << "\n[SYSTEM ERROR] " << e.what() << "\n";
            jeda();
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
            try
            {
                string n, p;
                cout << "Nama: ";
                cin >> n;
                cout << "NIM : ";
                cin >> p;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (n.empty() || p.empty())
                    throw KesalahanInput("Nama dan NIM tidak boleh kosong.");

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
                    throw DataTidakDitemukan("Nama/NIM tidak ditemukan. Login gagal.");
            }
            catch (const KesalahanInput &e)
            {
                cout << "\n[!] " << e.what() << "\n";
                jeda();
            }
            catch (const DataTidakDitemukan &e)
            {
                cout << "\n[!] " << e.what() << "\n";
                jeda();
            }
        }
        else if (pil == 2)
        {
            try
            {
                if (jmlUser >= MAX_USER)
                    throw KapasitasPenuh("Jumlah user sudah mencapai batas maksimum.");

                User u;
                cout << "Nama: ";
                cin >> u.nama;
                cout << "NIM : ";
                cin >> u.nim;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (u.nama.empty() || u.nim.empty())
                    throw KesalahanInput("Nama dan NIM tidak boleh kosong.");

                for (int i = 0; i < jmlUser; i++)
                {
                    if (users[i].nim == u.nim)
                        throw KesalahanInput("NIM '" + u.nim + "' sudah terdaftar.");
                }

                u.role = "user";
                users[jmlUser++] = u;
                cout << "Registrasi Berhasil! Silakan login.\n";
                jeda();
            }
            catch (const KapasitasPenuh &e)
            {
                cout << "\n[!] " << e.what() << "\n";
                jeda();
            }
            catch (const KesalahanInput &e)
            {
                cout << "\n[!] " << e.what() << "\n";
                jeda();
            }
        }

    } while (pil != 0);

    cout << "Terima kasih telah menggunakan Agen Travel Nusantara!\n";
    return 0;
}