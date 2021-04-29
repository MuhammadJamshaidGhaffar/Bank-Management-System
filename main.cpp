//created By Muhammad Jamshaid Ghaffar
#include <fstream>
#include <iostream>
#include <iomanip>
#include<conio.h>
#include <vector>
#include<stdio.h>
#include <string.h>
#include <string> 
#include <cstring>
#include <dirent.h>
//------- My own Includes----------
#include "menu_api.h"
//  GOLBAL VARIABLES
int  max_int = 2147483647;
const std::string accounts_path = "./data/";
const std::string admin_accounts_path  = "./data/";
const std::string transaction_path = "./log/";
const std::string accounts_file_name = "Accounts.dat";
const std::string admin_accounts_file_name = "Data.dat";
COORD destCoord;
HANDLE hStdout = createHandle();
//--------- Menus ----------
namespace Menu{
	MenuClass main{{"CREATE NEW ACCOUNT" , "LOGIN" , "ADMIN LOGIN" , "SHOW ALL ACCOUNT HOLDERS" , "Credits"}};
	//----- admin Menus ----------
	MenuClass adminMenu {{"CHANGE PASSWORD" ,  "SHOW ALL ACCOUNTS" , "DELETE ACCOUNT" , "CREATE NEW ADMIN ACCOUNT" , "SHOW TRANSACTION HISTORY" , "LOGOUT" }};
	MenuClass adminDeleteAccount { {"DELETE  ACCOUNT HOLDER" , "DELETE ADMIN ACCOUNT"}};
	MenuClass adminShowAllAccounts {{"SHOW ALL ACCOUNT HOLDERS" , "SHOW ALL ADMIN ACCOUNTS" }};
	MenuClass adminChangePass { { "CHANGE ACCOUNT HOLDER PASS" , "CHANGE YOUR PASS"}};
	//----- Logged In User Menu ------------
	MenuClass loggedIn { { "DISPLAY ACCOUNT INFO", "MODIFY ACCOUNT" ,"DEPOSIT" , "WITHDRAW","SHOW TRANSACTION HISTORY" , "LOGOUT" , "DELETE ACCOUNT"  } };
	MenuClass modify { { "CHANGE NAME" , "CHANGE PASSWORD" } };

}
//---  menuSetUp------------
void menuSetUp();
//--- Global Functions---------
void displayTime();
//---------> class  of acoount holder <-----------


