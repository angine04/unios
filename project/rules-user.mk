# build user programs
$(OBJDIR)user/%: $(OBJDIR)user/%.c.obj $(USER_DEPS_OBJECTS) $(LIBRT_FILE) $(LIBGCC_FILE)
	@$(call begin-job,ld,$(notdir $@))
	@mkdir -p $(@D)
	@$(LD) $(LDFLAGS) -o $@ $^
	@$(call end-job,done,ld,$(notdir $@))

USER_ASSETS := $(wildcard assets/*)

# archive user programs and assets
$(USER_TAR_FILE): $(USER_PROG_FILES) $(USER_ASSETS)
	@$(call begin-job,tar,$(notdir $@))
	@mkdir -p $(@D)
	@tar -vcf $@ -C $(OBJDIR)user $(notdir $(USER_PROG_FILES)) > /dev/null 2>&1
	@tar -rvf $@ -C assets $(notdir $(USER_ASSETS)) > /dev/null 2>&1
	@$(call end-job,done,tar,$(notdir $@))
