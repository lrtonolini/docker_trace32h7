###############################################
# Khaled Jmal - Lauterbach Datentechnik 2008 #
##############################################

RM := rm -rf

C_SRCS += \
./arm-dcc.c \
./fileio.c \
./main.c \
./remote.c \

OBJS += \
./arm-dcc.o \
./fileio.o \
./main.o \
./remote.o \

DEPS += \
${addprefix ./, \
arm-dcc.d \
fileio.d \
main.d \
remote.d \
}

%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(CROSS_COMPILE)gcc -O0 -Wall -c -o$@ $<
	@$(CROSS_COMPILE)gcc -O0 -Wall -c -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	$(CROSS_COMPILE)gcc -MM -MG -P -w -O0 -Wall -c  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '
	
LIBS := -lpthread

all: t32server

t32server: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	@echo $(CROSS_COMPILE)gcc  -ot32server $(OBJS) $(LIBS)
	@$(CROSS_COMPILE)gcc  -ot32server $(OBJS) $(LIBS)
	@$(CROSS_COMPILE)strip t32server
	@rm -f *.o *.d
	@echo 'Finished building target: $@'
	@echo ' '
	
t32server.static: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	@echo $(CROSS_COMPILE)gcc -static -ot32server.static $(OBJS) $(LIBS)
	@$(CROSS_COMPILE)gcc -static -ot32server.static $(OBJS) $(LIBS)
	@$(CROSS_COMPILE)strip t32server.static
	@rm -f *.o *.d
	@echo 'Finished building target: $@'
	@echo ' '
	
	
clean:
	-$(RM) $(OBJS)$(DEPS) t32server*
	-@echo ' '