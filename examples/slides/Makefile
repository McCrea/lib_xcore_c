
products := \
  fpointers/fpointer0.c.build \
  fpointers/fpointer1.c.build \
  fpointers/fpointer2.c.build \
  fpointers/fpointer3.c.build \
  fpointers/fpointer4.c.build \
  fpointers/fpointer5.c.run \

.PHONY: all
all: $(products)

.PHONY: clean
clean:
	rm -f $(products)

%.c.build : %.c
	echo "$$ xcc -target=XCORE-200-EXPLORER $< -report" > $@.tmp
	-xcc -target=XCORE-200-EXPLORER $< -report >> $@.tmp 2>&1
	mv $@.tmp $@

.SECONDARY: %.xe 
%.xe : %.c
	xcc -target=XCORE-200-EXPLORER $< -o $@

%.c.run : %.xe
	echo "$$ xsim $<" > $@.tmp
	-xsim $< >> $@.tmp 2>&1
	mv $@.tmp $@


