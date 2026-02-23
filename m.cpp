#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <thread>
using namespace std;

// class Data{
//     string time[3];
//     int N_table[5];
//     string name[15];
// };

void showData(vector<string> &a, string b[], int c[], int N, int M)
{
    cout << right << setw(12) << "Time/Table";
    for (int i = 0; i < M; i++)
    {
        if (i == 0)
            cout << setw(8) << c[i];
        if (i > 0)
            cout << setw(10) << c[i];
    }
    cout << "\n--------------|---------|---------|---------|---------|---------|\n";
    for (int i = 0; i < N * M; i++)
    {
        if (i == 0)
            cout << setw(12) << b[i];
        cout << setw(10);
        if (a[i] == "Emply")
            cout << "Emply";
        else
            cout << "Booked";
        if (i == 14)
            break;
        if ((i + 1) % M == 0)
        {
            cout << endl;
            cout << "--------------|---------|---------|---------|---------|---------|\n";
            cout << setw(12) << b[(i + 1) / M];
        }
    }
    cout << "\n--------------|---------|---------|---------|---------|---------|\n";
    cout << "Choose time: 10:00-12:00 -> Enter 1\n"; // ใหม่
    cout << "Choose time: 13:00-15:00 -> Enter 2\n"; // ใหม่
    cout << "Choose time: 16:00-18:00 -> Enter 3\n"; // ใหม่
}

void updatefile(vector<string> &status)
{
    ofstream new_data("status table.txt");
    for (int i = 0; i < status.size(); i++)
        new_data << status[i] << endl;
    new_data.close();
}

bool Check(vector<string> &a, int time, int table, string name)
{
    int index = (time - 1) * 5 + (table - 1);

    if (a[index] == "Emply")
    {
        a[index] = name; // เก็บชื่อลูกค้า
        return true;
    }
    return false;
}

void receipt(string name, int table, string time)
{
    cout << "\n========================================\n";
    cout << "       TABLE RESERVATION RECEIPT     \n";
    cout << "========================================\n";
    cout << left << setw(15) << " Customer" << ": " << name << endl;
    cout << left << setw(15) << " Table No." << ": " << table << endl;
    cout << left << setw(15) << " Time" << ": " << time << endl;
    cout << "----------------------------------------\n";
    cout << "        Thank you for booking           \n";
    cout << "========================================\n\n";
}

bool searchByName(vector<string> &status, string time[], int N1, int N2)
{
    string name;
    cout << "\nEnter your nickname to search: ";
    cin >> name;

    for (int i = 0; i < status.size(); i++)
    {
        if (status[i] == name)
        {
            int timeIndex = i / N2;
            int tableIndex = i % N2;

            receipt(name, tableIndex + 1, time[timeIndex]);
            return true;
        }
    }

    cout << "\nO ================================= O\n";
    cout << "| No booking found under this name. |\n";
    cout << "O ================================= O\n\n";

    return false;
}

int main()
{
    // Data Customer = {{"10:00-12:00","13:00-15:00","16:00-18:00"},{1,2,3,4,5},"Emply","Emply","Emply","Emply","Emply","Emply","Emply","Emply","Emply","Emply","Emply","Emply","Emply","Emply","Emply"};
    string Nickname;
    int N_time;
    int Table;
    string time[] = {"10:00-12:00", "13:00-15:00", "16:00-18:00"};
    int N_table[] = {1, 2, 3, 4, 5};
    string Ans = "Y";
    int count = 0;
    int N1 = sizeof(time) / sizeof(time[0]);
    int N2 = sizeof(N_table) / sizeof(N_table[0]);
    vector<string> status(N1 * N2);

    ifstream source;
    source.open("status table.txt"); // เปิดไฟล์ที่จะอ่าน
    string texline;
    int number = 0;
    while (getline(source, texline))
    {
        status[number] = texline;
        number++;
    }
    source.close(); // ปิดไฟล์ที่อ่าน

    cout << "========================================\n";
    cout << "     Welcome to 3Student restaurant\n";
    cout << "========================================\n";

    cout << "Have you already booked a table? (Y/N): ";
    string firstChoice;
    cin >> firstChoice;

     if (firstChoice == "Y" || firstChoice == "y")
    {
        bool found = searchByName(status, time, N1, N2);

        if (found)
            return 0;   // เจอแล้วจบโปรแกรมเลย
    }

    Ans = "Y";

    // เข้าหน้าจองโต๊ะ
    while (Ans == "Y" || Ans == "y")
    {
        cout << "\nPlease select the table you wish to reserve:\n";
        showData(status, time, N_table, N1, N2);

        cout << "[Choose Time] : ";
        cin >> N_time;

        while (N_time < 1 || N_time > 3)
        {
            cout << "Wrong Choice! Please choose again: ";
            cin >> N_time;
        }

        cout << "[Choose Table] : ";
        cin >> Table;

        while (Table < 1 || Table > 5)
        {
            cout << "Wrong Table! Please choose again: ";
            cin >> Table;
        }

        int index = (N_time - 1) * 5 + (Table - 1);

        if (status[index] == "Emply")
        {
            
            cout << "[Enter Nickname] : ";
            cin >> Nickname;

            status[index] = Nickname;   // เก็บชื่อจริง
            updatefile(status);

            receipt(Nickname, Table, time[N_time - 1]);
            cout << "Would you like to make another booking? (Y/N): ";
            cin >> Ans;
        }
        else
        {
            cout << "\nO ========================================================= O\n";
            cout << "|     Sorry, Table already booked! Please choose again.     |\n";
            cout << "O ========================================================= O\n";

            this_thread::sleep_for(chrono::seconds(2));
        }
    }

    return 0;
}
