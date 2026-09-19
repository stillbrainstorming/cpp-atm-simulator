#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>

#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;

class AuthenticationService;

struct Transaction
{
    string type;
    double amount;
    long int relatedAccountNo;
    double balanceAfter;
};

enum class WithdrawalResult
{
    InvalidAmount,
    InsufficientBalance,
    Success
};

enum class DepositResult
{
    InvalidAmount,
    Success
};

enum class TransferResult
{
    InvalidAmount,
    SameAccount,
    AccountNotFound,
    InsufficientBalance,
    Success
};

bool isValidMobileNumber(const string& mobileNo)
{
    if (mobileNo.size() != 10)
    {
        return false;
    }

    for (char digit : mobileNo)
    {
        if (digit < '0' || digit > '9')
        {
            return false;
        }
    }

    return true;
}

class Account
{
private:
    long int account_No;
    string name;
    int accessCode;
    double balance;
    string mobile_No;
    vector<Transaction> transactions;

    friend class AuthenticationService;

public:
    void setData(long int account_No_a, string name_a, int accessCode_a, double balance_a, string mobile_No_a)
    {
        account_No = account_No_a;
        name = name_a;
        accessCode = accessCode_a;
        balance = balance_a;
        mobile_No = mobile_No_a;
        transactions.clear();
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

    const vector<Transaction>& getTransactions() const
    {
        return transactions;
    }

    void updateMobile(const string& mobile_No_a)
    {
        mobile_No = mobile_No_a;
    }

    void deductBalance(double amount)
    {
        balance -= amount;
    }

    void addBalance(double amount)
    {
        balance += amount;
    }

    void recordTransaction(const string& type, double amount, long int relatedAccountNo = 0)
    {
        transactions.push_back({type, amount, relatedAccountNo, balance});
    }

    void restoreState(double balance_a, const string& mobile_No_a, const vector<Transaction>& transactions_a)
    {
        balance = balance_a;
        mobile_No = mobile_No_a;
        transactions = transactions_a;
    }
};

class AuthenticationService
{
public:
    Account* authenticate(vector<Account>& accounts, long int account_No_a, int accessCode_a)
    {
        for (auto& account : accounts)
        {
            if (account.account_No == account_No_a && account.accessCode == accessCode_a)
            {
                return &account;
            }
        }

        return nullptr;
    }
};

class TransactionService
{
public:
    WithdrawalResult withdraw(Account& account, double amount)
    {
        if (!isfinite(amount) || amount <= 0)
        {
            return WithdrawalResult::InvalidAmount;
        }

        if (amount > account.getBalance())
        {
            return WithdrawalResult::InsufficientBalance;
        }

        account.deductBalance(amount);
        account.recordTransaction("Withdrawal", amount);
        return WithdrawalResult::Success;
    }

    DepositResult deposit(Account& account, double amount)
    {
        if (!isfinite(amount) || amount <= 0)
        {
            return DepositResult::InvalidAmount;
        }

        account.addBalance(amount);
        account.recordTransaction("Deposit", amount);
        return DepositResult::Success;
    }

    TransferResult transfer(Account& source, Account* target, double amount)
    {
        if (!isfinite(amount) || amount <= 0)
        {
            return TransferResult::InvalidAmount;
        }

        if (target == nullptr)
        {
            return TransferResult::AccountNotFound;
        }

        if (source.getAccountNo() == target->getAccountNo())
        {
            return TransferResult::SameAccount;
        }

        if (amount > source.getBalance())
        {
            return TransferResult::InsufficientBalance;
        }

        source.deductBalance(amount);
        target->addBalance(amount);
        source.recordTransaction("Transfer Out", amount, target->getAccountNo());
        target->recordTransaction("Transfer In", amount, source.getAccountNo());
        return TransferResult::Success;
    }

