include = ../include

cflags = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -pthread

files = ilrd_uid dllist sorted_ll priority_q scheduler

headers = $(addsuffix .h, $(files))

objs = $(addsuffix .o, $(files))


all: $(headers) $(objs)
	$(CC) $(cflags) -I. watch_dog_api.c watch_dog.c $(objs) -o wd.out
	$(CC) $(cflags) -I. watch_dog_api.c app.c $(objs) -o app.out
	rm -f $(objs) 

%.o:
	$(CC) $(cflags) -I. $(include)/$*.c -c -o $@

%.h:
	ln -sf $(include)/$*.h $*.h

clean:
	rm -f $(headers) *.o *.out
