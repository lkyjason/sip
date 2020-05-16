import sys, socket
import threading

def thread_func(server_ip, server_port, id):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((server_ip, server_port))

    sent = s.send(f"hello i am {id}\0".encode())
    print(f"thread {id} sent {sent}")

    content = s.recv(100)
    print("recv: " + content.decode("utf-8"))


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("python3 client.py server_ip server_port")

    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])

    threads = []
    
    num_threads = 3

    for i in range(num_threads):
        thread = threading.Thread(target=thread_func, args=(server_ip, server_port, i))
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join()

    print("exit main thread")