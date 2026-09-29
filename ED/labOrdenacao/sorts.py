from item import less, exch, compexch

def insertion_sort(input_data):
    pass

def selection_sort(input_data):
    for i in range(len(input_data)):
        for j in range(i,0,-1):
            item = input_data[j]
            anterior = input_data[j-1]
            input_data[j],input_data[j-1] = compexch(item, anterior)
        

def bubble_sort(input_data):
    pass

def shaker_sort(input_data):
    pass

A = 2
B = 3
A,B = compexch(A,B)
print(A,B)