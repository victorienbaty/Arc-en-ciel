import socket
import traceback
from threading import Thread
from tkinter import *

clients = {}


class Player:
    def __init__(self, name):
        self.__kill = 0
        self.__death = 0
        self.__name = name

    def get_kills(self):

        return self.__kill

    def get_deaths(self):

        return self.__death

    def get_name(self):

        return self.__name

    def set_kills(self):
        self.__kill = self.__kill + 1

    def set_death(self):
        self.__death = self.__death + 1


def main():
    start_server()


def start_server():
    host = "192.168.43.30"
    port = 8080

    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print("Socket created")

    try:
        soc.bind((host, port))
    except:
        print("Bind failed. Error : " + str(sys.exc_info()))
        sys.exit()

    soc.listen(5)  # queue up to 5 requests
    print("Socket now listening")

    # infinite loop- do not reset for every requests
    while True:
        connection, address = soc.accept()
        ip, port = str(address[0]), str(address[1])
        print("Connected with " + ip + ":" + port)

        try:
            player = Player(ip)
            clients[ip] = player
            Thread(target=client_thread, args=(connection, player)).start()
        except:
            print("Thread did not start.")
            traceback.print_exc()


def client_thread(connection, player, max_buffer_size=5120):
    is_active = True
    while is_active:
        try:
            client_input = receive_input(connection, max_buffer_size)
            if client_input == 'touche':

                ennemi = getVs(player.get_name())
                print(ennemi)
                print('Le joueur ' + player.get_name() + ' s\'est fait touché par un autre joueur.')
                player.set_death()
                print('Le joueur', player.get_name(), 'est mort', str(player.get_deaths()), 'fois.')
            else:
                pass
        except:
            pass


def receive_input(connection, max_buffer_size):
    client_input = connection.recv(max_buffer_size)
    client_input_size = sys.getsizeof(client_input)

    if client_input_size > max_buffer_size:
        print("The input size is greater than expected {}".format(client_input_size))

    try:
        decoded_input = str(client_input, 'utf-8')
    except:
        return ""

    if len(decoded_input.split(',')) > 0:
        if decoded_input.split(',')[0] == 'MSG':
            return decoded_input.split(',')[1]


def getVs(player):

    for client in clients:
        if client != player:
            return client


if __name__ == "__main__":
    main()
