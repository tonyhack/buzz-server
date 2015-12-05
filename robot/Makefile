MAKE = make --no-print-directory

define ECHO
	@printf "\033[;31m"; printf $1; printf "\033[0m\n"
endef

.PHONY: debug release clean profile

dep-%:
	@$(call ECHO, "[build libbrickredcore]")
	@$(MAKE) -f mak/libbrickredcore.mak $* 
	@$(call ECHO, "[build libtinyxml2]")
	@$(MAKE) -f mak/libtinyxml2.mak $*
	@$(call ECHO, "[build libbuzzprotocol]")
	@$(MAKE) -f mak/libbuzzprotocol.mak $*

debug release clean profile:
	@$(call ECHO, "[build buzzrobot]")
	@$(MAKE) -f mak/buzzrobot.mak $@

sync:
	tools/sync_buzz_protocol.sh
	tools/sync_buzz_etc.sh

