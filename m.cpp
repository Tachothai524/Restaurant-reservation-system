#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <thread>
#include <limits>
#include <cctype>
#include<windows.h>
#include<algorithm>
using namespace std;

void setTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

string toUpperStr(string x) //ทำให้ค่าเข้ามาเป็นพิมพ์ใหญ่ทุกตัว
{
    string y = x;

    for(int i = 0; i < x.size(); i++){
        y[i] = toupper(x[i]);  
    }
    return y;
}

void chekdata_cin(int &input) //เช็คว่า cin ที่เข้าไปนั้นทำให้เกิด error ไหม ถ้าใช่ให้ล้างค่าออกและเปลี่ยนค่านั้นเป็น 0
{
    if(cin.fail()){ //เช็คว่า cin ที่เข้าไปนั้นทำให้เกิด error ไหม
        cin.clear(); // ล้างค่า cin ออก
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        input = 0;
    }   
}

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

        if (a[i] == "Emply"){
            setTextColor(2);
            cout << "Emply";
            setTextColor(7);
        }else{
            setTextColor(8);
            cout << "Booked";
            setTextColor(7);
        }

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
        if ( toUpperStr(status[i]) == toUpperStr(name) )
        {
            int timeIndex = i / N2;
            int tableIndex = i % N2;

            receipt(status[i], tableIndex + 1, time[timeIndex]);
            
        }
    }

    for (int i = 0; i < status.size(); i++)
    {
        if ( toUpperStr(status[i]) == toUpperStr(name) )
        {
            return true;
        }
    }
    setTextColor(12);
    cout << "\nO ================================= O\n";
    cout << "| No booking found under this name. |\n";
    cout << "O ================================= O\n\n";
    setTextColor(7);

    return false;
}

int main()
{
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
    bool isAllSpaces;

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
for (char &c : firstChoice)
        c = tolower(c);

    while (firstChoice != "y" && firstChoice != "yes" && firstChoice != "n" && firstChoice != "no") // แก้ให้ต้องใส่ input ใหม่จนกว่าถูก
    {
        cout << "Please answer (Yes/No/Y/N): ";
        cin >> firstChoice;
        for (char &c : firstChoice)
            c = tolower(c);
    }

    if (firstChoice == "y" || firstChoice == "yes")
    {
        bool found = searchByName(status, time, N1, N2);
        if (found)
            return 0;
    }

    Ans = "Y";

    // เข้าหน้าจองโต๊ะ
    while (Ans == "Y")
    {
        cout << "\nPlease select the table you wish to reserve:\n";
        showData(status, time, N_table, N1, N2);

        cout << "[Choose Time] : ";
        cin >> N_time;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        chekdata_cin(N_time); // เช็คว่า cin นั้น error ไหม ถ้า error ให้เคลียและเปลี่ยน cin นั้น = 0

        while (N_time < 1 || N_time > 3)
        {
            cout << "Wrong Choice! Please choose again: ";
            cin >> N_time;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            chekdata_cin(N_time); // เช็คว่า cin นั้น error ไหม ถ้า error ให้เคลียและเปลี่ยน cin นั้น = 0
        }

        cout << "[Choose Table] : ";
        cin >> Table;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        chekdata_cin(Table); // เช็คว่า cin นั้น error ไหม ถ้า error ให้เคลียและเปลี่ยน cin นั้น = 0

        while (Table < 1 || Table > 5)
        {
            cout << "Wrong Table! Please choose again: ";
            cin >> Table;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            chekdata_cin(Table); // เช็คว่า cin นั้น error ไหม ถ้า error ให้เคลียและเปลี่ยน cin นั้น = 0
        }

        int index = (N_time - 1) * 5 + (Table - 1);

        if (status[index] == "Emply")
        {
            
            cout << "[Enter Nickname] : ";
            getline(cin,Nickname);

            // isAllSpaces = all_of(Nickname.begin(),Nickname.end(),[](unsigned char ch) {return isspace(ch);});
            while(Nickname.empty() || Nickname == " "){
                cout << "Please Write Nickname : ";
                getline(cin,Nickname);
            }

            status[index] = Nickname;   // เก็บชื่อจริง
            updatefile(status);

            receipt(Nickname, Table, time[N_time - 1]);
            cout << "Would you like to make another booking? (Y/N): ";
            cin >> Ans;
            for (char &c : Ans)
                c = tolower(c);

            while (Ans != "y" && Ans != "yes" && Ans != "n" && Ans != "no") // แก้ให้ต้องใส่ input ใหม่จนกว่าถูก
            {
                cout << "Please answer (Yes/No/Y/N): ";
                cin >> Ans;
                for (char &c : Ans)
                    c = tolower(c);
            }

            if (Ans == "yes" || Ans == "y")
                Ans = "Y";
            if (Ans == "no" || Ans == "n")
                Ans = "N";
        }
        else
        {
            setTextColor(12);
            cout << "\nO ========================================================= O\n";
            cout << "|     Sorry, Table already booked! Please choose again.     |\n";
            cout << "O ========================================================= O\n";
            setTextColor(7);

            this_thread::sleep_for(chrono::seconds(2));
        }
    }

    return 0;
}
