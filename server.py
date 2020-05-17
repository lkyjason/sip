import sys, socket
import threading, queue

q = queue.Queue()

def thread_func():
    while True:
        (sock, addr) = q.get()

        content = sock.recv(100)
        print("recv: " + content.decode("utf-8"))

        sent = sock.send(f"hello i am server".encode())
        sock.close()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("python3 server.py server_port")

    server_port = int(sys.argv[1])

    threads = []
    
    num_threads = 1
    max_listen_queue = 10

    for i in range(num_threads): 
        thread = threading.Thread(target=thread_func, args=())
        thread.start()
        threads.append(thread)

    listen_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listen_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listen_sock.bind(('', server_port)) 
    listen_sock.listen(max_listen_queue)

    while True:
        (client_sock, address) = listen_sock.accept()
        q.put((client_sock, address))


    print("exit main thread")