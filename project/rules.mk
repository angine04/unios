# path to project mk files
PROJMK_PREFIX ?=

# general rules
all: $(IMAGE_FILE) $(KERNEL_DEBUG_FILE) $(TOOLS_EXECUTABLE)
.PHONY: all

clean:
	@\
	if [ ! -d $(OBJDIR) ]; then exit; fi;		\
	echo -ne "[PROC] clean-up all stuffs\r";	\
	if rm -rf $(OBJDIR) 2> /dev/null; then						\
		echo -e "\e[1K\r\e[32m[DONE]\e[0m clean-up all stuffs";	\
	else														\
		echo -e "\e[1K\r\e[31m[FAIL]\e[0m clean-up all stuffs";	\
	fi
.PHONY: clean

# run & debug rules
run: $(IMAGE_FILE)
	@$(QEMU) $(QEMU_FLAGS) -drive file=$<,format=raw
.PHONY: run

debug: $(IMAGE_FILE) $(KERNEL_DEBUG_FILE)
	@$(QEMU) $(QEMU_FLAGS) -drive file=$<,format=raw -s -S
.PHONY: debug

monitor:
	@\
	dbg="$(KERNEL_DEBUG_FILE)";							\
	if [ -e "$${dbg}" ]; then 							\
		$(GDB) $(GDB_FLAGS)								\
			-ex 'layout-sac'							\
			-ex "file $${dbg}"							\
			-ex 'b _start'								\
			-ex 'c';									\
	else												\
		echo -e "\e[31m[FAIL]\e[0m missing debug file";	\
	fi
.PHONY: monitor

monitor-real: $(GDB_REALMODE_XML)
	@\
	dbg="$(KERNEL_DEBUG_FILE)";							\
	if [ -e "$${dbg}" ]; then 							\
		$(GDB) $(GDB_FLAGS)								\
			-ex 'layout-rac'							\
			-ex "file $${dbg}"							\
			-ex 'set tdesc filename $<'					\
			-ex 'b *0x7c00'								\
			-ex 'c';									\
	else												\
		echo -e "\e[31m[FAIL]\e[0m missing debug file";	\
	fi

# unios rules
include $(PROJMK_PREFIX)rules-gen.mk
include $(PROJMK_PREFIX)rules-obj.mk
include $(PROJMK_PREFIX)rules-asbin.mk
include $(PROJMK_PREFIX)rules-lib.mk
include $(PROJMK_PREFIX)rules-user.mk
include $(PROJMK_PREFIX)rules-kernel.mk
include $(PROJMK_PREFIX)rules-image.mk
include $(PROJMK_PREFIX)rules-tools.mk
include $(PROJMK_PREFIX)rules-deps.mk

# format rules
format:
	@\
	TARGET="clang-format";											\
	MINVER=17;														\
	echo -ne "[PROC] format sources\r";								\
	FORMATTER=`which $${TARGET} 2> /dev/null`;						\
	if [ -z "$${FORMATTER}" ]; then 								\
		echo -e "\e[31m[FAIL]\e[0m $${TARGET} is not available";	\
		exit;														\
	fi;																\
	VERSION=`$${FORMATTER} --version | grep -Po '(\d+)(?=\.\d+\.\d+)'`;		\
	if [ "$${VERSION}" -lt $${MINVER} ]; then								\
		echo -e "\e[31m[FAIL]\e[0m requires $${TARGET} >= $${MINVER}.0.0";	\
		exit;																\
	fi;																		\
	$${FORMATTER} -i `git ls-files '*.c' '*.h'`;							\
	echo -e "\e[1K\r\e[32m[DONE]\e[0m format sources";
.PHONY: format

# install rules
install:
	@\
	files=\
	"		$(USER_PROG_FILES) 		\
			$(KERNEL_FILE) 			\
			$(KERNEL_DEBUG_FILE) 	\
			$(LIBRT_FILE)			\
			$(TOOLS_EXECUTABLE)		\
	";								\
	for file in $${files}; do 		\
		target=`basename $${file}`;									\
		echo -ne "[PROC] install $${target}\r";						\
		if [ -e "$${file}" ]; then									\
			cp -t $(OBJDIR) $${file}; 								\
			echo -e "\e[1K\r\e[32m[DONE]\e[0m install $${target}";	\
		else														\
			echo -e "\e[1K\r\e[33m[SKIP]\e[0m install $${target}";	\
		fi;															\
	done
.PHONY: install

# compile_commands.json rules
$(OBJDIR)/compile_commands.json: force
	@echo -ne "[PROC] dump $(notdir $@)\r"
	@mkdir -p $(@D)
	@bear --output $@ -- $(MAKE) -B > /dev/null 2>&1
	@echo -e "\e[1K\r\e[32m[DONE]\e[0m dump $(notdir $@)"
.PHONY: force

dup-cc: $(OBJDIR)/compile_commands.json

# extra configures
.DELETE_ON_ERROR:
.PRECIOUS: $(OBJECT_FILES) $(CACHED_FILES)

# target aliases
config: dup-cc
conf: config
build: all
b: build
r: run
d: debug
i: install
gdb: monitor
mon: monitor
mon-real: monitor-real
lib: $(LIBRT_FILE)
user: $(USER_TAR_FILE)
kernel: $(KERNEL_FILE) $(KERNEL_DEBUG_FILE)
krnl: kernel
image: $(IMAGE_FILE)
tools: $(TOOLS_EXECUTABLE)
fmt: format
