bin/Agenda: ./build/Agenda.o \
	./build/User.o \
	./build/Meeting.o \
	./build/Date.o \
	./build/Storage.o \
	./build/AgendaService.o \
	./build/AgendaUI.o
	g++-4.8	-std=c++11 $^ -o $@

build/%.o: src/%.cpp
	g++-4.8 -std=c++11 -I./include -c $^
	@mv ./*.o ./build
clean:
	@rm bin/Agenda
	@rm build/User.o
	@rm build/Date.o
	@rm build/Agenda.o
	@rm build/Meeting.o
	@rm build/Storage.o
	@rm build/AgendaService.o
	@rm build/AgendaUI.o
