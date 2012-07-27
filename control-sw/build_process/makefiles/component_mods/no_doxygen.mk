#
# this trick prevents flood of messages from doxygen. this is needed for some
# external components that are not docummented full, or sometimes at all.
#
# to use this mode add following line to user_pre.mk file in component's dir:
# include $(MAKEFILES_COMPONENT_MODS_DIR)/no_doxygen.mk
#
html/index.html:: Doxyfile
	@echo "(note: suppressing doxygen warnings for $(COMPONENT_NAME) component)"
	@cp 'Doxyfile' 'Doxyfile.prev'
	@sed -e 's:^\(WARNINGS \+=\).*:\1 NO:' \
	     -e 's:^\(WARN_IF_UNDOCUMENTED \+=\).*:\1 NO:' \
	     -e 's:^\(WARN_IF_DOC_ERROR \+=\).*:\1 NO:' \
        'Doxyfile' > 'Doxyfile.no_warn' || ( rm -f 'Dexyfile' ; false )
	@mv 'Doxyfile.no_warn' 'Doxyfile'
