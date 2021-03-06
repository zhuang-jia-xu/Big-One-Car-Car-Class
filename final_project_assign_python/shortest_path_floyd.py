import csv
import pandas
import numpy as np
import math

class shortest_path_floyd():
    n_of_node=0
    def __init__(self, file_path):
        # get how many nodes in the map
        reader=csv.reader(open(file_path))
        self.n_of_node=len(list(reader))+1
        #initialize the distance matrix
        self.distance=np.zeros((self.n_of_node+1,self.n_of_node+1),dtype=np.int)
        self.neighbor=np.zeros((self.n_of_node+1,5),dtype=np.int)#1 for up/2 for down/3 for left/4 for right
        self.next=np.zeros((self.n_of_node+1,self.n_of_node+1),dtype=np.int)
        self.treasure=list()
        for i in range(1,self.n_of_node+1):
            for j in range(1,self.n_of_node+1):
                if i!=j:
                    self.distance[i][j]=9999
        #read csv file
        csv_raw_data = pandas.read_csv(file_path).values
        self.initialize_maze_with_csv_raw_data(csv_raw_data)

    def initialize_maze_with_csv_raw_data(self, csv_raw_data):
        # process csv_raw_data:
        # for each csv_data_row,
        #     csv_data_row[0] is the node index;
        #     csv_data_row[i] is, if exists, the index of the neighboring node to the ith direction, with 1 <= i <= 4.
        flag=0
        for csv_data_row in csv_raw_data:
            n1=csv_data_row[0]
            cnt=0
            for i in range(1,5):
                if not math.isnan(csv_data_row[i]):
                    cnt+=1
                    n2=int(csv_data_row[i])
                    n1=int(n1)
                    self.neighbor[n1][i]=n2
                    self.distance[n1][n2]=csv_data_row[i+4]
                    self.next[n1][n2]=n2
                else:
                    n1=int(n1)
                    self.neighbor[n1][i]=0
            if cnt==1:
                #print(n1)
                self.treasure.append(n1)
        self.floyd()

    def floyd(self):
        # for i in range(1,self.n_of_node+1):
        #     for j in range(1,self.n_of_node+1):
        #         print(self.distance[i][j])
        #     print('\n')
        for k in range(1,self.n_of_node+1):
            for i in range(1,self.n_of_node+1):
                for j in range(1,self.n_of_node+1):
                    if self.distance[i][j]>self.distance[i][k]+self.distance[k][j]:
                        self.distance[i][j]=self.distance[i][k]+self.distance[k][j]
                        self.next[i][j]=self.next[i][k]
    def path_reconstruction(self,start,end):
        path=[start]
        nxt=start
        while nxt!=end:
            nxt=self.next[nxt][end]
            path.append(nxt)
        return path
    def remove_treasure(self,idx):
        self.treasure.remove(idx)
    def get_way(self,st,des):
        for i in range(1,5):
            if self.neighbor[st][i]==des:
                return i

