from src.scheme import Block, Transaction
from src.blockchain import Blockchain
from src.client import Client
import json


class Network:
    def __init__(self):
        self.blockchain = Blockchain()
        self.clients: list[Client] = []

    def add_client(self, client: Client):
        client.network = self
        self.clients.append(client)

    def send_transaction(self, transaction: Transaction):
        client = next((client for client in self.clients if client == transaction.to_person), None)
        if not client:
            print(f'Receiver not found')
            return
        print(f'Transaction from {transaction.from_person.name} '
              f'to {transaction.to_person.name}: {transaction.amount}')
        self.blockchain.transaction_process(transaction)
        client.receiveTransaction(transaction.amount)

    def save_to_json(self, default_path='blockchain.json'):
        print('Saving to json')
        with open(default_path, 'w') as f:
            json.dump([block.__repr__() for block in self.blockchain.chain], f, indent=4)

    def load_from_json(self, default_path='blockchain.json'):
        print('Loading fom json')
        with open(default_path, 'r') as f:
            data = json.load(f)
            self.blockchain = Blockchain()
            for block_js in data:
                transactions = []
                for tr in block_js['transactions']:
                    sender = next((client for client in self.clients if client.id == tr.from_person), None)
                    receiver = next((client for client in self.clients if client.id == tr.to_person), None)
                    transactions.append(Transaction(sender, receiver, tr['amount']))
                block = Block(transactions, block_js['previous_hash'])
                block.nonce = block_js['nonce']
                block.hash = block.__hash__()
                self.blockchain.chain.append(block)


