include defs.mk

all: $(LIBRARY) $(SUBPROJECTS)
	@echo "All targets successfully built"

$(SUBPROJECTS):
	@echo "Building $(patsubst %/,%,$(dir $@))"
	@$(MAKE) -C $@

$(LIBRARY): $(OBJECTS) | $(LIBDIR)
	@$(AR) rcs $@ $(OBJECTS)
	@echo "Built library successfully"

$(LIBDIR):
	@mkdir -p $@

$(OBJECTS):
	@echo "Building $(patsubst %/,%,$(dir $@)) library portion"
	@$(MAKE) -C $(dir $@) $(notdir $@)

clean:
	@echo "Removing all built binaries"
	@-rm -rf $(BUILDDIR) $(OBJECTS)
	@-for dir in $(SUBPROJECTS); do\
		$(MAKE) -C $$dir clean;\
	done

.PHONY: clean $(OBJECTS) $(SUBPROJECTS)
