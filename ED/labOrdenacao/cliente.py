import sys
import time

from sorts import insertion_sort, selection_sort, bubble_sort, shaker_sort

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


    print(len(input_data))

    # stop the timer
    end = time.time()
    # calculate elapsed time
    print(end - start)

    # start the timer 
    # sortting arrays
    # stop the timer
    # printing sorted arrays

    file.close()        
    
main()