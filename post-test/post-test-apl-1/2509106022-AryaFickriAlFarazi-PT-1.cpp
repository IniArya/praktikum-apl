#include <iostream>
using namespace std;

int main()
{
    string nama, password;
    string namaBenar = "arya";
    string passBenar = "022";
    int kesempatan = 0;

    while (kesempatan < 3)
    {
        cout << "=====================================\n";
        cout << "               LOGIN                 \n";
        cout << "=====================================\n";
        cout << "Nama     : ";
        cin >> nama;
        cout << "Password : ";
        cin >> password;

        if (nama == namaBenar && password == passBenar)
        {
            cout << "\nLogin Berhasil, Selamat datang " << nama << "\n";
            break;
        }
        else
        {
            kesempatan++;
            cout << "\nLogin Gagal! Kesempatan tersisa: "
                 << 3 - kesempatan << endl
                 << endl;
        }
    }

    if (kesempatan == 3)
    {
        cout << "\nAnda gagal login 3 kali. Program akan berhenti.\n";
        return 0;
    }

    int pilihan;
    do
    {
        cout << "\n=====================================\n";
        cout << "          MENU KONVERSI WAKTU        \n";
        cout << "=====================================\n";
        cout << "1. Konversi Jam -> Menit dan Detik\n";
        cout << "2. Konversi Menit -> Jam dan Detik\n";
        cout << "3. Konversi Detik -> Jam dan Menit\n";
        cout << "4. Keluar\n";
        cout << "=====================================\n";
        cout << "Pilih menu (1-4): ";
        cin >> pilihan;

        if (pilihan == 1)
        {
            double jam;
            cout << "\nMasukkan jumlah Jam: ";
            cin >> jam;

            double menit = jam * 60;
            double detik = jam * 3600;

            cout << "Hasil setelah dikonversi:\n";
            cout << jam << " Jam = "
                 << menit << " Menit dan "
                 << detik << " Detik\n";
        }
        else if (pilihan == 2)
        {
            double menit;
            cout << "\nMasukkan jumlah Menit: ";
            cin >> menit;

            double jam = menit / 60;
            double detik = menit * 60;

            cout << "Hasil setelah dikonversi:\n";
            cout << menit << " Menit = "
                 << jam << " Jam dan "
                 << detik << " Detik\n";
        }
        else if (pilihan == 3)
        {
            double detik;
            cout << "\nMasukkan jumlah Detik: ";
            cin >> detik;

            double jam = detik / 3600;
            double menit = detik / 60;

            cout << "Hasil setelah dikonversi:\n";
            cout << detik << " Detik = "
                 << jam << " Jam dan "
                 << menit << " Menit\n";
        }
        else if (pilihan == 4)
        {

            cout << "\nprogram akan dihentikan\n";
        }
        else
        {
            cout << "\nPilihan tidak sesuai, Silakan pilih 1-4.\n";
        }

    } while (pilihan != 4);

    return 0;
}
