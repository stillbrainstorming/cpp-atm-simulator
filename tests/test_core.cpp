#define main atm_simulator_program_main
#include "../atm.cpp"
#undef main

#include <cassert>
#include <cmath>
#include <vector>

void testMobileValidation()
{
    assert(isValidMobileNumber("9087654321"));
    assert(!isValidMobileNumber("908765432"));
    assert(!isValidMobileNumber("90876543210"));
    assert(!isValidMobileNumber("90876543a1"));
}

void testAuthentication()
{
    Account tim;
    tim.setData(1234567, "Tim", 1111, 45000.90, "9087654321");

    Account alex;
    alex.setData(7654321, "Alex", 2222, 30000.00, "9876543210");

    std::vector<Account> accounts{tim, alex};
    AuthenticationService service;

    assert(service.authenticate(accounts, 1234567, 1111) != nullptr);
    assert(service.authenticate(accounts, 7654321, 2222) != nullptr);
    assert(service.authenticate(accounts, 1234567, 2222) == nullptr);
    assert(service.authenticate(accounts, 9999999, 1111) == nullptr);
}

void testWithdrawal()
{
    Account account;
    account.setData(1234567, "Tim", 1111, 1000.00, "9087654321");

    TransactionService service;

    assert(service.withdraw(account, 250.00) == WithdrawalResult::Success);
    assert(std::abs(account.getBalance() - 750.00) < 0.000001);
    assert(account.getTransactions().size() == 1);
    assert(account.getTransactions()[0].type == "Withdrawal");
    assert(account.getTransactions()[0].amount == 250.00);
    assert(account.getTransactions()[0].balanceAfter == 750.00);

    const double balanceAfterSuccess = account.getBalance();
    const size_t historyAfterSuccess = account.getTransactions().size();

    assert(service.withdraw(account, 0.00) == WithdrawalResult::InvalidAmount);
    assert(service.withdraw(account, -1.00) == WithdrawalResult::InvalidAmount);
    assert(service.withdraw(account, INFINITY) == WithdrawalResult::InvalidAmount);
    assert(service.withdraw(account, 1000.00) == WithdrawalResult::InsufficientBalance);

    assert(account.getBalance() == balanceAfterSuccess);
    assert(account.getTransactions().size() == historyAfterSuccess);
}

void testDeposit()
{
    Account account;
    account.setData(1234567, "Tim", 1111, 1000.00, "9087654321");

    TransactionService service;

    assert(service.deposit(account, 500.00) == DepositResult::Success);
    assert(std::abs(account.getBalance() - 1500.00) < 0.000001);
    assert(account.getTransactions().size() == 1);
    assert(account.getTransactions()[0].type == "Deposit");

    const double balanceAfterSuccess = account.getBalance();
    const size_t historyAfterSuccess = account.getTransactions().size();

    assert(service.deposit(account, 0.00) == DepositResult::InvalidAmount);
    assert(service.deposit(account, -1.00) == DepositResult::InvalidAmount);
    assert(service.deposit(account, -INFINITY) == DepositResult::InvalidAmount);

    assert(account.getBalance() == balanceAfterSuccess);
    assert(account.getTransactions().size() == historyAfterSuccess);
}

void testTransfers()
{
    Account source;
    source.setData(1234567, "Tim", 1111, 1000.00, "9087654321");

    Account target;
    target.setData(7654321, "Alex", 2222, 500.00, "9876543210");

    TransactionService service;

    assert(service.transfer(source, &target, 250.00) == TransferResult::Success);
    assert(std::abs(source.getBalance() - 750.00) < 0.000001);
    assert(std::abs(target.getBalance() - 750.00) < 0.000001);
    assert(source.getTransactions().size() == 1);
    assert(target.getTransactions().size() == 1);
    assert(source.getTransactions()[0].type == "Transfer Out");
    assert(target.getTransactions()[0].type == "Transfer In");
    assert(source.getTransactions()[0].relatedAccountNo == target.getAccountNo());
    assert(target.getTransactions()[0].relatedAccountNo == source.getAccountNo());

    const double sourceBalance = source.getBalance();
    const double targetBalance = target.getBalance();
    const size_t sourceHistory = source.getTransactions().size();
    const size_t targetHistory = target.getTransactions().size();

    assert(service.transfer(source, &source, 100.00) == TransferResult::SameAccount);
    assert(service.transfer(source, nullptr, 100.00) == TransferResult::AccountNotFound);
    assert(service.transfer(source, &target, 0.00) == TransferResult::InvalidAmount);
    assert(service.transfer(source, &target, -10.00) == TransferResult::InvalidAmount);
    assert(service.transfer(source, &target, 1000.00) == TransferResult::InsufficientBalance);

    assert(source.getBalance() == sourceBalance);
    assert(target.getBalance() == targetBalance);
    assert(source.getTransactions().size() == sourceHistory);
    assert(target.getTransactions().size() == targetHistory);
}

void testMobileUpdate()
{
    Account account;
    account.setData(1234567, "Tim", 1111, 1000.00, "9087654321");

    TransactionService service;

    assert(!service.updateMobile(account, "0000000000", "9876543210"));
    assert(account.getMobileNo() == "9087654321");

    assert(!service.updateMobile(account, "9087654321", "12345"));
    assert(account.getMobileNo() == "9087654321");

    assert(service.updateMobile(account, "9087654321", "9876543210"));
    assert(account.getMobileNo() == "9876543210");
}

int main()
{
    testMobileValidation();
    testAuthentication();
    testWithdrawal();
    testDeposit();
    testTransfers();
    testMobileUpdate();
    return 0;
}
