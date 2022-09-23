import threading
import time
import math
import os
import random
import re
import sys

''' do not change this code '''

count = 0
def add():
  global count
  for x in range(100000):
       count = count + 1;
  time.sleep(1)
  
'''do not change the above code '''



if __name__ == '__main__':
    my_input = int(input().strip())

    ''' write your code here '''
  
    '''Hint : 1. Use add function to increment the count using  threads that will call add() method                           internally.  Do not use count variable directely.  
    //       2. Wait for all threads to finish.
    //       learn to create Threads in python : https://www.tutorialspoint.com/python/python_multithreading.htm
    '''
    no_of_threads = int(my_input / 100000)
    
    for index in range(no_of_threads):
        thread = threading.Thread(target=add)
        thread.start()
        thread.join()
            

'''do not change this code this is for output '''
print(count)