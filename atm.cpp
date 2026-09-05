#include <conio.h>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class AuthenticationService;

class Account
{
private:
    long int account_No;
    string name;
    int accessCode;
    double balance;
    string mobile_No;

    friend class AuthenticationService;

public:
    void setData(long int account_No_a, string name_a, int accessCode_a, double balance_a, string mobile_No_a)
    {
        account_No = account_No_a;
        name = name_a;
        accessCode = accessCode_a;
        balance = balance_a;
        mobile_No = mobile_No_a;
    }

    long int getAccountNo()
    {
        return account_No;
    }

    string getName()
    {
        return name;
    }

    double getBalance()
    {
        return balance;
    }

    string getMobileNo()
    {
        return mobile_No;
    }

    void updateMobile(const string& mobile_No_a)
    {
        mobile_No = mobile_No_a;
    }

    void deductBalance(double amount)
    {
        balance -= amount;
    }
};

enum class WithdrawalResult
{
    InvalidAmount,
    InsufficientBalance,
    Success
};

class AuthenticationService
{
public:
    bool authenticate(const Account& account, long int account_No_a, int accessCode_a)
    {
        return account.account_No == account_No_a && account.accessCode == accessCode_a;
    }
};

class TransactionService
{
public:
    WithdrawalResult withdraw(Account& account, int amount)
    {
        if (amount <= 0)
        {
            return WithdrawalResult::InvalidAmount;
        }

        if (amount > account.getBalance())
        {
            return WithdrawalResult::InsufficientBalance;
        }

        account.deductBalance(amount);
        return WithdrawalResult::Success;
    }

    bool updateMobile(Account& account, const string& oldMobileNo, const string& newMobileNo)
    {
        if (oldMobileNo != account.getMobileNo())
        {
            return false;
        }

        account.updateMobile(newMobileNo);
        return true;
    }
};

class ATMApplication
{
private:
    Account account;
    AuthenticationService authenticationService;
    TransactionService transactionService;

    void waitForInput()
    {
        _getch();
    }

    bool login()
    {
        long int enterAccountNo;
        int enterAccessCode;

        cout << endl << "**** Welcome to ATM ****" << endl;
        cout << endl << "Enter Your Account No: ";
        cin >> enterAccountNo;

        cout << endl << "Enter PIN: ";
        cin >> enterAccessCode;

        if (authenticationService.authenticate(account, enterAccountNo, enterAccessCode))
        {
            return true;
        }

        cout << endl << "Invalid Account No. or PIN.";
        waitForInput();
        return false;
    }

    void showBalance()
    {
        cout << endl << "Your Bank Balance: " << account.getBalance();
        waitForInput();
    }

    void withdrawCash()
    {
        int amount = 0;

        cout << endl << "Enter the Amount: ";
        cin >> amount;

        WithdrawalResult result = transactionService.withdraw(account, amount);

        if (result == WithdrawalResult::Success)
        {
            cout << endl << "Please Collect Your Cash";
            cout << endl << "Available Balance: " << account.getBalance();
        }
        else if (result == WithdrawalResult::InvalidAmount)
        {
            cout << endl << "Invalid Withdrawal Amount";
        }
        else
        {
            cout << endl << "Invalid Input or Insufficient Balance";
        }

        waitForInput();
    }

    void showUserDetails()
    {
        cout << endl << "*** User Details ***";
        cout << endl << "Account No : " << account.getAccountNo();
        cout << endl << "Name       : " << account.getName();
        cout << endl << "Balance    : " << account.getBalance();
        cout << endl << "Mobile No  : " << account.getMobileNo();
        waitForInput();
    }

    void updateMobileNo()
    {
        string oldMobileNo, newMobileNo;

        cout << endl << "Enter Old Mobile No.: ";
        cin >> oldMobileNo;
        cout << endl << "Enter New Mobile No.: ";
        cin >> newMobileNo;

        if (transactionService.updateMobile(account, oldMobileNo, newMobileNo))
        {
            cout << endl << "Successfully Updated Mobile No.";
        }
        else
        {
            cout << endl << "Incorrect Old Mobile No.";
        }

        waitForInput();
    }

    void runSession()
    {
        int choice = 0;

        do
        {
            system("cls");

            cout << endl << "**** Welcome to ATM ****" << endl;
            cout << endl << "Select an Option:";
            cout << endl << "1. Check Balance";
            cout << endl << "2. Cash Withdraw";
            cout << endl << "3. Show User Details";
            cout << endl << "4. Update Mobile No.";
            cout << endl << "5. Exit" << endl;
            cin >> choice;

            switch (choice)
            {
            case 1:
                showBalance();
                break;

            case 2:
                withdrawCash();
                break;

            case 3:
                showUserDetails();
                break;

            case 4:
                updateMobileNo();
                break;

            case 5:
                exit(0);

            default:
                cout << endl << "Invalid Input. Please Enter a Valid Option.";
            }

        } while (1);
    }

public:
    void initialize()
    {
        account.setData(1234567, "Tim", 1111, 45000.90, "9087654321");
    }

    void run()
    {
        system("cls");
        initialize();

        do
        {
            system("cls");

            if (login())
            {
                runSession();
            }

        } while (1);
    }
};

int main()
{
    ATMApplication application;
    application.run();

    return 0;
}
