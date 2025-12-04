from time import sleep
from random import randint
from threading import Thread, Semaphore

def produtor():
  global buffer
  for i in range(10):
    sleep(randint(0,2))           # fica um tempo produzindo...
    item = 'item ' + str(i)
    # verifica se há lugar no buffer
    semaforo_prod.acquire() # bloqueia
    buffer.append(item)
    print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
    semaforo_cons.release() # libera

def consumidor():
  global buffer
  for i in range(10):
    # aguarda que haja um item para consumir
    semaforo_cons.acquire() # bloqueia
    item = buffer.pop(0)
    print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
    sleep(randint(0,2))         # fica um tempo consumindo...
    semaforo_prod.release() # libera

buffer = []
tam_buffer = 3

# cria semáforos
semaforo_prod = Semaphore(tam_buffer) # produtor
semaforo_cons = Semaphore(0) # consumidor

prod = Thread(target=produtor)
cons = Thread(target=consumidor)

prod.start()
cons.start()

prod.join()
cons.join()
