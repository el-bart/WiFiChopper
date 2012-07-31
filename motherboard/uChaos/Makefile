include $(TOPDIR)/rules.mk

PKG_NAME:=uChaos
PKG_RELEASE:=0.0.0

include $(INCLUDE_DIR)/uclibc++.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=WiFiChopper's uC board (i.e. motherboard) driver.
  DEPENDS:=+uclibcxx
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gen/release/uChaos $(1)/bin/
endef

$(eval $(call BuildPackage,uChaos))
