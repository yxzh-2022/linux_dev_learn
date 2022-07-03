A = a b c
B = $(foreach f,$(A),$(f).o)

C = a b c d/

D = $(filter %/,$(C))  #取出符合 xxx/ 格式值

E = $(filter-out %/,$(C)) ##取出不符合 xxx/ 格式值

files = $(wildcard *.c)

file2 = a.c b.c c.c d.c e.c 
file3 = $(wildcard $(file2))  #判断哪些文件真实存在

dep_files = $(patsubst %.c, %.d ,$(file2))  # 从列表中取出每一个值，如果符合，则替换
all: 
	@echo B = $(B)
	@echo D = $(D)
	@echo E = $(E)
	@echo files = $(files)
	@echo file3 = $(file3)
	@echo dep_files = $(dep_files)
