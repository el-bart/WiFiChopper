#
# force-link names finder
#
FORCE_LINK_SYMBOLS=$(shell $(NM) -P $^ | grep 'FORCE_LINK_THIS_OBJECT__[^ ]*__END.* [A-Za-z] ' | sed -e 's: .*::g' -e 's:^:-u :' | sort | uniq)
