import threading 
balance = 100
threadLock = threading.Lock()

def deposit():
    # Get lock to synchronize threads
    threadLock.acquire()
    global balance
    for i in range(1000000):
        balance = balance + 1
    # Free lock to release next thread
    threadLock.release()
        
def withdraw():
    # Get lock to synchronize threads
    threadLock.acquire()
    global balance
    for i in range(1000000):
        balance = balance - 1
    # Free lock to release next thread
    threadLock.release()

if __name__ == "__main__": 
    balance = int(input().strip())
    # creating thread 
    t1 = threading.Thread(target=deposit, args=()) 
    t2 = threading.Thread(target=withdraw, args=()) 
  
    # starting thread 1 
    t1.start() 
    # starting thread 2 
    t2.start() 
  
    # wait until thread 1 is completely executed 
    t1.join() 
    # wait until thread 2 is completely executed 
    t2.join() 
  
    # both threads completely executed 
    print("all done: balance = " + str(balance))