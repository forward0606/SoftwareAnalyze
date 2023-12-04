a.out:
	gcc --coverage -g -O2 huff.c huffman.h
	mv a-huff.gcno huff.gcno
	mv a-huffman.gcno huffman.gcno
	
clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f a.out huffman.h.gch input.data.huff huff.c.gcov
test_gcov: a.out
	./a.out input.data
	mv a-huff.gcda  huff.gcda
	gcov huff.c
lcov: huff.c.gcov
	lcov -directory  .  --capture  --output-file  app.info
	genhtml app.info