//*******************************************************
//                ERROR HANDLING
//*******************************************************
int getInt(int min,int max,std::string input_string)
{
    int num = 0;
    bool flag = true;
    while (true)
    {
        std::cin >> num;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            std::cout << "\n\n\tINVALID INPUT!";
            std::cout << "\n\n\t";
            system("pause");
            std::cout << "\n\n\t" << input_string ;
            continue;
        }
        else
        {
            std::cin.ignore(32767, '\n');
            if (num >= min && num <= max)
            {
                return num;
            }
            std::cout << "\n\n\tPLEASE ENTER NUMBER BETWEEN " << min << " and " << max<< "\n\n\t";
            system("pause");
            std::cout << "\n\n\t" << input_string;
            
        }
    }
}
////------------GET PASSWORD----------------------------
void getHiddenPass(char* pass)
{
    int i = 0;
    pass[0] = '\0';
    while (true) {

        // Get the hidden input 
        // using getch() method 
        char x{};
          x  = _getch();
        if (x == 13)
        {
            break;
        }
        else if (x == 8)
        {
            if (i > 0)
            {
                std::cout << "\b \b";
                i--;
                pass[i] = '\0';
            }
                continue;
            
        }
        
        pass[i] = x;
        pass[i + 1] = '\0';
        i++;
        // Print * to show that 
        // a character is entered 

        printf("*");
    }
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

enum class TransactionType {
    DEPOSIT,
    WITHDRAW
};
std::string getTransactionType(TransactionType temp_type)
{
    switch (temp_type)
    {
    case TransactionType::DEPOSIT:
        return "DEPOSIT";
        break;
    case TransactionType::WITHDRAW:
        return "WITHDRAW";
        break;
    default:
        break;
    }
}

class transaction {
public:
    time_t time = 0;
    TransactionType type;
    int cash = 0;
    void add(TransactionType temp_type, int temp_cash)
    {
        time = std::time(0);
        type = temp_type;
        cash = temp_cash;
    }

};

void addTransaction(int acno, TransactionType temp_type, int temp_cash)
{
    transaction new_transaction;
    new_transaction.add(temp_type, temp_cash);

    std::ofstream outfile{ transaction_path + std::to_string(acno) + ".dat",std::ios::binary | std::ios::app };
    if (!outfile)
    {
        std::cout << "\n\n\tCannot open Transaction File";
        return;
    }

    outfile.write((char*)&new_transaction, sizeof(new_transaction));
    outfile.close();
}
void showTransaction(int acno)
{
    transaction new_transaction;
    system("cls");
    std::cout << "\n\tACCOUNT NUMBER : " << acno;
    std::cout << "\n";
    std::cout << "\n--------------------------------------------------------------------------";
    std::cout  <<"\n"<< std::left << std::setw(20) << "  TYPE" << std::left << std::setw(1)<<" " << std::left << std::setw(20) << "CASH" << std::left << std::setw(30) << "  TIME";
    std::cout << "\n--------------------------------------------------------------------------";
    std::ifstream infile{ transaction_path + std::to_string(acno) + ".dat",std::ios::binary };
    if (!infile)
    {
        std::cout << "\n\n\tCannot open Transaction File!\n\n\tTHE FILE IS EITHER DELETED OR NOT CREATED YET!";
        std::cout << "\n\n\t";
        system("pause");
        return;
    }

    while (infile.read((char*)&new_transaction, sizeof(new_transaction)))
    {
        char* printed_time = ctime(&new_transaction.time);
        std::cout << "\n"  << std::left << std::setw(20) <<"  "+ getTransactionType(new_transaction.type) << std::left << std::setw(3) << "RS." << std::left << std::setw(20) << new_transaction.cash << std::left << std::setw(30) << printed_time;
    }

    infile.close();
    std::cout << "\n\n\t";
    system("pause");
}
void deleteTransaction(int acno)
{
    remove((transaction_path + std::to_string(acno) + ".dat").c_str());
    std::ifstream infile{ transaction_path + std::to_string(acno) + ".dat",std::ios::binary  };
   
}


//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//*******************************************************
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
class account {
public:
    char name[40]{}, pass[20]{};
    int acno = 0, balance = 0;


    int getAcno()
    {
        return acno;
    }
    char* getPass()
    {
        return pass;
    }
    char* getName()
    {
        return name;
    }
    int getBalance()
    {
        return balance;
    }
    void createAccount()
    {
            std::cout << "\n\n\tEnter the account number : ";            
            acno = getInt(0, max_int, "\n\n\tEnter the account number : ");

        std::cout << "\n\tEnter the account pass : ";
        getHiddenPass(pass);
        //std::cin.ignore();
        while (strcmp(name, "") == 0)
        {


            std::cout << "\n\n\tEnter the name of account holder : ";
            std::cin.getline(name, 40);
            std::cin.ignore();
        }
        std::cout << "\n\tEnter the current balance : ";
        balance = getInt(0, max_int, "\n\tEnter the current balance : ");
        //std::cin.ignore();
        

    }
    void show()
    {
        system("cls");
        std::cout << "\n\n\tACCOUNT NUMBER : " << acno;
        std::cout << "\n\n\tACCOUNT HOLDER NAME : " << name;
        std::cout << "\n\n\tBALANCE : RS." << balance;
        std::cout << "\n\n\t";
        system("pause");
    }
    int modify()
    {


        system("cls");
        int option;
        std::cout << "\n\n\t\tMODIFY ACCOUNT";
        option = Menu::modify.optionsLoop(hStdout , destCoord);
        switch (option)
        {
        case 1:
            system("cls");
            std::cout << "\n\n\t\tMODIFYING NAME";
            std::cout << "\n\n\tENTER NEW NAME : ";
            std::cin.getline(name, 40);
            
            break;
        case 2:
            system("cls");
            std::cout << "\n\n\t\tMODIFYING PASSWORD";
            std::cout << "\n\n\tENTER NEW PASSWORD : ";
            getHiddenPass(pass);
            
            break;
        case 0:
            return 0;
        default:
            break;
        }

        std::cin.ignore();
        return 1;

    }
    void deposit()
    {
        system("cls");
        int deposit = 0;
        std::cout << "\n\n\t\tDEPOSIT MONEY";
        std::cout << "\n\n\tENTER THE AMOUNT TO DEPOSIT : ";
        deposit = getInt(0, max_int, "\n\n\tENTER THE AMOUNT TO DEPOSIT : ");
        //std::cin >> deposit;
        //std::cin.ignore();
        balance += deposit;
        addTransaction(acno, TransactionType::DEPOSIT, deposit);
    }
    void withdraw()
    {
        system("cls");
        int withdraw = 0;
        std::cout << "\n\n\t\tWITHDRAW MONEY";
        std::cout << "\n\n\tENTER THE AMOUNT TO WITHDRAW : ";
        withdraw = getInt(0, max_int, "\n\n\tENTER THE AMOUNT TO WITHDRAW : ");
        addTransaction(acno, TransactionType::WITHDRAW, withdraw);
        balance -= withdraw;
    }

};
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//*******************************************************
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
int getTotalBankMoney()
{
    account user;
    int total_money = 0;
    std::ifstream infile{ accounts_path + accounts_file_name , std::ios::binary };
    if (!infile)
    {
        std::cout << "\n\tError! cannot open the file! \n\tThe file may be Delete or is not created yet!";
        return 0;
    }

    while (infile.read((char*)&user, sizeof(user)))
    {
        total_money += user.getBalance();
    }
    infile.close();
    return total_money;
}
//*****************************accounts_path + accounts_file_name**************************
//                LOGGED IN  MENU FUNCTIONS
//*******************************************************

//*******************************************************
//                READ DATA After Login FROM FILE
//*******************************************************
void showAccount(const int& pos, account& user)
{
    std::ifstream infile{ accounts_path + accounts_file_name , std::ios::binary };
    if (!infile)
    {
        std::cout << "\n\tError! cannot open the file";
        return;
    }
    infile.seekg(pos);
    infile.read((char*)&user, sizeof(user));
    infile.close();
    std::cout << "\n\n     ACCOUNT NUMBER : " << user.getAcno() << "\t\t\tBALANCE : RS." << user.getBalance();
    std::cout << "\n\n\t\t\tNAME : " << user.getName();


}
//*******************************************************
//                MODIFY ACCOUNT ON FILE
//*******************************************************
void modifyAccount(const int& pos, account& user)
{
    std::ofstream outfile{ accounts_path + accounts_file_name , std::ios::binary | std::ios::in | std::ios::out};
    if (!outfile)
    {
        std::cout << "\n\tError! cannot open the file ";
        return;
    }
    outfile.seekp(pos, std::ios::beg);
    outfile.write((char*)&user, sizeof(user));
    outfile.close();
    std::cout << "\n\tAccount modified!";
    std::cout << "\n\n\t";
    system("pause");
}
//*******************************************************
//                DEPOSIT
//*******************************************************
void depositAccount(const int& pos, account& user)
{
    std::ofstream outfile{ accounts_path + accounts_file_name , std::ios::binary | std::ios::out | std::ios::in };
    if (!outfile)
    {
        std::cout << "\n\tError! cannot open the file ";
        return;
    }
    outfile.seekp(pos, outfile.beg);
    outfile.write((char*)&user, sizeof(user));
    outfile.seekp(0, outfile.end);
    outfile.close();
    std::cout << "\n\tMONEY DEPOSITED!";
    std::cout << "\n\n\t";
    system("pause");
}
//*******************************************************
//                WITHDRAW
//*******************************************************
void withdrawAccount(const int& pos, account& user)
{
    std::ofstream outfile{ accounts_path + accounts_file_name , std::ios::binary | std::ios::out | std::ios::in };
    if (!outfile)
    {
        std::cout << "\n\tError! cannot open the file ";
        return;
    }
    outfile.seekp(pos, std::ios::beg);
    outfile.write((char*)&user, sizeof(user));
    outfile.close();
    std::cout << "\n\tMONEY WITHDRAWN!";
    std::cout << "\n\n\t";
    system("pause");
}
//*******************************************************
//                DELETE ACCOUNT
//*******************************************************
void closeAccount(const int& acno)
{
    
    account new_account;
    {
        bool flag = false;
    std::ifstream infile(accounts_path + accounts_file_name, std::ios::binary);
    if (!infile)
    {
        std::cout << "\n\tError! cannot open the file to read ";
        return;
    }
    while (infile.read((char*)&new_account, sizeof(new_account)))
    {
        if (new_account.getAcno() == acno)
        {
            flag = true;
            
        }
    }
    infile.close();
    if (flag == false)
    {
        std::cout << "\n\n\tSORRY ACCOUNT NOT FOUND!";
        std::cout << "\n\n\t";
        system("pause");
        return;
    }
     }
    std::ifstream infile1(accounts_path + accounts_file_name, std::ios::binary);
    if (!infile1)
    {
        std::cout << "\n\tError! cannot open the file to read ";
        return;
    }
    std::ofstream outfile(accounts_path + "temp.dat", std::ios::binary | std::ios::app);
    if (!outfile)
    {
        std::cout << "\n\tError! cannot open the file to write ";
        return;
    }
    while (infile1.read((char*)&new_account, sizeof(new_account)))
    {
        if (new_account.getAcno() != acno)
        {
            outfile.write((char*)&new_account, sizeof(new_account));
        }
    }
    infile1.close();
    outfile.close();
    remove((accounts_path + accounts_file_name).c_str());
    rename((accounts_path + "temp.dat").c_str() , (accounts_path + accounts_file_name).c_str());
    system("cls");
    std::cout << "\n\n\tACCOUNT DELETED!";
    deleteTransaction(new_account.getAcno());
    std::cout << "\n\n\t";
    system("pause");
}

//*******************************************************
//                LOGGED IN USER INTERFACE MENU
//*******************************************************
void loggedIn(const int& pos)
{
    while (true)
    {
        system("cls");
        //refreshing Accounts data after every loop
        account user;
        showAccount(pos, user);
        //Looged In interface Menu

        int option = 0;
        option = Menu::loggedIn.optionsLoop(hStdout , destCoord);
        switch (option)
        {
        case 1:
            user.show();
            break;
        case 2:
        {
            while (user.modify())
            {

                modifyAccount(pos, user);
            }

            break;

        }
        case 3:
            user.deposit();
            depositAccount(pos, user);
            break;
        case 4:
            user.withdraw();
            withdrawAccount(pos, user);
            break;
        case 5:
            showTransaction(user.acno);
            break;
        case 6:
        case 0:
            return;
        case 7:
            closeAccount(user.getAcno());           
          
            return;
        default:
            break;
        }
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//---------------------                      ----------------------------------
//---------------------         ADMIN        ----------------------------------
//---------------------                      ----------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//*******************************************************
//                ADMIN FUCNTIONS
//*******************************************************
enum class AccountType {
    OWNER,
    CO_OWNER,
    ADMIN,
    MAX_TYPES//  ADMIN 
};                        //  ENUM
std::string getAccountTypeName(AccountType type)
{
    std::string name;
    if (type == AccountType::OWNER)
    {
        name = "OWNER";
        return name;
    }
    else if (type == AccountType::CO_OWNER)
    {
        name = "CO_OWNER";
        return name;
    }
    else if (type == AccountType::ADMIN)
    {
        name = "ADMIN";
        return name;
    }

}

class adminAccount
{
public:
    char name[40]{};
    char pass[20]{};
    AccountType type;
	
	adminAccount()
	{
	}
	
	adminAccount(const char* p_name, const char* p_pass, const AccountType p_type)
	{
		strcpy(name,p_name);
		strcpy(pass,p_pass);
		type = p_type;
	}
    char* getName()           //ADMIN
    {                         //CLASS
        return name;
    }
    char* getPass()
    {
        return pass;
    }
    AccountType getType()
    {
        return type;
    }
    void create()
    {
        std::cout << "\n\n\t\tCREATING ADMIN ACCOUNT";
        std::cout << "\n\n\tENETR USER NAME : ";
        std::cin.getline(name, 40);
        std::cin.ignore(32767, '\n');
        std::cout << "\n\n\tENETR PASSWORD : ";
        getHiddenPass(pass);
        //std::cin.getline(pass, 20);
        //std::cin.ignore(32767, '\n');
        std::cout << "\n\n\tSELECT ACCOUNT TYPE ";
        std::cout << "\n\n\t01. CO-OWNER";
        std::cout << "\n\n\t02. ADMIN";
        std::cout << "\n\n\tSELECT OPTION (1-2) : ";
        int option = 0;
        option = getInt(1, 2, "\n\n\tSELECT OPTION (1-2) : ");
        //std::cin >> option;
        //std::cin.ignore();
        switch (option)
        {
        case 1:
            
            type = AccountType::CO_OWNER;
            break;
        case 2:
            type = AccountType::ADMIN;
            break;
        default:
            break;
        }
    }


};
//*******************************************************
//                CREATE OWNER ACCOUNT
//*******************************************************
void createOwnerAccount()
{
    std::ifstream infile{ admin_accounts_path + admin_accounts_file_name , std::ios::binary | std::ios::in };
    if (!infile)
    {
        std::ofstream outfile{ admin_accounts_path + admin_accounts_file_name , std::ios::binary | std::ios::out };
        adminAccount admin{ "Jamshaid" , "avast" , AccountType::OWNER };
        outfile.write((char*)&admin, sizeof(admin));
        std::cin.ignore();
    }
}
//*******************************************************
//                READ DATA After Login FROM FILE
//*******************************************************
void showAdminAccount(const int& admin_pos, adminAccount& admin)
{
    std::ifstream infile{ admin_accounts_path + admin_accounts_file_name , std::ios::binary };
    if (!infile)
    {
        std::cout << "\n\tError! cannot open the file";
        return;
    }
    infile.seekg(admin_pos, std::ios::beg);
    infile.read((char*)&admin, sizeof(admin));
    infile.close();
    //std::cout << "name : " << admin.getName();
    std::cout << "\n\n      Name: " << std::left << std::setw(20) << admin.getName() << std::left << std::setw(10) << "Type : " << getAccountTypeName(admin.getType());
}
//*******************************************************
//             CHANGE ACCOUNT HOLDER PASS
//*******************************************************
void AdminChangePass()
{
    int acno=0;
    char pass[20]{};

    system("cls");
    //-----------COLLCETING INFORMATION---------------
    std::cout << "\n\n\t\tMODIFYING PASSWORD";
    std::cout << "\n\n\tENTER ACCOUNT NUMBER : ";
    acno = getInt(0, max_int, "\n\n\tENTER ACCOUNT NUMBER : ");
    //std::cin >> acno;
    //std::cin.ignore();
    std::cout << "\n\n\tENTER NEW PASSWORD : ";
    getHiddenPass(pass);
    //std::cin.getline(pass, 20);
    //std::cin.ignore();
    //-----------MODIFYING PASSWORD---------------
    std::ifstream infile{ accounts_path + accounts_file_name , std::ios::binary };
    if (!infile)
    {
        std::cout << "\n\n\tError! cannot open the file to read ";
        return;
    }
    account user;
    bool flag = false;
    int pos = 0;
    while (infile.read((char*)&user, sizeof(user)))
    {
        if (user.getAcno() == acno)
        {
            flag = true;
            pos = static_cast<int>(infile.tellg()) - static_cast<int>(sizeof(user));
            break;
        }
    }
    if (flag == false)
    {
        std::cout << "\n\n\tSorry! Account number not found!";
        std::cout << "\n\n\t";
        system("pause");
        return;
    }
    infile.close();
    std::ofstream outfile{ accounts_path + accounts_file_name , std::ios::binary | std::ios::in | std::ios::out };
    if (!outfile)
    {
        std::cout << "\n\tError! cannot open the file ";
        return;
    }

    strcpy(user.pass,  pass);
    outfile.seekp(pos);
    outfile.write((char*)&user, sizeof(user));
    outfile.close();
    std::cout << "\n\n\tPASSWORD MODIFIED";
    std::cout << "\n\n\t";
    system("pause");
    std::cin.ignore(32767, '\n');
}
//*******************************************************
//              ADMIN  SHOW ALL ACCOUNT HOLDERS WITH PASS
//*******************************************************
void AdminshowAllAccounts()
{
    system("cls");
    std::cout << "\n--------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(3) << "   " << std::left << std::setw(13) << "ACCOUNT" << std::setw(30) << "NAME" << std::left << std::setw(20) << "PASSWORD" << std::left << std::setw(10) << "CASH ";
    std::cout << "\n" << std::left << std::setw(12) << "   NUMBER";
    std::cout << "\n--------------------------------------------------------------------------------\n";
    std::ifstream infile{ accounts_path + accounts_file_name , std::ios::binary | std::ios::in };
    if (!infile)
    {
        std::cout << "\n\tError! cannot open the file";
        return;
    }
    account new_account;
    while (infile.read((char*)&new_account, sizeof(new_account)))
    {
        std::cout << "\n   " << std::left << std::setw(13) << new_account.getAcno() << std::setw(30) << new_account.getName() << std::left << std::setw(20) << new_account.getPass() << std::left << std::setw(10) << new_account.getBalance();
    }
    std::cout << "\n\n\t";
    system("pause");
}
//*******************************************************
//                DELETE ACCOUNT HOLDER ACCOUNT
//*******************************************************
void AdminDeleteAccount()
{
    system("cls");
    int acno = 0;
    std::cout << "\n\n\t\tDELETING ACCOUNT ";
    std::cout << "\n\n\tENTER THE ACCOUNT NUMBER : ";
    //std::cin >> acno;
    acno = getInt(0, max_int, "\n\n\tENTER THE ACCOUNT NUMBER : ");
    closeAccount(acno);
}
//*******************************************************
//                CHANGE ADMIN PASSWORD
//*******************************************************
void changeAdminPass(const int& pos, adminAccount& admin)
{
    system("cls");
    std::ofstream outfile{ admin_accounts_path + admin_accounts_file_name , std::ios::binary | std::ios::in | std::ios::out };
    if (!outfile)
    {
        std::cout << "\n\tError! cannot open the file ";
        return;
    }

    std::cout << "\n\n\t\tMODIFYING PASSWORD";
    std::cout << "\n\n\tENTER NEW PASSWORD : ";
    //std::cin.getline(admin.pass, 20);
    admin.pass[0] = '\0';
    std::cin.clear();
    getHiddenPass(admin.pass);
    outfile.seekp(pos, std::ios::beg);
    outfile.write((char*)&admin, sizeof(admin));
    outfile.close();
    std::cout << "\n\n\tAccount modified!";
    std::cout << "\n\n\t";
    system("pause");
}

//*******************************************************
//                CREATE ADMIN ACCOUNT
//*******************************************************
void createAdminAccount(AccountType type)
{
    system("cls");
    if (type == AccountType::ADMIN)
    {
        std::cout << "\n\n\tYOU DON'T HAVE PERMISSION";
        std::cout << "\n\n\t";
        system("pause");
        return;
    }
    adminAccount admin;
    admin.create();
    if (admin.getType() == AccountType::CO_OWNER)
    {
    	if(type == AccountType::CO_OWNER)
    	{
    		std::cout << "\n\n\tYOU CANNOT CREATE CO-OWNER ACCOUNT!";
	        std::cout << "\n\n\t";
	        system("pause");
	        return;
		}
    }
    {  //---- Checking if account already exists-----------
        adminAccount test_account;
        std::ifstream infile{ admin_accounts_path + admin_accounts_file_name , std::ios::binary };
        while (infile.read((char*)&test_account, sizeof(test_account)))
        {
            if (strcmp(test_account.getName() , admin.getName())==0)
            {
                system("cls");
                std::cout << "\n\n\n\tERROR! ACCOUNT ALREADY EXISTS!";
                std::cout << "\n\n\t";
                system("pause");
                return;
            }
        }
    }
    //----- opening admins file and writing admin account to disk------------
    std::ofstream outfile{ admin_accounts_path + admin_accounts_file_name , std::ios::binary | std::ios::out | std::ios::app };
    if (!outfile)
    {
        std::cout << "\n\n\tError! cannot open the file";
        std::cout << "\n\n\t";
        system("pause");
        return;
    }
    outfile.write((char*)&admin, sizeof(admin));
    outfile.close();
    std::cout << "\n\n\tACCOUNT CREATED!";
    std::cout << "\n\n\t";
    system("pause");
}
//*******************************************************
//       SHOW ALL OWNER/ADMIN/CO-OWNER ACCOUNTS
//*******************************************************
void showAllAdminAccounts(AccountType type)
{
    system("cls");
    std::cout << "\n--------------------------------------------------------------------------------\n";
    //if (type == AccountType::OWNER)
    //{
        std::cout << std::left << std::setw(5) << "     " << std::left << std::setw(30) << "NAME" << std::left << std::setw(20) << "PASSWORD" << std::left << std::setw(10) << "ACCOUNT TYPE";
    //}
    //else
   // {
   //     std::cout << std::left << std::setw(5) << "     " << std::left << std::setw(40) << "NAME" << std::left << std::setw(10) << "ACCOUNT TYPE";
    //}
    std::cout << "\n--------------------------------------------------------------------------------\n";

    std::ifstream infile{ admin_accounts_path + admin_accounts_file_name , std::ios::binary | std::ios::in };
    if (!infile)
    {
        std::cout << "\n\tError! cannot open the file";
        return;
    }
    adminAccount new_account;
    while (infile.read((char*)&new_account, sizeof(new_account)))
    {
        if (type == AccountType::OWNER)
        {
            std::cout << std::left << std::setw(5) << "     " << std::left << std::setw(30) << new_account.getName() << std::left << std::setw(20) << new_account.getPass() << std::left << std::setw(10) << getAccountTypeName(new_account.getType()) << "\n";

        }
        else {
            //std::cout << std::left << std::setw(5) << "     " << std::left << std::setw(40) << new_account.getName() << std::left << std::setw(10) << getAccountTypeName(new_account.getType()) << "\n";
            std::cout << std::left << std::setw(5) << "     " << std::left << std::setw(30) << new_account.getName() << std::left << std::setw(20) << "N/A" << std::left << std::setw(10) << getAccountTypeName(new_account.getType()) << "\n";
        }
    }
    std::cout << "\n\n\t";
    system("pause");
}
//*******************************************************
//                DELETE ADMIN ACCOUNT
//*******************************************************
void deleteAdminAccount(AccountType type )
{
    system("cls");
    if (type == AccountType::ADMIN)
    {
        std::cout << "\n\n\tYOU DON'T HAVE PERMISSION! " ;     
        std::cout << "\n\n\t";
        system("pause");
        return;
    }
    char name[40];
    std::cout << "\n\n\t\tDELETING ACCOUNT ";
    std::cout << "\n\n\tENTER THE USER NAME : ";
    std::cin.getline(name, 40);

    adminAccount new_account;
    std::ifstream infile_check(admin_accounts_path + admin_accounts_file_name, std::ios::binary);
    if (!infile_check)
    {
        std::cout << "\n\tError! cannot open the file to read ";
        return;
    }

    //--------------------------------------------------
    //--------------CHECKING PERMISSIONS---------------------
    //--------------------------------------------------
    bool account_found = false;
    while (infile_check.read((char*)&new_account, sizeof(new_account)))
    {
        if (strcmp(new_account.getName(), name) == 0)
        {
        	account_found = true;
            if (new_account.getType() <= type)
            {
                std::cout << "\n\n\tYOU DON'T HAVE PERMISSION TO DELETE " << getAccountTypeName(new_account.getType());
                std::cin.clear();
                std::cout << "\n\n\t";
                system("pause");
                return ;
            }
            if (new_account.getType() == AccountType::OWNER)
            {
                std::cout << "\n\n\tYOU CAANOT DELETE " << getAccountTypeName(new_account.getType());
                std::cin.clear();
                std::cout << "\n\n\t";
                system("pause");
                return ;
            }
        }
    }
    infile_check.close();
	//----- if acccount not found go back---------
	if(!account_found){
		std::cout << "\n\n\tAccount not found!";
        std::cin.clear();
        std::cout << "\n\n\t";
        system("pause");
        return ;
	}

    //--------------------------------------------------
    //--------------DELETING ACCOUNT---------------------
    //--------------------------------------------------
    std::ifstream infile(admin_accounts_path + admin_accounts_file_name, std::ios::binary);
    if (!infile)
    {
        std::cout << "\n\tError! cannot open the file to read ";
        return;
    }
    std::ofstream outfile(accounts_path + "temp.dat", std::ios::binary | std::ios::app);
    if (!outfile)
    {
        std::cout << "\n\tError! cannot open the file to write ";
        return;
    }
    while (infile.read((char*)&new_account, sizeof(new_account)))
    {
        if (strcmp(new_account.getName(), name) != 0)
        {
            outfile.write((char*)&new_account, sizeof(new_account));
        }
    }
    infile.close();
    outfile.close();
    remove((admin_accounts_path + admin_accounts_file_name).c_str());
    rename((accounts_path + "temp.dat").c_str(), (admin_accounts_path+admin_accounts_file_name).c_str());
    std::cout << "\n\n\tACCOUNT DELETED!";
    std::cout << "\n\n\t";
    system("pause");  
}
//*******************************************************
//                CHANGE PASS
//*******************************************************
void adminChangePassPanel(const int& admin_pos, adminAccount& admin)
{
    while (true)
    {
        system("cls");
        int option = 0; 
        option = Menu::adminChangePass.optionsLoop(hStdout , destCoord);
        switch (option)
        {
        case 1:
            AdminChangePass();
            break;
        case 2:
            changeAdminPass(admin_pos, admin);
            break;
        case 0:
            return;

        default:
            break;
        }
    }

}
//*******************************************************
//                SHOW ALL ACCOUNTS
//*******************************************************
void adminShowAllAccountsPanel(const int& admin_pos, adminAccount& admin)
{
    while (true)
    {
        system("cls");	
        int option = 0;
        option = Menu::adminShowAllAccounts.optionsLoop(hStdout , destCoord);
        switch (option)
        {

        case 1:
            AdminshowAllAccounts();
            break;
        case 2:
            showAllAdminAccounts(admin.getType());
            break;
        case 0:
            return;
        default:
            break;
        }
    }
}
//*******************************************************
//                DELETE ACCOUNT
//*******************************************************
void adminDeleteAccountPanel(const int& admin_pos, adminAccount& admin)
{
    while (true)
    {
        system("cls");
        adminAccount admin;
        showAdminAccount(admin_pos, admin);
        
        int option = 0;
        option = Menu::adminDeleteAccount.optionsLoop(hStdout , destCoord);
        switch (option)
        {

        case 1:
            AdminDeleteAccount();
            break;
        case 2:
            deleteAdminAccount(admin.getType());
            break;
        case 0:
            return;

        default:
            break;
        }
    }
}
//*******************************************************
//                ADMIN SHOW TRANSACTION HISTORY
//*******************************************************
void adminShowTransactionHistory()
{
    system("cls");
    int acno = 0;
    std::cout << "\n\n\tENTER THE ACCOUNT NUMBER : ";
    acno = getInt(0, max_int, "\n\n\tENTER THE ACCOUNT NUMBER : ");

    showTransaction(acno);
}

//*******************************************************
//                ADMIN USER INTERFACE
//*******************************************************
void adminLoggedIn(const int& admin_pos)
{
    int total_bank_money = 0;
    total_bank_money = getTotalBankMoney();
    {
    	
	}
    while (true)
    {
        system("cls");
        adminAccount admin;
        showAdminAccount(admin_pos, admin);
        std::cout << "\n\n          BANK MONEY :  RS." << total_bank_money;
	 
        
        int option = 0;
        option = Menu::adminMenu.optionsLoop(hStdout , destCoord);
        switch (option)
        {
        case 1:
            adminChangePassPanel(admin_pos, admin);
            break;

        case 2:
            adminShowAllAccountsPanel(admin_pos, admin);
            break;
        case 3:
            adminDeleteAccountPanel(admin_pos, admin);
            break;
        case 4:
            createAdminAccount(admin.getType());
            break;
        case 5:
            adminShowTransactionHistory();
            break;
        case 6 :
		case 0:
            system("cls");
            std::cout << "\n\n\n\tYOU ARE LOGGED OUT!";
            std::cout << "\n\n\t";
            system("pause");
            
            return;

        default:
            break;
        }
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//---------------------    MAIN MENU           ----------------------------------
//---------------------                      ----------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//*******************************************************
//                MAIN MENU FUNCTIONS
//*******************************************************
//*******************************************************
//                CREATE ACCOUNT
//*******************************************************
void writeAccount()
{
    system("cls");
    
    std::ofstream outfile{ accounts_path + accounts_file_name , std::ios::binary | std::ios::app };
    if (!outfile)
    {
        std::cout << "\n\n\tError! cannot open the file ";
        return;
    }
    account new_account;
    new_account.createAccount();
    {
        account test_account; 
        std::ifstream infile{ accounts_path + accounts_file_name , std::ios::binary };
        while (infile.read((char*)&test_account, sizeof(test_account)))
        {
            if (test_account.getAcno() == new_account.getAcno())
            {
                system("cls");
                std::cout << "\n\n\n\tERROR! ACCOUNT ALREADY EXISTS!";
                std::cout << "\n\n\t";
                system("pause");
                return;
            }
        }
    }
    outfile.write((char*)&new_account, sizeof(new_account));
    std::cout << "\n\n\tAccount created!";
    outfile.close();
    std::cout << "\n\n\t";
    system("pause");
}


//*******************************************************
//                LOGIN  
//*******************************************************

void login(int& pos, bool& logged_in)
{
    system("cls");
    std::ifstream infile{ accounts_path + accounts_file_name, std::ios::binary | std::ios::in };
    if (!infile)
    {
        std::cout << "\n\n\tError! cannot open the file";
        return;
    }

    int acno = 0;
    char pass[20]{};
    //Getting acno 
    std::cout << "\n\n\tEnter the account number: ";
    acno = getInt(0,max_int, "\n\n\tEnter the account number: ");
    //std::cin >> acno;
    //std::cin.ignore();
    //getting Password
    std::cout << "\n\n\tPasswod : ";
    getHiddenPass(pass);
    //std::cin.ignore();
    //Reading data and comapring
    account new_account;
    bool flag = false;
    while (infile.read((char*)&new_account, sizeof(new_account)))
    {
        if (new_account.getAcno() == acno)
        {
            flag = true;
            if (strcmp(pass, new_account.getPass()) == 0)
            {

                std::cout << "\n\n\tYou are Logged in!";

                pos = static_cast<int>(infile.tellg()) - static_cast<int>(sizeof(new_account));
                logged_in = true;
                std::cout << "\n\n\t";
                system("pause");
                return;
            }
            else {
                std::cout << "\n\n\tWrong Password!";
                std::cout << "\n\n\t";
                system("pause");
                return;
            }
        }
    }
    if (flag == false)
    {
        std::cout << "\n\n\tSorry! Account number not found!";
        std::cout << "\n\n\t";
        system("pause");
        return;
    }
}

//*******************************************************
//                ADMIN LOGIN
//*******************************************************
void adminLogin(int &admin_pos , bool &is_logged_in) 
{
    //TAKING CREDENTIALS
    system("cls");
    char name[40] = {};
    char pass[20] = {};
    std::cout << "\n\n\t\tADMIN LOGIN";
    std::cout << "\n\n\tENTER THE USER NAME : "; 
    std::cin.getline(name, 40);
    std::cin.ignore(32767 , '\n');
    std::cout << "\n\n\tENTER THE PASSWORD : ";
    //std::cin.getline(pass, 20);
    getHiddenPass(pass);
    //std::cin.ignore(32767, '\n');
//Comparing with Data
    std::ifstream infile{ admin_accounts_path + admin_accounts_file_name , std::ios::binary | std::ios::in };
    if (!infile)
    {
        std::cout << "\n\tError! cannot open the file";
        return;
    }
    bool flag = false;
    adminAccount admin;
    while (infile.read((char*)&admin , sizeof(admin)))
    {
        if (strcmp(name, admin.getName()) == 0)
        {
            flag = true;
            if (strcmp(pass, admin.getPass()) == 0)
            {
                admin_pos = static_cast<int>(infile.tellg()) - static_cast<int>(sizeof(admin));
                is_logged_in = true;
                std::cout << "\n\n\tYOU ARE LOGGED IN AS " << getAccountTypeName(admin.getType());
                break;
            }
            else 
            {
                std::cout << "\n\n\tWRONG PASSWORD!";
            }
        }
    }
    if (!flag)
    {
        std::cout << "\n\n\tADMIN ACCOUNT NOT FOUND!";
    }
    std::cout << "\n\n\t";
    system("pause");
    std::cin.ignore(32767, '\n');
}

//*******************************************************
//                SHOW ALL ACCOUNT HOLDERS
//*******************************************************
void showAllAccounts()
{
    system("cls");
    std::cout << "\n--------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(3) <<"   "<< std::left << std::setw(20) << "ACCOUNT" << std::setw(40) << "NAME" << std::left << std::setw(10) << "CASH";
    std::cout << "\n" << std::left << std::setw(12) << "   NUMBER";
    std::cout << "\n--------------------------------------------------------------------------------\n";
    std::ifstream infile{ accounts_path + accounts_file_name , std::ios::binary | std::ios::in };
    if (!infile)
    {
        std::cout << "\n\tError! cannot open the file";
        return;
    }
    account new_account;
    while (infile.read((char*)&new_account, sizeof(new_account)))
    {
        std::cout << "\n   " << std::left << std::setw(20) << new_account.getAcno() << std::setw(40) << new_account.getName() << std::left << std::setw(10) << new_account.getBalance();
    }
    std::cout << "\n\n\t";
    system("pause");
}

void credits()
{
	//--- Printing Created By Muhammad Jamshaid
	{
		{
		destCoord.X = 12;
		destCoord.Y = 2;  // options - 1 becuase cursor coords starts from zero not from 1 
		SetConsoleCursorPosition(hStdout, destCoord); 
		WORD currentColor = GetColor(hStdout);
		SetConsoleTextAttribute(hStdout , FOREGROUND_GREEN | FOREGROUND_INTENSITY);   
		std::cout << std::setw(10) << std::left<< "Created By";
		std::cout << std::setw(4) << std::left<< " :  ";
		SetConsoleTextAttribute(hStdout , FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "MUHAMMAD JAMSHAID GHAFFAR";
		SetConsoleTextAttribute(hStdout , currentColor);
		}
		{
		destCoord.X = 12;
		destCoord.Y = 4;  // options - 1 becuase cursor coords starts from zero not from 1 
		SetConsoleCursorPosition(hStdout, destCoord);
		WORD currentColor = GetColor(hStdout);
		SetConsoleTextAttribute(hStdout , FOREGROUND_GREEN | FOREGROUND_INTENSITY);   
		std::cout << std::setw(10) << std::left<< "Version";
		std::cout << std::setw(4) << std::left<< " :  ";
		SetConsoleTextAttribute(hStdout , FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "2";
		SetConsoleTextAttribute(hStdout , currentColor);
		}
		}
	while(true)
	{
		char key = getch();
		switch(key){
			case KEY_ESC:
				return;
		}
	}
	
}

int main()
{
	//----- Setting Color For Console------------
	SetConsoleTextAttribute(hStdout , FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	menuSetUp();
    while (true)
    {

        system("cls");
        mkdir(accounts_path.c_str());
        mkdir(admin_accounts_path.c_str());
        mkdir(transaction_path.c_str());
        createOwnerAccount();
        int option = 0, pos = -1;
        bool logged_in = false;
        int admin_pos = -1;
        bool admin_is_logged_in = false;
        //-----Printing Bank Management System HEADER--------
        {
		destCoord.X = 12;
		destCoord.Y = 2;  // options - 1 becuase cursor coords starts from zero not from 1 
		SetConsoleCursorPosition(hStdout, destCoord); 
		WORD currentColor = GetColor(hStdout);
		SetConsoleTextAttribute(hStdout , FOREGROUND_GREEN | FOREGROUND_INTENSITY);   
		std::cout << "BANK MANAGEMENT SYSTEM";
		SetConsoleTextAttribute(hStdout , currentColor);
		}
        option = Menu::main.optionsLoop(hStdout , destCoord);

        system("CLS");
        switch (option)
        {
        	case 0:
        		return 0;
        		
        case 1:
            writeAccount();
            break;
        case 2:
            login(pos, logged_in);
            if (logged_in == true)
            {
                loggedIn(pos);
            }
            break;
        case 3:
            adminLogin(admin_pos, admin_is_logged_in);
            if (admin_is_logged_in == true)
            {

                adminLoggedIn(admin_pos);
            }
            break;
        case 4:
            showAllAccounts();
            break;
        case 5:
        	credits();
        default:
            break;
        }
    }

    
    
    
    return 0;
}


//-------------MenuSetUp--------------
void menuSetUp()
{
	
	COORD newCoords;
	
	//---- adminMenu
	newCoords = Menu::adminMenu.getCoords();
    newCoords.Y = 7;
    Menu::adminMenu.changeCoords(newCoords , Menu::adminMenu.getGapBtwOptions());
    //------------
    newCoords = Menu::loggedIn.getCoords();
    newCoords.Y = 7;
    Menu::loggedIn.changeCoords(newCoords , Menu::loggedIn.getGapBtwOptions());
    
}