    bool updateMobile(Account& account, const string& oldMobileNo, const string& newMobileNo)
    {
        if (oldMobileNo != account.getMobileNo() || !isValidMobileNumber(newMobileNo))
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
    static const int maxLoginAttempts = 3;
    static constexpr const char* storagePath = "atm_data.txt";
    vector<Account> accounts;
    Account* activeAccount = nullptr;
    AuthenticationService authenticationService;
    TransactionService transactionService;

    Account* findAccount(long int accountNo)
    {
        for (auto& account : accounts)
        {
            if (account.getAccountNo() == accountNo)
            {
                return &account;
            }
        }

        return nullptr;
    }

    void loadState()
    {
        ifstream file(storagePath);
        if (!file.is_open())
        {
            return;
        }

        string line;
        while (getline(file, line))
        {
            if (line.empty())
            {
                continue;
            }

            try
            {
                stringstream stream(line);
                string kind;
                getline(stream, kind, '|');

                if (kind == "A")
                {
                    string accountNoText, name, balanceText, mobileNo;
                    getline(stream, accountNoText, '|');
                    getline(stream, name, '|');
                    getline(stream, balanceText, '|');
                    getline(stream, mobileNo, '|');

                    long int accountNo = stol(accountNoText);
                    double balance = stod(balanceText);
                    Account* account = findAccount(accountNo);
                    if (account != nullptr)
                    {
                        account->restoreState(balance, mobileNo, {});
                    }
                }
                else if (kind == "T")
                {
                    string accountNoText, type, amountText, relatedText, balanceText;
                    getline(stream, accountNoText, '|');
                    getline(stream, type, '|');
                    getline(stream, amountText, '|');
                    getline(stream, relatedText, '|');
                    getline(stream, balanceText, '|');

                    long int accountNo = stol(accountNoText);
                    Account* account = findAccount(accountNo);
                    if (account != nullptr)
                    {
                        Transaction transaction{type, stod(amountText), stol(relatedText), stod(balanceText)};
                        vector<Transaction> transactions = account->getTransactions();
                        transactions.push_back(transaction);
                        account->restoreState(account->getBalance(), account->getMobileNo(), transactions);
                    }
                }
            }
            catch (const exception&)
            {
            }
        }
    }

    void saveState()
    {
        ofstream file(storagePath, ios::trunc);
        if (!file.is_open())
        {
            return;
        }

        file << fixed << setprecision(2);
        for (const auto& account : accounts)
        {
            file << "A|" << account.getAccountNo() << "|"
                 << account.getName() << "|"
                 << account.getBalance() << "|"
                 << account.getMobileNo() << "|\n";

            for (const auto& transaction : account.getTransactions())
            {
                file << "T|" << account.getAccountNo() << "|"
                     << transaction.type << "|"
                     << transaction.amount << "|"
                     << transaction.relatedAccountNo << "|"
                     << transaction.balanceAfter << "|\n";
            }
        }
    }

    int readPin()
    {
#ifdef _WIN32
        string pin;
        char digit;
        cout << endl << "Enter PIN: ";
        while ((digit = static_cast<char>(_getch())) != '\r')
        {
            if (digit >= '0' && digit <= '9' && pin.size() < 4)
            {
                pin += digit;
                cout << '*';
            }
        }
        cout << endl;
        if (pin.size() != 4)
        {
            return -1;
        }
        return stoi(pin);
#else
        string pin;
        cout << endl << "Enter PIN: ";
        cin >> pin;
        if (pin.size() != 4 || !isValidMobileNumber("000000" + pin))
        {
            return -1;
        }
        return stoi(pin);
#endif
    }

    bool readLongInput(const string& prompt, long int& value)
    {
        cout << endl << prompt;
        if (cin >> value)
        {
            return true;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl << "Invalid numeric input.";
        return false;
    }

    bool readAmount(const string& prompt, double& amount)
    {
        cout << endl << prompt;
        if (cin >> amount && isfinite(amount))
        {
            return true;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl << "Invalid numeric input.";
        return false;
    }

    void waitForInput()
    {
#ifdef _WIN32
        _getch();
#else
        cin.ignore();
        cin.get();
#endif
    }

    bool login()
    {
        long int enterAccountNo;
        int failedAttempts = 0;

        cout << endl << "**** Welcome to ATM ****" << endl;

        while (failedAttempts < maxLoginAttempts)
        {
            if (!readLongInput("Enter Your Account No: ", enterAccountNo))
            {
                continue;
            }

            int enterAccessCode = readPin();
            Account* authenticatedAccount = authenticationService.authenticate(accounts, enterAccountNo, enterAccessCode);

            if (authenticatedAccount != nullptr)
            {
                activeAccount = authenticatedAccount;
                return true;
            }

            ++failedAttempts;
            cout << endl << "Invalid Account No. or PIN.";

            if (failedAttempts < maxLoginAttempts)
            {
                cout << endl << "Attempts remaining: " << maxLoginAttempts - failedAttempts;
            }
        }

        cout << endl << "Maximum login attempts exceeded.";
        waitForInput();
        return false;
    }

    void showBalance()
    {
        cout << fixed << setprecision(2);
        cout << endl << "Your Bank Balance: " << activeAccount->getBalance();
        waitForInput();
    }

    void showTransactionHistory()
    {
        const auto& transactions = activeAccount->getTransactions();
        cout << fixed << setprecision(2);
        cout << endl << "*** Transaction History ***";

        if (transactions.empty())
        {
            cout << endl << "No transactions recorded.";
            waitForInput();
            return;
        }

        for (size_t i = 0; i < transactions.size(); ++i)
        {
            const auto& transaction = transactions[i];
            cout << endl << i + 1 << ". " << transaction.type
                 << " | Amount: " << transaction.amount
                 << " | Balance: " << transaction.balanceAfter;

            if (transaction.relatedAccountNo != 0)
            {
                cout << " | Account: " << transaction.relatedAccountNo;
            }
        }

        waitForInput();
    }

    void withdrawCash()
    {
        double amount = 0;

        if (!readAmount("Enter the Amount: ", amount))
        {
            waitForInput();
            return;
        }

        WithdrawalResult result = transactionService.withdraw(*activeAccount, amount);

        if (result == WithdrawalResult::Success)
        {
            saveState();
            cout << endl << "Please Collect Your Cash";
            cout << fixed << setprecision(2);
            cout << endl << "Available Balance: " << activeAccount->getBalance();
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

    void depositCash()
    {
        double amount = 0;

        if (!readAmount("Enter the Amount: ", amount))
        {
            waitForInput();
            return;
        }

        DepositResult result = transactionService.deposit(*activeAccount, amount);

        if (result == DepositResult::Success)
        {
            saveState();
            cout << endl << "Cash deposited successfully.";
            cout << fixed << setprecision(2);
            cout << endl << "Available Balance: " << activeAccount->getBalance();
        }
        else
        {
            cout << endl << "Invalid Deposit Amount";
        }

        waitForInput();
    }

    void transferFunds()
    {
        long int targetAccountNo;
        double amount = 0;

        if (!readLongInput("Enter Target Account No.: ", targetAccountNo))
        {
            waitForInput();
            return;
        }

        if (!readAmount("Enter Transfer Amount: ", amount))
        {
            waitForInput();
            return;
        }

        Account* targetAccount = findAccount(targetAccountNo);
        TransferResult result = transactionService.transfer(*activeAccount, targetAccount, amount);

        if (result == TransferResult::Success)
        {
            saveState();
            cout << endl << "Transfer completed successfully.";
            cout << fixed << setprecision(2);
            cout << endl << "Available Balance: " << activeAccount->getBalance();
        }
        else if (result == TransferResult::InvalidAmount)
        {
            cout << endl << "Invalid Transfer Amount";
        }
        else if (result == TransferResult::SameAccount)
        {
            cout << endl << "Source and target accounts must be different.";
        }
        else if (result == TransferResult::AccountNotFound)
        {
            cout << endl << "Target account not found.";
        }
        else
        {
            cout << endl << "Insufficient Balance";
        }

        waitForInput();
    }

    void showUserDetails()
    {
        cout << fixed << setprecision(2);
        cout << endl << "*** User Details ***";
        cout << endl << "Account No : " << activeAccount->getAccountNo();
        cout << endl << "Name       : " << activeAccount->getName();
        cout << endl << "Balance    : " << activeAccount->getBalance();
        cout << endl << "Mobile No  : " << activeAccount->getMobileNo();
        waitForInput();
    }

    void updateMobileNo()
    {
        string oldMobileNo, newMobileNo;

        cout << endl << "Enter Old Mobile No.: ";
        cin >> oldMobileNo;
        cout << endl << "Enter New Mobile No.: ";
        cin >> newMobileNo;

        if (transactionService.updateMobile(*activeAccount, oldMobileNo, newMobileNo))
        {
            saveState();
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
#ifdef _WIN32
            system("cls");
#endif

            cout << endl << "**** Welcome to ATM ****" << endl;
            cout << endl << "Select an Option:";
            cout << endl << "1. Check Balance";
            cout << endl << "2. Cash Withdraw";
            cout << endl << "3. Deposit Cash";
            cout << endl << "4. Transfer Funds";
            cout << endl << "5. Show User Details";
            cout << endl << "6. Update Mobile No.";
            cout << endl << "7. Transaction History";
            cout << endl << "8. Exit" << endl;
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << endl << "Invalid Input. Please Enter a number from 1 to 8.";
                waitForInput();
                continue;
            }

            switch (choice)
            {
            case 1:
                showBalance();
                break;

            case 2:
                withdrawCash();
                break;

            case 3:
                depositCash();
                break;

            case 4:
                transferFunds();
                break;

            case 5:
                showUserDetails();
                break;

            case 6:
                updateMobileNo();
                break;

            case 7:
                showTransactionHistory();
                break;

            case 8:
                exit(0);

            default:
                cout << endl << "Invalid Input. Please Enter a Valid Option.";
            }

        } while (1);
    }

public:
    void initialize()
    {
        accounts.clear();

        Account firstAccount;
        firstAccount.setData(1234567, "Tim", 1111, 45000.90, "9087654321");
        accounts.push_back(firstAccount);

        Account secondAccount;
        secondAccount.setData(7654321, "Alex", 2222, 30000.00, "9876543210");
        accounts.push_back(secondAccount);

        loadState();
    }

    void run()
    {
#ifdef _WIN32
        system("cls");
#endif
        initialize();

        do
        {
#ifdef _WIN32
            system("cls");
#endif

            activeAccount = nullptr;
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
