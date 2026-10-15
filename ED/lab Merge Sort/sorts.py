from item import less, exch, compexch

CUTOFF = 12

def insertion_sort(input_data, lo, hi):
    for i in range(lo + 1, hi + 1):
        item = input_data[i]
        j = i - 1
        while j >= lo and input_data[j] > item:
            input_data[j + 1] = input_data[j]
            j -= 1
        input_data[j + 1] = item


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

def merge_sort_classico(a, aux, lo, hi):
    if(hi <= lo):
        return
    mid = lo + (hi-lo) // 2 # Avoid overflow
    merge_sort_classico(a, aux, lo, mid)
    merge_sort_classico(a, aux, mid+1, hi)
    merge(a, aux, lo, mid, hi)


def merge_sort_cutoff(a, aux, lo, hi):
    if(hi <= lo + CUTOFF - 1):
        insertion_sort(a,lo,hi)
        return
    mid = lo + (hi-lo) // 2 # Avoid overflow
    merge_sort_cutoff(a, aux, lo, mid)
    merge_sort_cutoff(a, aux, mid+1, hi)
    merge(a, aux, lo, mid, hi)

def merge_sort_skip_merge(a, aux, lo, hi):
    if(hi <= lo):
        return
    
    mid = lo + (hi-lo) // 2
    merge_sort_skip_merge(a, aux, lo, mid)
    merge_sort_skip_merge(a, aux, mid+1, hi)
    if (not less(a[mid+1], a[mid])):
        return
    merge(a, aux, lo, mid, hi)


def merge_sort_cutof_skip_merge(a, aux, lo, hi):
    if(hi <= lo + CUTOFF - 1):
        insertion_sort(a,lo,hi)
        return
    mid = lo + (hi-lo) // 2
    merge_sort_cutof_skip_merge(a, aux, lo, mid)
    merge_sort_cutof_skip_merge(a, aux, mid+1, hi)
    if (not less(a[mid+1], a[mid])):
        return
    merge(a, aux, lo, mid, hi)


def MIN(X, Y):
    if X < Y:
        return X
    else:
        return Y

    
def merge_sort_bottomup(a, lo, hi):
    N = (hi - lo) + 1
    y = N - 1
    aux = [None] * N
    sz = 1
    while sz < N:
        for aux_lo in range(0, N - sz, 2 * sz):
            x = aux_lo + 2 * sz - 1
            merge(a, aux, aux_lo, aux_lo + sz - 1, MIN(x, y))
        sz = 2 * sz


def merge_sort_bottomup_cutof(a, lo, hi):
    N = (hi - lo) + 1
    if N <= 1:
        return
    y = N - 1
    aux = [None] * N

    # insertion em blocos de CUTOFF
    for inicio in range(0, N, CUTOFF):
        fim = MIN(inicio + CUTOFF - 1, y)
        insertion_sort(a, lo + inicio, lo + fim)

    # merges a partir de CUTOFF
    sz = CUTOFF
    while sz < N:
        for aux_lo in range(0, N - sz, 2 * sz):
            x = aux_lo + 2 * sz - 1
            merge(a, aux, aux_lo, aux_lo + sz - 1, MIN(x, y))
        sz *= 2


def merge_sort_bottomup_cutof_merge_skip(a, lo, hi):
    N = (hi - lo) + 1
    if N <= 1:
        return
    y = N - 1
    aux = [None] * N

    # insertion em blocos de CUTOFF
    for inicio in range(0, N, CUTOFF):
        fim = MIN(inicio + CUTOFF - 1, y)
        insertion_sort(a, lo + inicio, lo + fim)

    # merges a partir de CUTOFF
    sz = CUTOFF
    while sz < N:
        for aux_lo in range(0, N - sz, 2 * sz):
            x = aux_lo + 2 * sz - 1

            fim_esq  = lo + aux_lo + sz - 1
            ini_dir  = lo + aux_lo + sz
            if a[fim_esq] <= a[ini_dir]:
                continue
            
            merge(a, aux, aux_lo, aux_lo + sz - 1, MIN(x, y))
        sz *= 2