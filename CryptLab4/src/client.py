from uuid import uuid4
from src.scheme import Transaction


class Client:
    def __init__(self, name: str, balance=50):
        self.id = str(uuid4())
        self.name = name
        self.balance = balance
        self.starting_balance = balance

    def makeTransaction(self, to_person, amount: int):
        if amount > self.balance:
            print(f"Cannot make transaction - not enough balance")
            return
        if to_person == self:
            print('Cannot make transaction to self')
            return
        if amount < 0:
            print(f"Cannot make negative transaction")
            return
        transaction = Transaction(self, to_person, amount)
        return transaction

    def receiveTransaction(self, amount: int):
        self.balance += amount
