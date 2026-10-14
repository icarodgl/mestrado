import sys
import time
from sorts import *

sys.setrecursionlimit(2000)

def main():
    # reading the input data
    # example of time counting for reading the input data
    #start the timer
    start = time.time()
    input_data = []
    with open(sys.argv[1], 'r') as file:
        # Read each line in the file
        for line in file:
            x = int(line)
            input_data.append(x)


    # input_data = [9,7,6,5,4,4,3,2,8]
    print("Tamanho: ",len(input_data))
    sort(input_data)
    
    # stop the timer
    end = time.time()
    # calculate elapsed time
    print("Tempo: ",end - start)

    # for i in input_data:
    #     print(i)

    # file.close()        

def sort(a):
    aux = [None] * len(a)
    merge_sort_classico(a, aux, 0,len(a)-1)

main()