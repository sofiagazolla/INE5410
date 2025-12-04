from time import sleep
from random import randint
from threading import Thread, Lock, Condition

def produtor():
  global buffer
  for i in range(10):
    sleep(randint(0,2))           # fica um tempo produzindo...
    item = 'item ' + str(i)
    with lock:
      while(len(buffer) == tam_buffer):
        print('>>> Buffer cheio. Produtor ira aguardar.')
        lugar_no_buffer.wait()    # aguarda que haja lugar no buffer
      buffer.append(item)
      print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
      item_no_buffer.notify()

def consumidor():
  global buffer
  for i in range(10):
    with lock:
      while(len(buffer) == 0):
        print('>>> Buffer vazio. Consumidor ira aguardar.')
        item_no_buffer.wait()   # aguarda que haja um item para consumir 
      item = buffer.pop(0)
      print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
      lugar_no_buffer.notify()
    sleep(randint(0,2))         # fica um tempo consumindo...

buffer = []
tam_buffer = 5
lock = Lock()
lugar_no_buffer = Condition(lock)
item_no_buffer = Condition(lock)

prod, cons = [], []

for i in range (2):
  # transformar isso daqui em um laço
  prod.append(Thread(target=produtor))
  cons.append(Thread(target=consumidor))
  prod[i].start()
  cons[i].start()
  
for i in range (2):
  prod[i].join()
  cons[i].join()

# versao que identifica qual produtor produziu e qual consumidor consumiu
''''
def produtor(id_produtor):
  global buffer
  for i in range(10):
    sleep(randint(0,2))
    item = f'item {i} (P{id_produtor})' # Identifica qual produtor criou
    with lock:
      while(len(buffer) == tam_buffer):
        print(f'>>> P{id_produtor}: Buffer cheio. Aguardando.')
        lugar_no_buffer.wait()
      buffer.append(item)
      print(f'P{id_produtor} produziu {item}. Buffer: {len(buffer)}')
      item_no_buffer.notify()

def consumidor(id_consumidor):
  global buffer
  for i in range(10):
    with lock:
      while(len(buffer) == 0):
        print(f'>>> C{id_consumidor}: Buffer vazio. Aguardando.')
        item_no_buffer.wait()
      item = buffer.pop(0)
      print(f'C{id_consumidor} consumiu {item}. Buffer: {len(buffer)}')
      lugar_no_buffer.notify()
    sleep(randint(0,2))

buffer = []
tam_buffer = 5
lock = Lock()
lugar_no_buffer = Condition(lock)
item_no_buffer = Condition(lock)

prod, cons = [], []

# Criação das threads com argumentos
for i in range(2):
  p = Thread(target=produtor, args=(i+1,))
  c = Thread(target=consumidor, args=(i+1,))
  prod.append(p)
  cons.append(c)
  p.start()
  c.start()

for p in prod: p.join()
for c in cons: c.join()
'''