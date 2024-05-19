import hashlib


class MerkleTree:
    def __init__(self):
        self.root: Node | None = None
        self.leaves: list[Node] = []

    def addNode(self, data):
        node = Node(data)
        self.leaves.append(node)
        self.update()

    def update(self):
        if len(self.leaves) == 0:
            self.root = None
            return

        tree_copy = self.leaves
        while len(tree_copy) > 1:
            if len(tree_copy) % 2 == 1:
                tree_copy.append(Node(self.leaves[-1].hash))
            new_node_level = []
            for i in range(0, len(tree_copy), 2):
                new_node = Node(leftNode=tree_copy[i], rightNode=tree_copy[i + 1])
                new_node_level.append(new_node)
            tree_copy = new_node_level

        self.root = tree_copy[0]


class Node:
    def __init__(self, data=None, leftNode=None, rightNode=None):
        self.leftNode = leftNode
        self.rightNode = rightNode
        self.hash = self.hashNode(data)

    def hashNode(self, data):
        if data is not None:
            return hashlib.sha256(data.encode()).hexdigest()
        else:
            data2hash = self.leftNode.hash + self.rightNode.hash if (self.leftNode and self.rightNode) else ''
            return hashlib.sha256(data2hash.encode()).hexdigest()
