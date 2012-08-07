.SUFFIXES:

# C++ source files:
%.oxx:: %.cpp $(DEP_WLD_COMPILE)
	@echo "CXX   $*.cpp"
	$(CXX) $(CXXFLAGS) -MD -MF "$@.dxx" -c "$(THIS_SRC_BASE_DIR)/$*.cpp" -o "$@"

# C source files:
%.o:: %.c $(DEP_WLD_COMPILE)
	@echo "CC    $*.c"
	$(CC) $(CFLAGS)    -MD -MF "$@.d"   -c "$(THIS_SRC_BASE_DIR)/$*.c"   -o "$@"

# missing headers on dependency problem - C++
%.hpp:
	@true

# missing headers on dependency problem - C
%.h:
	@true
