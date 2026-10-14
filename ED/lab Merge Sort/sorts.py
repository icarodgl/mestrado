from item import less, exch, compexch

def selection_sort(input_data):
    for i in range(len(input_data)):
        for j in range(i,0,-1):
            item = input_data[j]
            anterior = input_data[j-1]
            input_data[j],input_data[j-1] = compexch(item, anterior)
            if item == anterior:
                break

def merge_sort_classico(a, aux, lo, hi):
    if(hi <= lo):
        return
    mid = lo + (hi-lo) // 2 # Avoid overflow
    merge_sort_classico(a, aux, lo, mid)
    merge_sort_classico(a, aux, mid+1, hi)
    merge(a, aux, lo, mid, hi)


def merge(a, aux, lo, mid, hi):
    for k in range(lo, hi+1):
        aux[k] = a[k]
    i,j = lo, mid+1
    for k in range(lo, hi+1):
        if(i > mid):
            a[k] =  aux[j]
            j+=1
        elif j > hi:
            a[k] = aux[i]
            i+=1
        elif less(aux[j], aux[i]):
            a[k] = aux[j]
            j+=1
        else:
            a[k] = aux[i]
            i+=1
