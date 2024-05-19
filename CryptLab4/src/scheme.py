from uuid import uuid4
import json
import hashlib
from datetime import datetime
from src.merkle import MerkleTree


class Transaction:
    def __init__(self, from_person, to_person, amount: int):
        self.id = str(uuid4())
        self.from_person = from_person
        self.to_person = to_person
        self.amount = amount

    def __repr__(self):
        return {
            "transaction_id": self.id,
            "from": self.from_person.id,
            "to": self.to_person.id,
            "amount": self.amount
        }


class Block:
    def __init__(self, transactions: list[Transaction], previous_block=None):
        self.previous_block_hash = previous_block.hash if previous_block else None
        self.timestamp = (datetime.utcnow()).timestamp()
        self.transactions = transactions
        self.nonce = 0
        self.merkleTree = MerkleTree()
        self.fillTree()
        self.hash = self.__hash__()
        print(f'Block with hash {self.hash} created')

    def __repr__(self):
        return {
            "previous_hash": self.previous_block_hash,
            "transactions": [transaction.__repr__() for transaction in self.transactions],
            "timestamp": self.timestamp,
            "nonce": self.nonce
        }

    def __hash__(self):
        data2hash = f"{self.previous_block_hash}{self.nonce}{self.merkleTree.root.hash}" if self.merkleTree.root else f"{self.previous_block_hash}{self.nonce}"
        return hashlib.sha256(data2hash.encode()).hexdigest()

    def fillTree(self):
        for transaction in self.transactions:
            self.merkleTree.addNode(json.dumps(transaction.__repr__()))

    def proof_of_work(self, DIFF=4):
        while True:
            if self.hash[:DIFF] == '0' * DIFF:
                return True
            self.nonce += 1
            self.hash = self.__hash__()

    def getBlockData(self, clients_data: dict):
        for transaction in self.transactions:
            for client in [transaction.to_person, transaction.from_person]:
                if client.name not in clients_data:
                    clients_data[client.name] = {"current": client.starting_balance,
                                                 "min": client.starting_balance,
                                                 "max": client.starting_balance}

            clients_data[transaction.from_person.name]['current'] -= transaction.amount
            clients_data[transaction.to_person.name]['current'] += transaction.amount
            clients_data[transaction.from_person.name]['min'] = min(clients_data[transaction.from_person.name]['min'],
                                                                    clients_data[transaction.from_person.name][
                                                                        'current'])
            clients_data[transaction.to_person.name]['max'] = max(clients_data[transaction.to_person.name]['max'],
                                                                  clients_data[transaction.to_person.name][
                                                                      'current'])
        return clients_data
