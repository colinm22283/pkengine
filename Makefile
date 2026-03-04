CARGO=cargo

PROJECTS+=pksweeper

.PHONY: pkengine
pkengine:
	cd pkengine && $(CARGO) test

.PHONY: build-%
build-%:
	cd $* && $(CARGO) build

.PHONY: run-%
run-%:
	cd $* && $(CARGO) run

