ifdef BROWSER
	DOC_VIEWER=$(BROWSER)
else
	DOC_VIEWER=firefox
endif

DOXYFILE=./docs/doxygen/Doxyfile
DOXYGEN_INDEX_HTML=./docs/doxygen/html/index.html

default: view-doxygen

doxygen:
	doxygen $(DOXYFILE)

view-doxygen: doxygen
	$(DOC_VIEWER) $(DOXYGEN_INDEX_HTML) 1>/dev/null 2>&1 &

.PHONY: doxygen view-doxygen
