LIBDIR				=	lib
CFLAGS				=	-I$(LIBDIR)
OBJFILES			=	$(wildcard $(CURDIR)/*/*.o)
PETERSONLOCK_TEST	=	petersonlock-test
FILTERLOCK_TEST		=	filterlock-test
BAKERYLOCK_TEST		=	bakerylock-test

define run
	@echo ld $@
	@$(CC) $(LDFLAGS) -o $@ $^
	@./$@
	@$(RM) $@
endef

help:
	@echo availiable targets:
	@echo - $(PETERSONLOCK_TEST) -- run test programm for Peterson lock
	@echo - $(FILTERLOCK_TEST) -- run test programm for Filter lock
	@echo - $(BAKERYLOCK_TEST) -- run test programm for Bakery lock
	@echo - clean -- clean object files

$(PETERSONLOCK_TEST): $(LIBDIR)/petersonlock.o c2/$(PETERSONLOCK_TEST).o
	$(call run,$@)

$(FILTERLOCK_TEST): $(LIBDIR)/filterlock.o c2/$(FILTERLOCK_TEST).o
	$(call run,$@)

$(BAKERYLOCK_TEST): $(LIBDIR)/bakerylock.o c2/$(BAKERYLOCK_TEST).o
	$(call run,$@)

%.o: %.c
	@echo cc $@
	@$(CC) $< -c -o $@ $(CFLAGS)

clean: $(OBJFILES)
	$(foreach c, $(OBJFILES), @echo rm $(c) && $(RM) $(c))
