from item import less, exch, compexch

def insertion_sort(input_data):
    for i in range(len(input_data)-1):
        j = i+1
        while j > 0:
             compexch(input_data[j],input_data[j-1])
             j-=1
         
                   


def selection_sort(input_data):
    for i in range(len(input_data)):
        for j in range(i,0,-1):
            item = input_data[j]
            anterior = input_data[j-1]
            input_data[j],input_data[j-1] = compexch(item, anterior)
            if item == anterior:
                break
        

def bubble_sort(input_data):
    desordenado = True
    while desordenado:
        desordenado = False
        for i in range(len(input_data)-1):
            if less(input_data[i+1], input_data[i]):
                input_data[i+1] , input_data[i]  = exch(input_data[i+1], input_data[i])
                desordenado = True
        

def shaker_sort(input_data):
    desordenado = True

    while desordenado:
        desordenado = False
        for i in range(len(input_data)-1):
                    if less(input_data[i+1], input_data[i]):
                        input_data[i+1] , input_data[i]  = exch(input_data[i+1], input_data[i])
                        desordenado = True
        for i in range(len(input_data)-1,1,-1):
                    if less(input_data[i], input_data[i-1]):
                        input_data[i] , input_data[i-1]  = exch(input_data[i], input_data[i-1])
                        desordenado = True
