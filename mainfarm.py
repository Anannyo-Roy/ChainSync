import mysql.connector
import sys

mydb = mysql.connector.connect(host='localhost',password='1234',user='root',database ='cs1')
if(mydb.is_connected):
    print("Connected")

mycursor = mydb.cursor()

class Dijkstra:
    def __init__(self, farmer_name1, farm_name1, closest_to1, dist1, capacity):
        
        mycursor.execute("""
                        CREATE TABLE IF NOT EXISTS farmer (
                          id INT AUTO_INCREMENT PRIMARY KEY,
                          farmer_name VARCHAR(255),
                          farm_name VARCHAR(255),
                          closest_to VARCHAR(255),
                          dist INT
                        )
                        """) 
                        
        mydb.commit()
        
        mycursor.execute("""
                        CREATE TABLE IF NOT EXISTS storage (
                          id INT AUTO_INCREMENT PRIMARY KEY,
                          name VARCHAR(255),
                          capacity INT,
                          closest_to VARCHAR(255),
                          dist INT
                        )
                        """)
                        
        mydb.connect()
        
        mycursor.execute("SELECT COUNT(*) FROM storage")
        count = mycursor.fetchone()[0]
        
        if count == 0:
            # Insert rows into the storage table
            sql = """
            INSERT INTO storage (name, closest_to, dist, capacity) VALUES 
            ('Storage1', 'Aarohan Transport Hub', 5, 300),
            ('Storage2', 'Bandra Distributors', 10, 250),
            ('Storage3', 'Columbia Center', 15, 400),
            ('Storage4', 'District Bus Stand', 20, 200),
            ('Storage5', 'Emporium Marg', 25, 350)
            """
            mycursor.execute(sql)
            mydb.commit()
        
        self.capacity = capacity
        
        mycursor.execute("delete from farmer")
        mydb.commit()
        # Prepare the SQL query with placeholders
        sql = "INSERT INTO farmer (farmer_name, farm_name, closest_to, dist) VALUES (%s, %s, %s, %s)"
        mycursor.execute(sql,(farmer_name1, farm_name1, closest_to1, dist1))
        mydb.commit()
        
        # Initialize the graph with all zeros
        self.graph = [
            [0, 8, 0, 11, 0, 0, 0, 0, 0],
            [4, 0, 8, 0, 0, 0, 0, 0, 0],
            [0, 3, 0, 2, 0, 0, 0, 0, 0],
            [0, 0, 4, 6, 0, 0, 0, 0, 0],
            [7, 0, 0, 0, 7, 0, 4, 0, 0],
            [0, 5, 6, 0, 0, 0, 0, 1, 9],
            [0, 0, 5, 0, 0, 0, 0, 0, 4],
            [0, 0, 0, 0, 6, 0, 0, 6, 0],
            [0, 0, 0, 0, 3, 0, 4, 0, 0],
        ]
        self.V = len(self.graph)
        self.places = ["Aarohan Transport Hub",
                     "Bandra Distributors",
                     "Columbia Center",
                     "District Bus Stand",
                     "Emporium Marg",
                     "Fishery",
                     "Green Packing House",
                     "High Court Street",
                     "Indo Grading Center"
                     ]

        self.storage_names = []  # List to store names of added storage locations
        self.farm_names = []  # List to store names of added farm locations
        
        mycursor.execute("SELECT farm_name FROM farmer")
        farm = mycursor.fetchone()[0]
        mycursor.execute("SELECT closest_to FROM farmer")
        cl = mycursor.fetchone()[0]
        mycursor.execute("SELECT dist FROM farmer")
        di = mycursor.fetchone()[0]
        self.add_node(cl,di,farm)
        
        mycursor.execute("SELECT s.name, s.closest_to, s.dist FROM storage s")
        storage_data = mycursor.fetchall()
        for storage_data in storage_data:
            storage_name, closest_to, dist = storage_data
            self.add_storage(closest_to, dist, storage_name)
            
        mydb.commit()

    def min_distance(self, dist, spt_set):
        # Find the vertex with the minimum distance value
        min_dist = sys.maxsize
        min_index = -1

        for v in range(self.V):
            if dist[v] < min_dist and not spt_set[v]:
                min_dist = dist[v]
                min_index = v

        return min_index

    def get_vertex_index(self, place):
        # Get the index of the vertex based on the place name
        return self.places.index(place)

    def add_node(self, closest_to, distance, name):
        # Increase the number of vertices
        self.V += 1
        # Add a new row and column to the graph
        for row in self.graph:
            row.append(0)
        self.graph.append([0] * self.V)
        # Set distance only from the new node to the closest vertex mentioned
        closest_index = self.get_vertex_index(closest_to)
        new_node_index = self.V - 1
        self.graph[new_node_index][closest_index] = distance
        # Make sure there are no existing paths from the closest vertex to the new node
        for i in range(self.V - 1):
            if self.graph[closest_index][i] > 0:
                self.graph[new_node_index][i] = 0
        # Store the name of the farm location
        self.farm_names.append(name)
        self.places.append(name)

    def add_storage(self, closest_to, distance, name):
        # Increase the number of vertices
        self.V += 1
        # Add a new row and column to the graph
        for row in self.graph:
            row.append(0)
        self.graph.append([0] * self.V)
        # Set distance only from the closest vertex to the new storage
        closest_index = self.get_vertex_index(closest_to)
        new_storage_index = self.V - 1
        self.graph[closest_index][new_storage_index] = distance
        # Make sure there are no existing paths from the new storage to the closest vertex
        for i in range(self.V - 1):
            if self.graph[i][closest_index] > 0:
                self.graph[i][new_storage_index] = 0
        # Store the name of the storage location
        self.storage_names.append(name)
        self.places.append(name)

    def print_path(self, parent, j):
        # Recursive function to print the path from source to j
        if parent[j] == -1:
            print(self.places[j], end=" ")
            return
        self.print_path(parent, parent[j])
        print("->", self.places[j], end=" ")
        
        if self.places[j] in self.storage_names:
            cap = self.get_storage_capacity(self.places[j])
            print("(Capacity of this storage :",cap,")")
            
            
    def get_path_string(self, parent, j):
        path = ""
        if parent[j] == -1:
            path += self.places[j]
        else:
            path += self.get_path_string(parent, parent[j]) + " -> " + self.places[j]
            if self.places[j] in self.storage_names:
                cap = self.get_storage_capacity(self.places[j])
                string_num = str(cap)
                path+=" (Capacity of this storage :"+string_num+")"
        return path

    
    def get_storage_capacity(self, storage_name):   
        mycursor.execute("SELECT capacity FROM storage WHERE name = %s", (storage_name,))
        capacity = mycursor.fetchone()[0]
        return capacity



    def dijkstra(self, src):
        src_index = self.get_vertex_index(src)
    
        # Initialize distance array with maximum values and source vertex with 0
        dist = [sys.maxsize] * self.V
        dist[src_index] = 0
        parent = [-1] * self.V  # Array to store the shortest path tree
    
        # Initialize set to keep track of vertices included in shortest path tree
        spt_set = [False] * self.V
    
        queue = []
        queue.append(src_index)
    
        while queue:
            u = queue.pop(0)
            spt_set[u] = True
    
            # Check if the current vertex is a storage location and has sufficient capacity
            if self.places[u] in self.storage_names:
                cap = self.get_storage_capacity(self.places[u])
                if cap >= self.capacity:
                    break
    
            # Update distance values of adjacent vertices
            for v in range(self.V):
                if (
                    self.graph[u][v] > 0
                    and not spt_set[v]
                    and dist[v] > dist[u] + self.graph[u][v]
                ):
                    dist[v] = dist[u] + self.graph[u][v]
                    parent[v] = u
                    queue.append(v)
    
        # Find the index of the closest storage that meets the capacity requirement
        closest_storage_index = None
        min_distance = sys.maxsize
        for i in range(self.V):
            if self.places[i] in self.storage_names:
                cap = self.get_storage_capacity(self.places[i])
                if cap >= self.capacity and dist[i] < min_distance:
                    min_distance = dist[i]
                    closest_storage_index = i
    
        if closest_storage_index is not None:
            # Print the shortest distance from source to closest storage
            print(f"Shortest distance from {src} to {self.places[closest_storage_index]}: {dist[closest_storage_index]}")
    
            # Print the path from source to closest storage
            print("Path:", end=" ")
            self.print_path(parent, closest_storage_index)
            print()
            
        if closest_storage_index is not None:
            # Print the shortest distance from source to closest storage
            path = self.get_path_string(parent, closest_storage_index)
            return path
        else:
            print("No storage with sufficient capacity found for", src)

# Create an instance of the Dijkstra class with the existing graph
g = Dijkstra('Zammmmsadasdasdm','farm29320938','Bandra Distributors', 10, 25)
print(g.dijkstra(g.farm_names[0]))
