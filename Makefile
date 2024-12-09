all:
	(cd timerLib; make install)
	(cd lcdLib; make install)
	(cd wakedemo; make)
	(cd toy; make)

doc:
	rm -rf doxygen_docs
	doxygen Doxyfile
clean:
	(cd timerLib; make clean)
	(cd lcdLib; make clean)
	(cd toy; make clean)
	(cd wakedemo; make clean)
	rm -rf lib h
	rm -rf doxygen_docs/*
