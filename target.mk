
clean:	
	for dir in ${SUBDIRS} ; do ( cd $$dir ; ${MAKE} clean ) ; done
	rm -f core *~ $(OBJS) *.d 

propper: clean
	for dir in ${SUBDIRS} ; do ( cd $$dir ; ${MAKE} propper ) ; done
	rm -f  $(EXECS) *.da *.bb *.bbg *.gcov

distclean: propper

# debug:
#	debug

coverage:
	gcov -f $(OBJS_MAIN_EXEC:.o=.cc)

tags:
	@echo "use make TAGS instead"

TAGS:
	rm -f TAGS
	etags -a -l c++ -o TAGS $(OBJS_MAIN_EXEC:.o=.cc)


tar: compress

compress:
	cd ..; ln -s $(NAME) $(FULLNAME); tar czfh $(FULLNAME).tar.gz $(FULLNAME); rm $(FULLNAME); cd -


# lib:	libfbmini.a fblib.o libtst.o
# 	  rm fblib.o
# 	  gcc -c fblib.c
# 	  $(AR) libfbmini.a fblib.o
# 	  $(RANLIB) libfbmini.a
# 	  gcc libtst.o -L. -lfbmini -static -o libtst
# 
# shared:	libtst.o
# 	  gcc -fPIC -c fblib.c 
# 	  gcc -shared -o libfbmini.so fblib.o
# 	  gcc libtst.o -L. -lfbmini -o slibtst
# 	  rm fblib.o

