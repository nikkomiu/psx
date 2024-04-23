.PHONY: all clean $(EXE_TARGETS) $(ISO_TARGETS)

all: $(EXE_TARGETS) $(ISO_TARGETS)

$(EXE_TARGETS):
	$(MAKE) -C $@

$(ISO_TARGETS):
	$(MAKE) -C $@ iso

clean:
	@for dir in $(EXE_TARGETS) $(ISO_TARGETS); do \
		$(MAKE) -C $$dir clean; \
	done
