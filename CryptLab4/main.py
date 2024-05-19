from src.network import Network
from src.blockchain import Blockchain
from src.client import Client
from src.scheme import Transaction


network = Network()

alice = Client("Alice", balance=100)
bob = Client("Bob", balance=50)
eva = Client("Eva", balance=75)
network.add_client(alice)
network.add_client(bob)
network.add_client(eva)

transactions = [
    alice.makeTransaction(bob, 20),
    alice.makeTransaction(eva, 5),
    eva.makeTransaction(bob, 10),
    bob.makeTransaction(eva, 30),
    bob.makeTransaction(alice, 15),
    eva.makeTransaction(alice, 15)
]

print("Transactions anb blocks")
for transaction in transactions:
    network.send_transaction(transaction)

print("\nValidation of data in blockchain")
print("All is OK") if network.blockchain.validation() else print("Error.")

print("\nProof of work")
print("All is OK") if network.blockchain.chain[1].proof_of_work() else print("Error.")

print("\nData for block in blockchain")
print(network.blockchain.block_stats(block_position=1))

print("\nSave and load")
network.save_to_json()

print("\nValidation of data in loaded blockchain")
print("All is OK") if network.blockchain.validation() else print("Error.")

print("\nData for block in loaded blockchain")
print(network.blockchain.block_stats(block_position=2))