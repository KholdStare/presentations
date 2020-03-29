MD_SOURCES := $(abspath $(shell find . -iname "*.markdown" | grep -v './reveal'))

HTML_RESULTS := $(addsuffix .html,$(basename $(MD_SOURCES)))
PDF_RESULTS := $(addsuffix .pdf,$(basename $(MD_SOURCES)))

.PHONY: all pdf

all: $(HTML_RESULTS)
pdf: $(PDF_RESULTS)

%.html: %.markdown reveal.template Makefile
	pandoc --from=markdown+yaml_metadata_block+fenced_divs --tab-stop=3 --no-highlight -t revealjs --template reveal.template -o $@ $<

%.pdf: %.html
	decktape -p 100 reveal 'file://$<' $@

clean:
	-rm -f $(HTML_RESULTS)
