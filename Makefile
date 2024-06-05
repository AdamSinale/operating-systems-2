# Main makefile of the project.


# Define subdirectories
SUBDIRS = Question1 Question2 Question3 Question4 Question6

# Default target
all: $(SUBDIRS)

# Phony targets
.PHONY: all $(SUBDIRS) clean

# Target for each subdirectory
$(SUBDIRS):
	$(MAKE) -C $@

# Clean target
clean:
	for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir clean; \
	done
