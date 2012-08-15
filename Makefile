# default profile
PROFILE:=release

# profile setup
ifeq ($(PROFILE),release)
TARGET:=release
else
ifeq ($(PROFILE),debug)
TARGET:=debug
else
$(error PROFILE must be set to 'release' or 'debug'; for example: make all PROFILE=debug)
endif
endif

.PHONY: all
all: control-sw rt-controller motherboard motherboard-package

.PHONY: rt-controller
rt-controller:
	$(MAKE) -C rt-controller/src $(TARGET)

.PHONY: control-sw
control-sw:
	$(MAKE) -C control-sw/src $(TARGET)

.PHONY: motherboard
motherboard:
	$(MAKE) -C motherboard/src $(TARGET)

.PHONY: motherboard-package
motherboard-package: uChaos.tar.gz

uChaos.tar.gz: $(shell find motherboard/uChaos -type f -follow | grep -v /gen/)
	tar czvhf uChaos.tar.gz -C motherboard --exclude=gen uChaos

.PHONY: clean
clean:
	$(MAKE) -C rt-controller/src clean
	$(MAKE) -C control-sw/src clean
	$(MAKE) -C motherboard/src clean
	rm -fv uChaos.tar.gz
